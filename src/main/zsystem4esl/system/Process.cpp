/*
MIT License
Copyright (c) 2019-2021 Sven Lukas

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

#include <zsystem4esl/system/Process.h>
#include <zsystem4esl/system/process/Consumer.h>
#include <zsystem4esl/system/process/Producer.h>

#include <zsystem/process/Arguments.h>
#include <zsystem/process/Environment.h>
#include <zsystem/process/Consumer.h>
#include <zsystem/process/Producer.h>
#include <zsystem/process/ConsumerFile.h>
#include <zsystem/process/ProducerFile.h>
#include <zsystem/Signal.h>

#include <esl/io/Consumer.h>
#include <esl/io/Producer.h>
#include <esl/io/ProducerReader.h>
#include <esl/io/ConsumerWriter.h>
#include <esl/Stacktrace.h>

#include <signal.h> // sigaction(), sigsuspend(), sig*()

#include <tuple>
#include <map>
#include <memory>

namespace zsystem4esl {
namespace system {

std::unique_ptr<esl::system::Interface::Process> Process::create(esl::system::Arguments arguments, const esl::object::Values<std::string>&) {
	return std::unique_ptr<esl::system::Interface::Process>(new Process(std::move(arguments)));
}

Process::Process(esl::system::Arguments arguments)
: process(zsystem::process::Arguments(arguments.getArgs()))
{ }

void Process::setWorkingDir(std::string workingDir) {
	process.setWorkingDir(std::move(workingDir));
}

void Process::setEnvironment(std::unique_ptr<esl::system::Environment> aEnvironment) {
	environment = std::move(aEnvironment);

	if(environment) {
		process.setEnvironment(std::unique_ptr<zsystem::process::Environment>(new zsystem::process::Environment(environment->getValues())));
	}
	else {
		process.setEnvironment(nullptr);
	}
}

const esl::system::Environment* Process::getEnvironment() const {
	return environment.get();
}

void Process::sendSignal(esl::system::Interface::SignalType signal) {
	if(handle == zsystem::Process::noHandle) {
		return;
	}
	switch(signal) {
	case esl::system::Interface::SignalType::hangUp:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::hangUp);
		break;
	case esl::system::Interface::SignalType::interrupt:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::interrupt);
		break;
	case esl::system::Interface::SignalType::quit:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::quit);
		break;
	case esl::system::Interface::SignalType::ill:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::ill);
		break;
	case esl::system::Interface::SignalType::trap:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::trap);
		break;
	case esl::system::Interface::SignalType::abort:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::abort);
		break;
	case esl::system::Interface::SignalType::busError:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::busError);
		break;
	case esl::system::Interface::SignalType::floatingPointException:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::floatingPointException);
		break;
	case esl::system::Interface::SignalType::segmentationViolation:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::segmentationViolation);
		break;
	case esl::system::Interface::SignalType::user1:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::user1);
		break;
	case esl::system::Interface::SignalType::user2:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::user2);
		break;
	case esl::system::Interface::SignalType::alarm:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::alarm);
		break;
	case esl::system::Interface::SignalType::child:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::child);
		break;
	case esl::system::Interface::SignalType::stackFault:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::stackFault);
		break;
	case esl::system::Interface::SignalType::terminate:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::terminate);
		break;
	case esl::system::Interface::SignalType::pipe:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::pipe);
		break;
	case esl::system::Interface::SignalType::kill:
		zsystem::Signal::sendSignal(handle, zsystem::Signal::Type::kill);
		break;
	default:
		break;
	};
}

const void* Process::getNativeHandle() const {
	handle = process.getHandle();
	return handle == zsystem::Process::noHandle ? nullptr : &handle;
}

int Process::execute(esl::system::Interface::Process::ParameterStreams& aParameterStreams, esl::system::Interface::Process::ParameterFeatures&) {
	std::map<std::string, std::unique_ptr<zsystem::process::ConsumerFile>> pathToZSystemConsumerFile;
	std::map<std::string, std::unique_ptr<zsystem::process::ProducerFile>> pathToZSystemProducerFile;

	for(auto& parameterStream : aParameterStreams) {
		if(!parameterStream.second.getInput() && !parameterStream.second.getInputPath().empty()) {
			std::string path = parameterStream.second.getInputPath().generic_string();

			std::unique_ptr<zsystem::process::ConsumerFile>& zsystemConsumerFile = pathToZSystemConsumerFile[path];
			if(!zsystemConsumerFile) {
				zsystemConsumerFile.reset(new zsystem::process::ConsumerFile(zsystem::process::FileDescriptor::openFile(path, true, false, false)));
			}
		}

		if(!parameterStream.second.getOutput() && !parameterStream.second.getOutputPath().empty()) {
			std::string path = parameterStream.second.getOutputPath().generic_string();

			std::unique_ptr<zsystem::process::ProducerFile>& zsystemProducerFile = pathToZSystemProducerFile[path];
			if(!zsystemProducerFile) {
				zsystemProducerFile.reset(new zsystem::process::ProducerFile(zsystem::process::FileDescriptor::openFile(path, false, true, true)));
			}
		}
	}

	zsystem::Process::ParameterStreams zsystemParameterStreams;
	std::map<esl::io::Consumer*, std::unique_ptr<zsystem::process::Consumer>> eslToZSystemConsumer;
	std::map<esl::io::Producer*, std::unique_ptr<zsystem::process::Producer>> eslToZSystemProducer;

	for(auto& parameterStream : aParameterStreams) {
		zsystem::Process::ParameterStream& zystemParameterStream = zsystemParameterStreams[parameterStream.first];

		if(parameterStream.second.getInput()) {
			std::unique_ptr<zsystem::process::Consumer>& zsystemConsumer = eslToZSystemConsumer[&parameterStream.second.getInput().getConsumer()];
			if(!zsystemConsumer) {
				zsystemConsumer.reset(new process::Consumer(parameterStream.second.getInput().getConsumer()));
			}
			zystemParameterStream.consumer = zsystemConsumer.get();
		}
		else if(!parameterStream.second.getInputPath().empty()) {
			std::string path = parameterStream.second.getInputPath().generic_string();
			zystemParameterStream.consumer = pathToZSystemConsumerFile[path].get();
		}


		if(parameterStream.second.getOutput()) {
			std::unique_ptr<zsystem::process::Producer>& zsystemProducer = eslToZSystemProducer[&parameterStream.second.getOutput().getProducer()];
			if(!zsystemProducer) {
				zsystemProducer.reset(new process::Producer(parameterStream.second.getOutput().getProducer()));
			}
			zystemParameterStream.producer = zsystemProducer.get();
		}
		else if(!parameterStream.second.getOutputPath().empty()) {
			std::string path = parameterStream.second.getOutputPath().generic_string();
			zystemParameterStream.producer = pathToZSystemProducerFile[path].get();
		}
	}

	zsystem::Process::ParameterFeatures zsystemParameterFeatures;
	return process.execute(zsystemParameterStreams, zsystemParameterFeatures);
}

} /* namespace system */
} /* namespace zsystem4esl */
