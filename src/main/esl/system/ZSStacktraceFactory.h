#ifndef ESL_SYSTEM_ZSSTACKTRACEFACTORY_H_
#define ESL_SYSTEM_ZSSTACKTRACEFACTORY_H_

#include <esl/system/Stacktrace.h>
#include <esl/system/StacktraceFactory.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace esl {
inline namespace v1_6 {
namespace system {

class ZSStacktraceFactory : public StacktraceFactory {
public:
	struct Settings {
		Settings();
		Settings(const std::vector<std::pair<std::string, std::string>>& settings);

		unsigned int skipEntries = 3;
		bool showAddress = true;
		bool showFunction = true;
	};

	ZSStacktraceFactory(const Settings& settings);

	static std::unique_ptr<StacktraceFactory> create(const std::vector<std::pair<std::string, std::string>>& settings);
	static std::unique_ptr<StacktraceFactory> createNative(const Settings& settings = Settings());

    std::unique_ptr<Stacktrace> createStacktrace() override;

private:
	std::unique_ptr<StacktraceFactory> stacktraceFactory;
};

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_SYSTEM_ZSSTACKTRACEFACTORY_H_ */
