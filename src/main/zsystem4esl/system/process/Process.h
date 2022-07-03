/*
MIT License
Copyright (c) 2019-2022 Sven Lukas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ZSYSTEM4ESL_SYSTEM_PROCESS_PROCESS_H_
#define ZSYSTEM4ESL_SYSTEM_PROCESS_PROCESS_H_

#include <esl/system/Process.h>
#include <esl/system/Transceiver.h>
#include <esl/system/Arguments.h>
#include <esl/system/Environment.h>
#include <esl/object/Object.h>

#include <zsystem/Process.h>

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace zsystem4esl {
namespace system {
namespace process {

class Process : public esl::system::Process {
public:
	static std::unique_ptr<esl::system::Process> create(const std::vector<std::pair<std::string, std::string>>& setting);

	esl::system::Transceiver& operator[](const esl::system::FileDescriptor& fd) override;

	void setWorkingDir(std::string workingDir) override;
	void setEnvironment(std::unique_ptr<esl::system::Environment> environment) override;
	const esl::system::Environment* getEnvironment() const override;

	void addFeature(esl::object::Object& feature) override;

	int execute(esl::system::Arguments arguments) const override;

	void sendSignal(const esl::utility::Signal& signal) const override;
	const void* getNativeHandle() const override;

	zsystem::Process::Handle getHandle() const;

private:
	std::map<int, esl::system::Transceiver> transceivers;
	std::string workingDir;
	std::unique_ptr<esl::system::Environment> environment;

	mutable std::mutex processPtrMutex;
	mutable zsystem::Process* processPtr = nullptr;
	mutable zsystem::Process::Handle pid = zsystem::Process::noHandle;
};

} /* namespace process */
} /* namespace system */
} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_SYSTEM_PROCESS_PROCESS_H_ */
