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

#include <zsystem4esl/io/FileReader.h>
#include <zsystem4esl/io/FileDescriptor.h>

namespace zsystem4esl {
namespace io {

std::unique_ptr<esl::utility::Reader> FileReader::create(std::string filename, const esl::object::Values<std::string>& settings) {
	return std::unique_ptr<esl::utility::Reader>(new FileReader(std::move(filename), settings));
}

FileReader::FileReader(std::string filename, const esl::object::Values<std::string>& settings)
: fileDescriptor(zsystem::process::FileDescriptor::openFile(
		filename,
		FileDescriptor::getIsRead(true, settings),
		FileDescriptor::getIsWrite(false, settings),
		FileDescriptor::getDoOverwrite(false, settings)))
{ }

std::size_t FileReader::read(void* data, std::size_t size) {
	return fileDescriptor.read(data, size);
}

zsystem::process::ProducerFile& FileReader::getProducerFile() {
	if(!producerFile) {
		producerFile.reset(new zsystem::process::ProducerFile(std::move(fileDescriptor)));
	}
	return *producerFile;
}

} /* namespace io */
} /* namespace zsystem4esl */
