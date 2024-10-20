#include <esl/system/DefaultProcess.h>

#include <zsystem4esl/system/process/Process.h>

#include <stdexcept>

namespace esl {
inline namespace v1_6 {
namespace system {

DefaultProcess::Settings::Settings(const std::vector<std::pair<std::string, std::string>>& settings) {
    for(const auto& setting : settings) {
        throw std::runtime_error("unknown attribute '\"" + setting.first + "\"'.");
    }
}

DefaultProcess::DefaultProcess(const Settings& settings)
: process(createNative(settings))
{ }

std::unique_ptr<Process> DefaultProcess::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<Process>(new DefaultProcess(Settings(settings)));
}

std::unique_ptr<Process> DefaultProcess::createNative(const Settings& settings) {
	return std::unique_ptr<Process>(new zsystem4esl::system::process::Process);
}

Transceiver& DefaultProcess::operator[](const FileDescriptor& fd) {
	return (*process)[fd];
}

void DefaultProcess::setWorkingDir(std::string workingDir) {
	process->setWorkingDir(std::move(workingDir));
}

void DefaultProcess::setEnvironment(std::unique_ptr<Environment> environment) {
	process->setEnvironment(std::move(environment));
}

const Environment* DefaultProcess::getEnvironment() const {
	return process->getEnvironment();
}

void DefaultProcess::addFeature(object::Object& feature) {
	process->addFeature(feature);
}

int DefaultProcess::execute(Arguments arguments) const {
	return process->execute(std::move(arguments));
}

void DefaultProcess::sendSignal(const Signal& signal) const {
	process->sendSignal(signal);
}

const void* DefaultProcess::getNativeHandle() const {
	return process->getNativeHandle();
}

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */
