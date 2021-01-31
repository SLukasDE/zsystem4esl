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

#include <zsystem4esl/io/FileWriter.h>
#include <zsystem4esl/io/FileDescriptor.h>

namespace zsystem4esl {
namespace io {

std::unique_ptr<esl::utility::Writer> FileWriter::create(std::string filename, const esl::object::Values<std::string>& settings) {
	return std::unique_ptr<esl::utility::Writer>(new FileWriter(std::move(filename), settings));
}

FileWriter::FileWriter(std::string filename, const esl::object::Values<std::string>& settings)
: fileDescriptor(zsystem::process::FileDescriptor::openFile(
		filename,
		FileDescriptor::getIsRead(false, settings),
		FileDescriptor::getIsWrite(true, settings),
		FileDescriptor::getDoOverwrite(true, settings)))
{ }

std::size_t FileWriter::write(const void* data, std::size_t size) {
	return fileDescriptor.write(data, size);
}

zsystem::process::ConsumerFile& FileWriter::getConsumerFile() {
	if(!consumerFile) {
		consumerFile.reset(new zsystem::process::ConsumerFile(std::move(fileDescriptor)));
	}
	return *consumerFile;
}

} /* namespace io */
} /* namespace zsystem4esl */
