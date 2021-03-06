/*
MIT License
Copyright (c) 2019-2021 Sven Lukas

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

#include <zsystem4esl/Module.h>
#include <zsystem4esl/stacktrace/Stacktrace.h>
#include <zsystem4esl/system/Process.h>
#include <zsystem4esl/system/SignalHandler.h>

#include <esl/stacktrace/Interface.h>
#include <esl/system/Interface.h>
#include <esl/Module.h>

namespace zsystem4esl {

namespace {
const char* getImplementation() {
	return "zsystem4esl";
}
} /* anonymous namespace */

void Module::install(esl::module::Module& module) {
	esl::setModule(module);

	module.addInterface(esl::system::Interface::createInterface(
			getImplementation(),
			&system::Process::create, &system::signalHandlerInstall, &system::signalHandlerRemove));
	module.addInterface(esl::stacktrace::Interface::createInterface(
			getImplementation(),
			&stacktrace::Stacktrace::create));
}

} /* namespace zsystem4esl */
