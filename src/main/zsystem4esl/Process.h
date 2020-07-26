/*
MIT License
Copyright (c) 2019, 2020 Sven Lukas

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

#ifndef ZSYSTEM4ESL_PROCESS_H_
#define ZSYSTEM4ESL_PROCESS_H_

#include <esl/system/Interface.h>
#include <esl/system/process/Arguments.h>
#include <esl/system/process/Environment.h>

#include <zsystem/Process.h>

#include <string>
#include <memory>

namespace zsystem4esl {

class Output;

class Process : public esl::system::Interface::Process {
public:
	static std::unique_ptr<esl::system::Interface::Process> create(esl::system::process::Arguments arguments, std::string workingDir);
	static std::unique_ptr<esl::system::Interface::Process> createWithEnvironment(esl::system::process::Arguments arguments, esl::system::process::Environment environment, std::string workingDir);

	Process(esl::system::process::Arguments arguments, std::string workingDir);
	Process(esl::system::process::Arguments arguments, esl::system::process::Environment environment, std::string workingDir);

	int execute(const esl::system::Interface::Process::ParameterStreams& parameterStreams, esl::system::Interface::Process::ParameterFeatures& parameterFeatures) override;

private:
    zsystem::Process process;
};

} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_PROCESS_H_ */
