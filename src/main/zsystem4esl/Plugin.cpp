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

#include <zsystem4esl/Plugin.h>
#include <zsystem4esl/system/process/Process.h>
#include <zsystem4esl/system/signal/Signal.h>
#include <zsystem4esl/system/stacktrace/Stacktrace.h>

#include <esl/system/process/IProcess.h>
#include <esl/system/signal/ISignal.h>
#include <esl/system/stacktrace/IStacktrace.h>

#include <memory>

namespace zsystem4esl {

void Plugin::install(esl::plugin::Registry& registry, const char* data) {
	esl::plugin::Registry::set(registry);

	registry.addPlugin(std::unique_ptr<const esl::plugin::BasePlugin>(new esl::system::process::IProcess::Plugin(
			"zsystem4esl/system/process/Process",
			&system::process::Process::create)));

	registry.addPlugin(std::unique_ptr<const esl::plugin::BasePlugin>(new esl::system::signal::ISignal::Plugin(
			"zsystem4esl/system/signal/Signal",
			&system::signal::Signal::create)));

	registry.addPlugin(std::unique_ptr<const esl::plugin::BasePlugin>(new esl::system::stacktrace::IStacktrace::Plugin(
			"zsystem4esl/system/stacktrace/Stacktrace",
			&system::stacktrace::Stacktrace::create)));
}

} /* namespace zsystem4esl */
