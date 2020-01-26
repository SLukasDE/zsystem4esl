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

#ifndef ZSYSTEM4ESL_OUTPUT_H_
#define ZSYSTEM4ESL_OUTPUT_H_

#include <esl/system/Interface.h>
#include <zsystem/Process.h>

namespace zsystem4esl {

class Process;

class Output : public esl::system::Interface::Process::Output {
public:
	Output(zsystem::Process::Output &aOutput)
    : esl::system::Interface::Process::Output(),
	  output(aOutput)
    { }
	~Output() = default;

	std::size_t read(void* buffer, std::size_t s) override {
		return output.read(buffer, s);
	}

	bool setBlocking(bool blocking) override {
		return output.setBlocking(blocking);
	}

	zsystem::Process::Output &output;
};

class OutputDefault : public Output {
public:
	OutputDefault()
    : Output(output)
    { }
	~OutputDefault() = default;

	zsystem::Process::Default output;
};

class OutputPipe : public Output {
public:
	OutputPipe()
    : Output(output)
    { }
	~OutputPipe() = default;

	zsystem::Process::Pipe output;
};

class OutputFile : public Output {
public:
	OutputFile(const std::string& filename)
    : Output(output),
	  output(filename)
    { }
	~OutputFile() = default;

	zsystem::Process::File output;
};

} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_OUTPUT_H_ */
