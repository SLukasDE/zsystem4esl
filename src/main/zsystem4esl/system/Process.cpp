/*
MIT License
Copyright (c) 2019, 2020 Sven Lukas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <zsystem4esl/io/FileReader.h>
#include <zsystem4esl/io/FileWriter.h>
#include <zsystem4esl/system/Process.h>
#include <zsystem4esl/system/process/Consumer.h>
#include <zsystem4esl/system/process/Producer.h>

#include <zsystem/process/Arguments.h>
#include <zsystem/process/Environment.h>
#include <zsystem/process/Consumer.h>
#include <zsystem/process/Producer.h>
#include <zsystem/process/ConsumerFile.h>
#include <zsystem/process/ProducerFile.h>

#include <esl/io/FileReader.h>
#include <esl/io/FileWriter.h>
#include <esl/Stacktrace.h>
#include <esl/system/process/ProducerReader.h>
#include <esl/system/process/ConsumerWriter.h>

#include <map>
#include <memory>

namespace zsystem4esl {
namespace system {

std::unique_ptr<esl::system::Interface::Process> Process::create(esl::system::process::Arguments arguments, std::string workingDir, const esl::object::Values<std::string>&) {
	return std::unique_ptr<esl::system::Interface::Process>(new Process(std::move(arguments), std::move(workingDir)));
}

std::unique_ptr<esl::system::Interface::Process> Process::createWithEnvironment(esl::system::process::Arguments arguments, esl::system::process::Environment environment, std::string workingDir, const esl::object::Values<std::string>&) {
	return std::unique_ptr<esl::system::Interface::Process>(new Process(std::move(arguments), std::move(environment), std::move(workingDir)));
}

Process::Process(esl::system::process::Arguments arguments, std::string workingDir)
: process(zsystem::process::Arguments(arguments.getArgs()), std::move(workingDir))
{ }

Process::Process(esl::system::process::Arguments arguments, esl::system::process::Environment environment, std::string workingDir)
: process(zsystem::process::Arguments(arguments.getArgs()), zsystem::process::Environment(environment.getValues()), std::move(workingDir))
{ }

int Process::execute(const esl::system::Interface::Process::ParameterStreams& aParameterStreams, esl::system::Interface::Process::ParameterFeatures& aParameterFeatures) {
	zsystem::Process::ParameterStreams parameterStreams;
	zsystem::Process::ParameterFeatures parameterFeatures;

	std::map<esl::system::Interface::Consumer*, std::unique_ptr<zsystem::process::Consumer>> consumers;
	std::map<esl::system::Interface::Producer*, std::unique_ptr<zsystem::process::Producer>> producers;

	for(auto& parameterStream : aParameterStreams) {
		std::unique_ptr<zsystem::process::Consumer>& consumer = consumers[parameterStream.second.consumer];
		zsystem::process::ConsumerFile* zConsumerFile = nullptr;

		if(parameterStream.second.consumer) {
			esl::system::process::ConsumerWriter* eslConsumerWriter = dynamic_cast<esl::system::process::ConsumerWriter*>(parameterStream.second.consumer);
			if(eslConsumerWriter) {
				esl::io::FileWriter* eslFileWriter = dynamic_cast<esl::io::FileWriter*>(eslConsumerWriter->getWriter());
				if(eslFileWriter) {
					io::FileWriter* fileWriter = dynamic_cast<io::FileWriter*>(eslFileWriter->getBaseImplementation());
					if(fileWriter) {
						zConsumerFile = &fileWriter->getConsumerFile();
					}
				}
			}

			if(!zConsumerFile && !consumer) {
				consumer.reset(new process::Consumer(*parameterStream.second.consumer));
			}
		}

		std::unique_ptr<zsystem::process::Producer>& producer = producers[parameterStream.second.producer];
		zsystem::process::ProducerFile* zProducerFile = nullptr;

		if(parameterStream.second.producer) {
			esl::system::process::ProducerReader* eslProducerReader = dynamic_cast<esl::system::process::ProducerReader*>(parameterStream.second.producer);
			if(eslProducerReader) {
				esl::io::FileReader* eslFileReader = dynamic_cast<esl::io::FileReader*>(eslProducerReader->getReader());
				if(eslFileReader) {
					io::FileReader* fileReader = dynamic_cast<io::FileReader*>(eslFileReader->getBaseImplementation());
					if(fileReader) {
						zProducerFile = &fileReader->getProducerFile();
					}
				}
			}

			if(!zProducerFile && !producer) {
				producer.reset(new process::Producer(*parameterStream.second.producer));
			}
		}

		zsystem::Process::ParameterStream& zParameterStream = parameterStreams[parameterStream.first];
		zParameterStream.consumer = zConsumerFile ? zConsumerFile : consumer.get();
		zParameterStream.producer = zProducerFile ? zProducerFile : producer.get();
	}

	return process.execute(parameterStreams, parameterFeatures);
}

} /* namespace system */
} /* namespace zsystem4esl */
