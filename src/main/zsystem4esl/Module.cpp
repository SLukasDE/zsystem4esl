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

#include <zsystem4esl/Module.h>
#include <zsystem4esl/Process.h>
#include <zsystem4esl/Output.h>
#include <zsystem4esl/SignalHandler.h>
#include <zsystem4esl/Stacktrace.h>

#include <esl/system/Interface.h>
#include <esl/stacktrace/Interface.h>
#include <esl/module/Interface.h>

#include <memory>
#include <new>         // placement new
#include <type_traits> // aligned_storage

namespace zsystem4esl {

namespace {

class Module : public esl::module::Module {
public:
	Module();
};

typename std::aligned_storage<sizeof(Module), alignof(Module)>::type moduleBuffer; // memory for the object;
Module& module = reinterpret_cast<Module&> (moduleBuffer);
bool isInitialized = false;
//Module module;

esl::system::Interface::Process* createProcess() {
	return new Process;
}

esl::system::Interface::Process::Output* createOutputDefault() {
	return new OutputDefault();
}

esl::system::Interface::Process::Output* createOutputFile(const std::string& filename) {
	return new OutputFile(filename);
}

esl::system::Interface::Process::Output* createOutputPipe() {
	return new OutputPipe();
}

esl::stacktrace::Interface::Stacktrace* createStacktrace() {
	return new Stacktrace;
}

Module::Module()
: esl::module::Module()
{
	esl::module::Module::initialize(*this);

	addInterface(std::unique_ptr<const esl::module::Interface>(new esl::system::Interface(
			getId(), "zsystem",
			&createProcess,
			&createOutputDefault, &createOutputPipe, &createOutputFile,
			&signalHandlerInstall, &signalHandlerRemove)));
	addInterface(std::unique_ptr<const esl::module::Interface>(new esl::stacktrace::Interface(
			getId(), "zsystem",
			&createStacktrace)));
}

} /* anonymous namespace */

esl::module::Module& getModule() {
	if(isInitialized == false) {
		/* ***************** *
		 * initialize module *
		 * ***************** */

		isInitialized = true;
		new (&module) Module(); // placement new
	}
	return module;
}

} /* namespace zsystem4esl */
