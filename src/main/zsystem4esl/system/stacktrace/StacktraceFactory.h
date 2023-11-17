/*
MIT License
Copyright (c) 2019-2023 Sven Lukas

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

#ifndef ZSYSTEM4ESL_SYSTEM_STACKTRACE_STACKTRACEFACTORY_H_
#define ZSYSTEM4ESL_SYSTEM_STACKTRACE_STACKTRACEFACTORY_H_

#include <esl/system/DefaultStacktraceFactory.h>
#include <esl/system/Stacktrace.h>
#include <esl/system/StacktraceFactory.h>

#include <memory>

namespace zsystem4esl {
inline namespace v1_6 {
namespace system {
namespace stacktrace {

class StacktraceFactory : public esl::system::StacktraceFactory {
public:
	StacktraceFactory(const esl::system::DefaultStacktraceFactory::Settings& settings);

    std::unique_ptr<esl::system::Stacktrace> createStacktrace() override;

private:
	esl::system::DefaultStacktraceFactory::Settings settings;
};

} /* namespace stacktrace */
} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_SYSTEM_STACKTRACE_STACKTRACEFACTORY_H_ */
