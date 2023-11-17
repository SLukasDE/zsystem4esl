#ifndef ESL_SYSTEM_DEFAULTSTACKTRACEFACTORY_H_
#define ESL_SYSTEM_DEFAULTSTACKTRACEFACTORY_H_

#include <esl/system/Stacktrace.h>
#include <esl/system/StacktraceFactory.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace esl {
inline namespace v1_6 {
namespace system {

class DefaultStacktraceFactory : public StacktraceFactory {
public:
	struct Settings {
		Settings() = default;
		Settings(const std::vector<std::pair<std::string, std::string>>& settings);

		unsigned int skipEntries = 3;
		bool showAddress = true;
		bool showFunction = true;
	};

	DefaultStacktraceFactory(const Settings& settings);

	static std::unique_ptr<StacktraceFactory> create(const std::vector<std::pair<std::string, std::string>>& settings);

    std::unique_ptr<Stacktrace> createStacktrace() override;

private:
	std::unique_ptr<StacktraceFactory> stacktraceFactory;
};

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_SYSTEM_DEFAULTSTACKTRACEFACTORY_H_ */
