#include <esl/system/ZSStacktraceFactory.h>
#include <esl/utility/String.h>

#include <zsystem4esl/system/stacktrace/StacktraceFactory.h>

#include <stdexcept>

namespace esl {
inline namespace v1_6 {
namespace system {

ZSStacktraceFactory::Settings::Settings() {
}

ZSStacktraceFactory::Settings::Settings(const std::vector<std::pair<std::string, std::string>>& settings) {
	bool hasSkipEntries = false;
	bool hasShowAddress = false;
	bool hasShowFunction = false;

    for(const auto& setting : settings) {
		if(setting.first == "skip-entries") {
			if(hasSkipEntries) {
	            throw std::runtime_error("zsystem4esl: multiple definition of attribute 'skip-entries'.");
			}
			hasSkipEntries = true;
			int tmpSkipEntries = std::stoi(setting.second);
			if(tmpSkipEntries < 0) {
	            throw std::runtime_error("zsystem4esl: Invalid negative value \"" + std::to_string(tmpSkipEntries) + "\" for attribute 'skip-entries'.");
			}
			skipEntries = static_cast<unsigned int>(tmpSkipEntries);
		}

		else if(setting.first == "show-address") {
			if(hasShowAddress) {
	            throw std::runtime_error("zsystem4esl: multiple definition of attribute 'show-address'.");
			}
			hasShowAddress = true;
			showAddress = esl::utility::String::toBool(setting.second);
		}

		else if(setting.first == "show-function") {
			if(hasShowFunction) {
	            throw std::runtime_error("zsystem4esl: multiple definition of attribute 'show-function'.");
			}
			hasShowFunction = true;
			showFunction = esl::utility::String::toBool(setting.second);
		}
		else {
	        throw esl::system::Stacktrace::add(std::runtime_error("unknown attribute '\"" + setting.first + "\"'."));
		}
    }
};

ZSStacktraceFactory::ZSStacktraceFactory(const Settings& settings)
: stacktraceFactory(createNative(settings))
{ }

std::unique_ptr<StacktraceFactory> ZSStacktraceFactory::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<StacktraceFactory>(new ZSStacktraceFactory(Settings(settings)));
}

std::unique_ptr<StacktraceFactory> ZSStacktraceFactory::createNative(const Settings& settings) {
	return std::unique_ptr<esl::system::StacktraceFactory>(new zsystem4esl::system::stacktrace::StacktraceFactory(settings));
}

std::unique_ptr<Stacktrace> ZSStacktraceFactory::createStacktrace() {
	return stacktraceFactory->createStacktrace();
}

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */
