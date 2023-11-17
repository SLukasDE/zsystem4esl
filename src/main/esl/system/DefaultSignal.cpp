#include <esl/system/DefaultSignal.h>
#include <esl/system/Stacktrace.h>
#include <esl/utility/String.h>

#include <zsystem4esl/system/signal/Signal.h>

namespace esl {
inline namespace v1_6 {
namespace system {

DefaultSignal::Settings::Settings(const std::vector<std::pair<std::string, std::string>>& settings) {
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

DefaultSignal::DefaultSignal(const Settings& settings)
: signal(new zsystem4esl::system::signal::Signal(settings.isThreaded))
{ }

std::unique_ptr<Signal> DefaultSignal::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<Signal>(new DefaultSignal(Settings(settings)));
}

DefaultSignal::Handler DefaultSignal::createHandler(const utility::Signal& aSignal, std::function<void()> function) {
	return signal->createHandler(aSignal, function);
}

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */
