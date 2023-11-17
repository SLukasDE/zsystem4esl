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

#include <zsystem4esl/system/signal/ThreadManager.h>

#include <condition_variable>
#include <thread>

namespace zsystem4esl {
inline namespace v1_6 {
namespace system {
namespace signal {

namespace {
std::mutex instanceMutex;
std::unique_ptr<ThreadManager> instance;

std::mutex newMessageCVMutex;
std::condition_variable newMessageCV;
} /* anonymous namespace */

std::unique_ptr<ThreadHandler> ThreadManager::installThreadHandler(std::function<void()> function, zsystem::Signal::Type signalType) {
	std::lock_guard<std::mutex> lockInstanceMutext(instanceMutex);
	if(!instance) {
		std::unique_lock<std::mutex> lockNewMessagCVMutex(newMessageCVMutex);

		instance.reset(new ThreadManager);
		std::thread(ThreadManager::run).detach();

		newMessageCV.wait(lockNewMessagCVMutex);
	}

	std::lock_guard<std::mutex> lockRegistryMutex(instance->registryMutex);

	auto registryInsertResult = instance->registry.insert(std::make_pair(signalType, std::pair<zsystem::SignalHandler::Handle, std::set<ThreadHandler*>>(zsystem::SignalHandler::Handle(), {})));
	if(registryInsertResult.second == true) {
		registryInsertResult.first->second.first = zsystem::SignalHandler::install(signalType, [signalType]{signalHandler(signalType);});
	}

	std::set<ThreadHandler*>& threadHandlers = registryInsertResult.first->second.second;

	std::unique_ptr<ThreadHandler> threadHandler(new ThreadHandler(signalType, function, instance, newMessageCVMutex, newMessageCV));
	threadHandlers.insert(threadHandler.get());

	return threadHandler;
}


bool ThreadManager::uninstallThreadHandler(zsystem::Signal::Type signalType, ThreadHandler& threadHandler) {
	std::lock_guard<std::mutex> lockInstanceMutext(instanceMutex);
	if(!instance) {
		return false;
	}

	std::lock_guard<std::mutex> lockRegistryMutex(instance->registryMutex);

	auto registryFindResult = instance->registry.find(signalType);
	if(registryFindResult != instance->registry.end()) {
		std::set<ThreadHandler*>& handlers = registryFindResult->second.second;
		handlers.erase(&threadHandler);

		if(handlers.empty()) {
			instance->registry.erase(registryFindResult);
		}
	}

	if(instance->registry.empty()) {
		{
			std::lock_guard<std::mutex> lockMessagesMutex(instance->messagesMutex);
			instance->messages.push_back(std::unique_ptr<zsystem::Signal::Type>(nullptr));
		}

		newMessageCV.notify_one();
		return true;
	}
	return false;
}

void ThreadManager::signalHandler(zsystem::Signal::Type signalType) {
	std::lock_guard<std::mutex> lockInstacneMutext(instanceMutex);

	std::lock_guard<std::mutex> lockMessagesMutex(instance->messagesMutex);
	instance->messages.push_back(std::unique_ptr<zsystem::Signal::Type>(new zsystem::Signal::Type(signalType)));

	newMessageCV.notify_one();
}

void ThreadManager::run() {
	newMessageCV.notify_one();

	std::unique_lock<std::mutex> lockNewMessageCVMutex(newMessageCVMutex);
	while(true) {
		newMessageCV.wait(lockNewMessageCVMutex, [] {
				std::lock_guard<std::mutex> lockMessagesMutex(instance->messagesMutex);
				return !instance->messages.empty();
		});

		std::unique_ptr<zsystem::Signal::Type> message;
		{
			std::lock_guard<std::mutex> lockMessagesMutex(instance->messagesMutex);
			message = std::move(instance->messages.front());
			instance->messages.pop_front();
		}

		if(message) {
			std::lock_guard<std::mutex> lockRegistryMutex(instance->registryMutex);

			auto events = instance->registry.find(*message);
			if(events != instance->registry.end()) {
				for(auto& handler : events->second.second) {
					try {
						handler->invoke();
					}
					catch(...) {
					}
				}
			}
		}
		else {
			bool isEmpty = false;
			{
				std::lock_guard<std::mutex> lockRegistryMutex(instance->registryMutex);
				isEmpty = instance->registry.empty();
			}
			if(isEmpty) {
				instance.reset();
				lockNewMessageCVMutex.unlock();
				newMessageCV.notify_one();
				return;
			}
		}
	}
}

} /* namespace signal */
} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace zsystem4esl */
