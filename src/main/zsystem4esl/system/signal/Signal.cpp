/*
MIT License
Copyright (c) 2019-2023 Sven Lukas

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

#include <esl/system/Stacktrace.h>
#include <esl/utility/String.h>

#include <condition_variable>
#include <list>
#include <map>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>

namespace zsystem4esl {
inline namespace v1_6 {
namespace system {
namespace signal {

Signal::Signal(const esl::system::DefaultSignalManager::Settings& settings)
: threadedSignalHandler(settings.isThreaded)
{ }

esl::system::SignalManager::Handler Signal::createHandler(const esl::system::Signal& signal, std::function<void()> function) {
	esl::system::SignalManager::Handler signalHandler;

	zsystem::Signal::Type signalType = zsystem::Signal::Type::unknown;
	if(signal == esl::system::Signal::Type::hangUp) {
		signalType = zsystem::Signal::Type::hangUp;
	}
	else if(signal == esl::system::Signal::Type::interrupt) {
		signalType = zsystem::Signal::Type::interrupt;
	}
	else if(signal == esl::system::Signal::Type::quit) {
		signalType = zsystem::Signal::Type::quit;
	}
	else if(signal == esl::system::Signal::Type::ill) {
		signalType = zsystem::Signal::Type::ill;
	}
	else if(signal == esl::system::Signal::Type::trap) {
		signalType = zsystem::Signal::Type::trap;
	}
	else if(signal == esl::system::Signal::Type::abort) {
		signalType = zsystem::Signal::Type::abort;
	}
	else if(signal == esl::system::Signal::Type::busError) {
		signalType = zsystem::Signal::Type::busError;
	}
	else if(signal == esl::system::Signal::Type::floatingPointException) {
		signalType = zsystem::Signal::Type::floatingPointException;
	}
	else if(signal == esl::system::Signal::Type::segmentationViolation) {
		signalType = zsystem::Signal::Type::segmentationViolation;
	}
	else if(signal == esl::system::Signal::Type::user1) {
		signalType = zsystem::Signal::Type::user1;
	}
	else if(signal == esl::system::Signal::Type::user2) {
		signalType = zsystem::Signal::Type::user2;
	}
	else if(signal == esl::system::Signal::Type::alarm) {
		signalType = zsystem::Signal::Type::alarm;
	}
	else if(signal == esl::system::Signal::Type::child) {
		signalType = zsystem::Signal::Type::child;
	}
	else if(signal == esl::system::Signal::Type::stackFault) {
		signalType = zsystem::Signal::Type::stackFault;
	}
	else if(signal == esl::system::Signal::Type::terminate) {
		signalType = zsystem::Signal::Type::terminate;
	}
	else if(signal == esl::system::Signal::Type::pipe) {
		signalType = zsystem::Signal::Type::pipe;
	}
	else {
		return nullptr;
	}

	if(threadedSignalHandler) {
		signalHandler = esl::system::SignalManager::Handler(ThreadManager::installThreadHandler(function, signalType).release());
	}
	else {
		signalHandler = esl::system::SignalManager::Handler(new DirectHandler(function, signalType));
	}

	return signalHandler;
}

} /* namespace signal */
} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace zsystem4esl */
