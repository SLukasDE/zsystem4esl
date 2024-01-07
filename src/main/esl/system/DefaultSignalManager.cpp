#include <esl/system/DefaultSignalManager.h>
#include <esl/system/Stacktrace.h>
#include <esl/utility/String.h>

#include <zsystem4esl/system/signal/Signal.h>

namespace esl {
inline namespace v1_6 {
namespace system {

DefaultSignalManager::Settings::Settings() {
}

DefaultSignalManager::Settings::Settings(const std::vector<std::pair<std::string, std::string>>& settings) {
	bool hasThreadedSignalHandler = false;

    for(const auto& setting : settings) {
		if(setting.first == "is-threaded") {
			if(hasThreadedSignalHandler) {
		        throw esl::system::Stacktrace::add(std::runtime_error("multiple definition of attribute 'is-threaded'."));
			}
			hasThreadedSignalHandler = true;
			std::string value = esl::utility::String::toLower(setting.second);
			if(value == "true") {
				isThreaded = true;
			}
			else if(value == "false") {
				isThreaded = false;
			}
			else {
		    	throw esl::system::Stacktrace::add(std::runtime_error("Invalid value \"" + setting.second + "\" for attribute 'is-threaded'"));
			}
		}
		else {
	        throw esl::system::Stacktrace::add(std::runtime_error("unknown attribute '\"" + setting.first + "\"'."));
		}
    }
}

DefaultSignalManager::DefaultSignalManager(const Settings& settings)
: signalManager(createNative(settings))
{ }

std::unique_ptr<SignalManager> DefaultSignalManager::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<SignalManager>(new DefaultSignalManager(Settings(settings)));
}

std::unique_ptr<SignalManager> DefaultSignalManager::createNative(const Settings& settings) {
	return std::unique_ptr<esl::system::SignalManager>(new zsystem4esl::system::signal::Signal(settings));
}

SignalManager::Handler DefaultSignalManager::createHandler(const Signal& aSignal, std::function<void()> function) {
	return signalManager->createHandler(aSignal, function);
}

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */
