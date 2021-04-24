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
#include <zsystem4esl/Logger.h>

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

namespace zsystem4esl {
namespace system {

namespace {
Logger logger("zsystem4esl::system");
}

std::unique_ptr<esl::system::Interface::Process> Process::create(const esl::object::Values<std::string>&) {
	return std::unique_ptr<esl::system::Interface::Process>(new Process);
}

esl::system::Transceiver& Process::operator[](const esl::system::FileDescriptor& fd) {
	return transceivers[fd.getId()];
}

void Process::setWorkingDir(std::string aWorkingDir) {
	workingDir = std::move(aWorkingDir);
}

void Process::setEnvironment(std::unique_ptr<esl::system::Environment> aEnvironment) {
	environment = std::move(aEnvironment);
}

const esl::system::Environment* Process::getEnvironment() const {
	return environment.get();
}

void Process::addFeature(esl::object::Interface::Object& feature) {
}

int Process::execute(esl::system::Arguments arguments) const {
	std::map<std::string, std::unique_ptr<zsystem::process::ConsumerFile>> pathToZSystemConsumerFile;
	std::map<std::string, std::unique_ptr<zsystem::process::ProducerFile>> pathToZSystemProducerFile;
	zsystem::Process::ParameterStreams zsystemParameterStreams;
	std::map<esl::io::Consumer*, std::unique_ptr<zsystem::process::Consumer>> eslToZSystemConsumer;
	std::map<esl::io::Producer*, std::unique_ptr<zsystem::process::Producer>> eslToZSystemProducer;

	logger.trace << "zsystem4esl::...::Process: execute\n";

	for(auto& transceiver : transceivers) {
		zsystem::Process::ParameterStream& zystemParameterStream = zsystemParameterStreams[transceiver.first];

		switch(transceiver.first) {
		case 0:
			logger.trace << "Process tranceiver 0 (stdin).\n";
			break;
		case 1:
			logger.trace << "Process tranceiver 1 (stdout).\n";
			break;
		case 2:
			logger.trace << "Process tranceiver 2 (stderr).\n";
			break;
		default:
			logger.trace << "Process tranceiver " << transceiver.first << ".\n";
			break;
		}

		if(transceiver.second.getInput()) {
			logger.trace << "- got input\n";

			std::unique_ptr<zsystem::process::Consumer>& zsystemConsumer = eslToZSystemConsumer[&transceiver.second.getInput().getConsumer()];
			if(!zsystemConsumer) {
				logger.trace << "  - create zsystem-consumer\n";
				zsystemConsumer.reset(new process::Consumer(transceiver.second.getInput().getConsumer()));
			}
			else {
				logger.trace << "  - zsystem-consumer exists already\n";
			}
			zystemParameterStream.consumer = zsystemConsumer.get();
		}
		else if(!transceiver.second.getInputPath().empty()) {
			std::string path = transceiver.second.getInputPath().generic_string();
			logger.trace << "  - got input path: \"" << path << "\"\n";

			std::unique_ptr<zsystem::process::ConsumerFile>& zsystemConsumerFile = pathToZSystemConsumerFile[path];
			if(!zsystemConsumerFile) {
				logger.trace << "-> create zsystem-consumer-FILE\n";
				zsystemConsumerFile.reset(new zsystem::process::ConsumerFile(zsystem::process::FileDescriptor::openFile(path, false, true, true)));
			}
			else {
				logger.trace << "  - zsystem-consumer-FILE exists already\n";
			}
			zystemParameterStream.consumer = zsystemConsumerFile.get();
		}
		else {
			logger.trace << "- no input\n";
		}


		if(transceiver.second.getOutput()) {
			logger.trace << "- got output\n";

			std::unique_ptr<zsystem::process::Producer>& zsystemProducer = eslToZSystemProducer[&transceiver.second.getOutput().getProducer()];
			if(!zsystemProducer) {
				logger.trace << "  - create zsystem-producer\n";
				zsystemProducer.reset(new process::Producer(transceiver.second.getOutput().getProducer()));
			}
			else {
				logger.trace << "  - zsystem-producer exists already\n";
			}
			zystemParameterStream.producer = zsystemProducer.get();
		}
		else if(!transceiver.second.getOutputPath().empty()) {
			std::string path = transceiver.second.getOutputPath().generic_string();
			logger.trace << "- got output path: \"" << path << "\"\n";

			std::unique_ptr<zsystem::process::ProducerFile>& zsystemProducerFile = pathToZSystemProducerFile[path];
			if(!zsystemProducerFile) {
				logger.trace << "  - create zsystem-producer-FILE\n";
				zsystemProducerFile.reset(new zsystem::process::ProducerFile(zsystem::process::FileDescriptor::openFile(path, true, false, false)));
			}
			else {
				logger.trace << "  - zsystem-producer-FILE exists already\n";
			}

			zystemParameterStream.producer = zsystemProducerFile.get();
		}
		else {
			logger.trace << "- no output\n";
		}
	}

	zsystem::Process::ParameterFeatures zsystemParameterFeatures;
    zsystem::Process process(arguments.getArgs());
    process.setWorkingDir(workingDir);
	if(environment) {
		process.setEnvironment(std::unique_ptr<zsystem::process::Environment>(new zsystem::process::Environment(environment->getValues())));
	}
	else {
		process.setEnvironment(nullptr);
	}

	{
		const std::lock_guard<std::mutex> lock(processPtrMutex);
		processPtr = &process;
	}

	int rc = process.execute(zsystemParameterStreams, zsystemParameterFeatures);

	{
		const std::lock_guard<std::mutex> lock(processPtrMutex);
		processPtr = nullptr;
	}

	return rc;
}

void Process::sendSignal(esl::system::Interface::SignalType signal) const {
	zsystem::Process::Handle handle = getHandle();

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
	pid = getHandle();
	return pid == zsystem::Process::noHandle ? nullptr : &pid;
}

zsystem::Process::Handle Process::getHandle() const {
	const std::lock_guard<std::mutex> lock(processPtrMutex);
	return processPtr ? processPtr->getHandle() : zsystem::Process::noHandle;
}

} /* namespace system */
} /* namespace zsystem4esl */
