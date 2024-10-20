#include <esl/system/ZSProcess.h>

#include <zsystem4esl/system/process/Process.h>

#include <stdexcept>

namespace esl {
inline namespace v1_6 {
namespace system {

ZSProcess::Settings::Settings(const std::vector<std::pair<std::string, std::string>>& settings) {
    for(const auto& setting : settings) {
        throw std::runtime_error("unknown attribute '\"" + setting.first + "\"'.");
    }
}

ZSProcess::ZSProcess(const Settings& settings)
: process(createNative(settings))
{ }

std::unique_ptr<Process> ZSProcess::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<Process>(new ZSProcess(Settings(settings)));
}

std::unique_ptr<Process> ZSProcess::createNative(const Settings& settings) {
	return std::unique_ptr<Process>(new zsystem4esl::system::process::Process);
}

Transceiver& ZSProcess::operator[](const FileDescriptor& fd) {
	return (*process)[fd];
}

void ZSProcess::setWorkingDir(std::string workingDir) {
	process->setWorkingDir(std::move(workingDir));
}

void ZSProcess::setEnvironment(std::unique_ptr<Environment> environment) {
	process->setEnvironment(std::move(environment));
}

const Environment* ZSProcess::getEnvironment() const {
	return process->getEnvironment();
}

void ZSProcess::addFeature(object::Object& feature) {
	process->addFeature(feature);
}

int ZSProcess::execute(Arguments arguments) const {
	return process->execute(std::move(arguments));
}

void ZSProcess::sendSignal(const Signal& signal) const {
	process->sendSignal(signal);
}

const void* ZSProcess::getNativeHandle() const {
	return process->getNativeHandle();
}

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */
