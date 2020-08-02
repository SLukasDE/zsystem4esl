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

#ifndef ZSYSTEM4ESL_PRODUCERFILE_H_
#define ZSYSTEM4ESL_PRODUCERFILE_H_

#include <esl/system/Interface.h>
#include <esl/object/Values.h>

#include <zsystem/process/ProducerFile.h>

#include <string>
#include <memory>

namespace zsystem4esl {

class ProducerFile : public esl::system::Interface::ProducerFile {
public:
	static std::unique_ptr<esl::system::Interface::ProducerFile> create(std::string filename, const esl::object::Values<std::string>& settings);

	ProducerFile(std::string filename, const esl::object::Values<std::string>& settings);

	/* return: FileDescriptor::npos
	 *           if there is no more data to produce (IMPORTANT)
	 *
	 *         Number of characters written to fileDescriptor
	 *           if there are data available to write to fileDescripor
	 *           (produced now or queued from previous call). */
	std::size_t write(esl::system::Interface::FileDescriptor& fileDescriptor) override;
	std::size_t getFileSize() const override;

	zsystem::process::ProducerFile& getProducerFile();

private:
	zsystem::process::ProducerFile producerFile;
};

} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_PRODUCERFILE_H_ */
