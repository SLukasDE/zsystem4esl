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

#ifndef ZSYSTEM4ESL_SYSTEM_SIGNAL_THREADMANAGER_H_
#define ZSYSTEM4ESL_SYSTEM_SIGNAL_THREADMANAGER_H_

#include <zsystem4esl/system/signal/ThreadHandler.h>

#include <zsystem/SignalHandler.h>
#include <zsystem/Signal.h>

#include <esl/utility/Signal.h>

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <utility>

namespace zsystem4esl {
namespace system {
namespace signal {

class ThreadManager {
public:
	friend class ThreadHandler;

	static std::unique_ptr<ThreadHandler> installThreadHandler(std::function<void()> function, zsystem::Signal::Type signalType);

private:
	std::mutex registryMutex;
	std::map<zsystem::Signal::Type, std::pair<zsystem::SignalHandler::Handle, std::set<ThreadHandler*>>> registry;

	std::mutex messagesMutex;
	std::list<std::unique_ptr<zsystem::Signal::Type>> messages;

	ThreadManager() = default;

	// returns TRUE if ThreadHandler was last handler of given signal type
	static bool uninstallThreadHandler(zsystem::Signal::Type signalType, ThreadHandler& threadHandler);

	static void signalHandler(zsystem::Signal::Type signalType);
	static void run();
};

} /* namespace signal */
} /* namespace system */
} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_SYSTEM_SIGNAL_THREADMANAGER_H_ */
