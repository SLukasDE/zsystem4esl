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

#include <zsystem4esl/Process.h>
#include <zsystem4esl/Output.h>
#include <esl/logging/Logger.h>
#include <esl/Stacktrace.h>

namespace zsystem4esl {
namespace {
zsystem::Process::Output* getZsystemProcessOutput(esl::system::Interface::Process::Output* output) {
	if(output) {
		zsystem4esl::Output* zsystem4eslOutput = dynamic_cast<zsystem4esl::Output*>(output);
		if(zsystem4eslOutput == nullptr) {
			throw esl::addStacktrace(std::runtime_error("output object is not type of \"zsystem4esl::Output\""));
		}
		return &zsystem4eslOutput->output;
	}
	return nullptr;
}
}

void Process::enableTimeMeasurement(bool enabled) {
	process.enableTimeMeasurement(enabled);
}

void Process::setWorkingDirectory(const std::string& workingDirectory) {
	process.setWorkingDirectory(workingDirectory);
}

void Process::setStdOut(esl::system::Interface::Process::Output* output) {
	process.setStdOut(getZsystemProcessOutput(output));
}

void Process::setStdErr(esl::system::Interface::Process::Output* output) {
	process.setStdErr(getZsystemProcessOutput(output));
}
/*
void Process::setOutput(std::unique_ptr<esl::system::Interface::Process::Output> output, bool stdOut, bool stdErr) {
	zsystem4esl::Output* outputPtr = nullptr;
	if(output) {
		outputPtr = dynamic_cast<zsystem4esl::Output*>(output.get());
		if(outputPtr == nullptr) {
			throw esl::addStacktrace(std::runtime_error("object is not type of \"esl::zsystem::Output\""));
		}
		output.release();
	}

	if(stdOut) {
		// if previous output was specified for stdErr and stdErr,
		// then we have do move "outErr" to "err"
		if(outPtr == errPtr) {
			err = std::move(outErr);
		}
		outErr.reset(outputPtr);

		outPtr = outputPtr;

		if(stdErr) {
			errPtr = outputPtr;
		}
	}
	else if(stdErr) {
		err.reset(outputPtr);
		errPtr = outputPtr;
	}

	process.setStdOut(outPtr ? &outPtr->output : nullptr);
	process.setStdErr(errPtr ? &errPtr->output : nullptr);
}

esl::system::Interface::Process::Output* Process::getStdOut() const {
	return outPtr;
}

esl::system::Interface::Process::Output* Process::getStdErr() const {
	return errPtr;
}
*/
/* return true on success */
bool Process::execute(const std::string& executable, const std::list<std::string>& arguments) {
	return process.execute(executable, arguments);
}

int Process::wait() {
	return process.wait();
}

bool Process::isRunning() {
	return process.isRunning();
}

bool Process::isFailed() {
	return process.isFailed();
}

unsigned int Process::getTimeRealMS() const {
	return process.getTimeRealMS();
}

unsigned int Process::getTimeUserMS() const {
	return process.getTimeUserMS();
}

unsigned int Process::getTimeSysMS() const {
	return process.getTimeSysMS();
}

} /* namespace zsystem4esl */
