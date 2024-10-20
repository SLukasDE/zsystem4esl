#ifndef ESL_SYSTEM_DEFAULTSIGNALMANAGER_H_
#define ESL_SYSTEM_DEFAULTSIGNALMANAGER_H_

#include <esl/system/Signal.h>
#include <esl/system/SignalManager.h>

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace esl {
inline namespace v1_6 {
namespace system {

class DefaultSignalManager : public SignalManager {
public:
	struct Settings {
		Settings();
		Settings(const std::vector<std::pair<std::string, std::string>>& settings);

		bool isThreaded = true;
	};

	DefaultSignalManager(const Settings& settings);

	static std::unique_ptr<SignalManager> create(const std::vector<std::pair<std::string, std::string>>& settings);
	static std::unique_ptr<SignalManager> createNative(const Settings& settings = Settings());

	Handler createHandler(const Signal& signal, std::function<void()> function) override;

private:
	std::unique_ptr<SignalManager> signalManager;
};

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_SYSTEM_DEFAULTSIGNALMANAGER_H_ */
