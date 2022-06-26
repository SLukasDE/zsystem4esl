/*
MIT License
Copyright (c) 2019-2022 Sven Lukas

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

#include <zsystem4esl/system/signal/DirectHandler.h>
#include <zsystem4esl/system/signal/Signal.h>
#include <zsystem4esl/system/signal/ThreadManager.h>

#include <zsystem/Signal.h>

#include <esl/utility/String.h>
#include <esl/system/stacktrace/IStacktrace.h>

#include <condition_variable>
#include <list>
#include <map>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>

namespace zsystem4esl {
namespace system {
namespace signal {

std::unique_ptr<esl::system::signal::ISignal> Signal::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<esl::system::signal::ISignal>(new Signal(settings));
}

Signal::Signal(const std::vector<std::pair<std::string, std::string>>& settings) {
	bool hasThreadedSignalHandler = false;

    for(const auto& setting : settings) {
		if(setting.first == "is-threaded") {
			if(hasThreadedSignalHandler) {
		        throw esl::system::stacktrace::IStacktrace::add(std::runtime_error("multiple definition of attribute 'is-threaded'."));
			}
			hasThreadedSignalHandler = true;
			std::string value = esl::utility::String::toLower(setting.second);
			if(value == "true") {
				threadedSignalHandler = true;
			}
			else if(value == "false") {
				threadedSignalHandler = false;
			}
			else {
		    	throw esl::system::stacktrace::IStacktrace::add(std::runtime_error("Invalid value \"" + setting.second + "\" for attribute 'is-threaded'"));
			}
		}
		else {
	        throw esl::system::stacktrace::IStacktrace::add(std::runtime_error("unknown attribute '\"" + setting.first + "\"'."));
		}
    }
}

esl::system::signal::ISignal::Handler Signal::createHandler(const esl::utility::Signal& signal, std::function<void()> function) {
	esl::system::signal::ISignal::Handler signalHandler;

	zsystem::Signal::Type signalType = zsystem::Signal::Type::unknown;
	if(signal == esl::utility::Signal::Type::hangUp) {
		signalType = zsystem::Signal::Type::hangUp;
	}
	else if(signal == esl::utility::Signal::Type::interrupt) {
		signalType = zsystem::Signal::Type::interrupt;
	}
	else if(signal == esl::utility::Signal::Type::quit) {
		signalType = zsystem::Signal::Type::quit;
	}
	else if(signal == esl::utility::Signal::Type::ill) {
		signalType = zsystem::Signal::Type::ill;
	}
	else if(signal == esl::utility::Signal::Type::trap) {
		signalType = zsystem::Signal::Type::trap;
	}
	else if(signal == esl::utility::Signal::Type::abort) {
		signalType = zsystem::Signal::Type::abort;
	}
	else if(signal == esl::utility::Signal::Type::busError) {
		signalType = zsystem::Signal::Type::busError;
	}
	else if(signal == esl::utility::Signal::Type::floatingPointException) {
		signalType = zsystem::Signal::Type::floatingPointException;
	}
	else if(signal == esl::utility::Signal::Type::segmentationViolation) {
		signalType = zsystem::Signal::Type::segmentationViolation;
	}
	else if(signal == esl::utility::Signal::Type::user1) {
		signalType = zsystem::Signal::Type::user1;
	}
	else if(signal == esl::utility::Signal::Type::user2) {
		signalType = zsystem::Signal::Type::user2;
	}
	else if(signal == esl::utility::Signal::Type::alarm) {
		signalType = zsystem::Signal::Type::alarm;
	}
	else if(signal == esl::utility::Signal::Type::child) {
		signalType = zsystem::Signal::Type::child;
	}
	else if(signal == esl::utility::Signal::Type::stackFault) {
		signalType = zsystem::Signal::Type::stackFault;
	}
	else if(signal == esl::utility::Signal::Type::terminate) {
		signalType = zsystem::Signal::Type::terminate;
	}
	else if(signal == esl::utility::Signal::Type::pipe) {
		signalType = zsystem::Signal::Type::pipe;
	}
	else {
		return nullptr;
	}

	if(threadedSignalHandler) {
		signalHandler = esl::system::signal::ISignal::Handler(ThreadManager::installThreadHandler(function, signalType).release());
	}
	else {
		signalHandler = esl::system::signal::ISignal::Handler(new DirectHandler(function, signalType));
	}

	return signalHandler;
}

} /* namespace signal */
} /* namespace system */
} /* namespace zsystem4esl */
