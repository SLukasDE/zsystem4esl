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

#ifndef ZSYSTEM4ESL_SYSTEM_STACKTRACE_STACKTRACE_H_
#define ZSYSTEM4ESL_SYSTEM_STACKTRACE_STACKTRACE_H_

#include <esl/monitoring/Streams.h>
#include <esl/system/DefaultStacktraceFactory.h>
#include <esl/system/Stacktrace.h>
#include <esl/system/StacktraceFactory.h>

#include <zsystem/Backtrace.h>

#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace zsystem4esl {
inline namespace v1_6 {
namespace system {
namespace stacktrace {

class Stacktrace : public esl::system::Stacktrace {
public:
	Stacktrace(const esl::system::DefaultStacktraceFactory::Settings& settings);

	void dump(std::ostream& stream) const override;
	void dump(esl::monitoring::Streams::Real& stream, esl::monitoring::Streams::Location location) const override;
	void dumpFull(std::ostream& stream) const override;
	void dumpFull(esl::monitoring::Streams::Real& stream, esl::monitoring::Streams::Location location) const override;
	std::unique_ptr<esl::system::Stacktrace> clone() const override;

private:
	esl::system::DefaultStacktraceFactory::Settings settings;

	zsystem::Backtrace backtrace;

	std::list<std::string> createElementsReduced() const;
};

} /* namespace stacktrace */
} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_STACKTRACE_STACKTRACE_H_ */
