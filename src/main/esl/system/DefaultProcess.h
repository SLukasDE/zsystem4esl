#ifndef ESL_SYSTEM_DEFAULTPROCESS_H_
#define ESL_SYSTEM_DEFAULTPROCESS_H_

#include <esl/object/Object.h>
#include <esl/system/Arguments.h>
#include <esl/system/Environment.h>
#include <esl/system/FileDescriptor.h>
#include <esl/system/Process.h>
#include <esl/system/Transceiver.h>
#include <esl/utility/Signal.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace esl {
inline namespace v1_6 {
namespace system {

class DefaultProcess : public Process {
public:
	struct Settings {
		Settings() = default;
		Settings(const std::vector<std::pair<std::string, std::string>>& settings);
	};

	DefaultProcess();
	DefaultProcess(const Settings& settings);

	static std::unique_ptr<Process> create(const std::vector<std::pair<std::string, std::string>>& settings);

	Transceiver& operator[](const FileDescriptor& fd) override;

	void setWorkingDir(std::string workingDir) override;
	void setEnvironment(std::unique_ptr<Environment> environment) override;
	const Environment* getEnvironment() const override;

	void addFeature(object::Object& feature) override;

	int execute(Arguments arguments) const override;

	void sendSignal(const utility::Signal& signal) const override;
	const void* getNativeHandle() const override;

private:
	std::unique_ptr<Process> process;
};

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_SYSTEM_DEFAULTPROCESS_H_ */
