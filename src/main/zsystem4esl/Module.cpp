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
#include <zsystem4esl/io/FileReader.h>
#include <zsystem4esl/io/FileWriter.h>
#include <zsystem4esl/stacktrace/Stacktrace.h>
#include <zsystem4esl/system/Process.h>
#include <zsystem4esl/system/SignalHandler.h>

#include <esl/io/Interface.h>
#include <esl/Stacktrace.h>
#include <esl/stacktrace/Interface.h>
#include <esl/system/Interface.h>
#include <esl/module/Interface.h>

#include <stdexcept>
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
Module* modulePtr = nullptr;

const char* getImplementation() {
	return "zsystem4esl";
}

Module::Module()
: esl::module::Module()
{
	esl::module::Module::initialize(*this);

	addInterface(std::unique_ptr<const esl::module::Interface>(new esl::io::Interface(
			getId(), getImplementation(),
			&io::FileReader::create, &io::FileWriter::create)));
	addInterface(std::unique_ptr<const esl::module::Interface>(new esl::system::Interface(
			getId(), getImplementation(),
			&system::Process::create, &system::Process::createWithEnvironment,
			&system::signalHandlerInstall, &system::signalHandlerRemove)));
	addInterface(std::unique_ptr<const esl::module::Interface>(new esl::stacktrace::Interface(
			getId(), getImplementation(),
			&stacktrace::Stacktrace::create)));
}

} /* anonymous namespace */

esl::module::Module& getModule() {
	if(modulePtr == nullptr) {
		/* ***************** *
		 * initialize module *
		 * ***************** */

		modulePtr = reinterpret_cast<Module*> (&moduleBuffer);
		new (modulePtr) Module; // placement new
	}

	return *modulePtr;
}

} /* namespace zsystem4esl */
