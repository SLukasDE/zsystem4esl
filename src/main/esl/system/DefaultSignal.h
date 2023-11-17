#ifndef ESL_SYSTEM_DEFAULTSIGNAL_H_
#define ESL_SYSTEM_DEFAULTSIGNAL_H_

#include <esl/system/Signal.h>
#include <esl/utility/Signal.h>

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace esl {
inline namespace v1_6 {
namespace system {

class DefaultSignal : public Signal {
public:
	struct Settings {
		Settings() = default;
		Settings(const std::vector<std::pair<std::string, std::string>>& settings);

		bool isThreaded = true;
	};

	DefaultSignal(const Settings& settings);

	static std::unique_ptr<Signal> create(const std::vector<std::pair<std::string, std::string>>& settings);

	Handler createHandler(const utility::Signal& signal, std::function<void()> function) override;

private:
	std::unique_ptr<Signal> signal;
};

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_SYSTEM_DEFAULTSIGNAL_H_ */
