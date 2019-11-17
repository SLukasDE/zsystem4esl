/*
MIT License
Copyright (c) 2019 Sven Lukas

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
#include <zsystem/Process.h>
#include <string>
#include <list>
#include <unistd.h>
#include <memory>

namespace zsystem4esl {

class Output;

class Process : public esl::system::Interface::Process {
public:
    Process() = default;
    ~Process() = default;

    void enableTimeMeasurement(bool enabled) override;
    void setWorkingDirectory(const std::string& workingDirectory) override;

    //void setOutput(std::unique_ptr<esl::system::Interface::Process::Output> output, bool stdOut, bool stdErr) override;
    void setStdOut(esl::system::Interface::Process::Output* output) override;
    //esl::system::Interface::Process::Output* getStdOut() const override;

    void setStdErr(esl::system::Interface::Process::Output* output) override;
    //esl::system::Interface::Process::Output* getStdErr() const override;

    /* return true on success */
    bool execute(const std::string& executable, const std::list<std::string>& arguments) override;

    int wait() override;
    bool isRunning() override;
    bool isFailed() override;

    unsigned int getTimeRealMS() const override;
    unsigned int getTimeUserMS() const override;
    unsigned int getTimeSysMS() const override;

private:
    ::zsystem::Process process;
/*
    std::unique_ptr<zsystem4esl::Output> outErr;
    std::unique_ptr<zsystem4esl::Output> err;

    zsystem4esl::Output* outPtr = nullptr;
    zsystem4esl::Output* errPtr = nullptr;
*/
};

} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_PROCESS_H_ */
