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

#include <zsystem4esl/ProducerFile.h>
#include <zsystem4esl/FileDescriptor.h>

namespace zsystem4esl {

std::unique_ptr<esl::system::Interface::ProducerFile> ProducerFile::create(std::string filename, const esl::object::Values<std::string>& values) {
	return std::unique_ptr<esl::system::Interface::ProducerFile>(new ProducerFile(std::move(filename), values));
}

ProducerFile::ProducerFile(std::string filename, const esl::object::Values<std::string>& values)
: producerFile(FileDescriptor::getFileDescriptor(std::move(filename), true, false, false, values))
{ }

std::size_t ProducerFile::write(esl::system::Interface::FileDescriptor& aFileDescriptor) {
//	FileDescriptor fileDescriptor(aFileDescriptor);
//	return producerFile.write(fileDescriptor);
	return esl::system::Interface::FileDescriptor::npos;
}

std::size_t ProducerFile::getFileSize() const {
	return producerFile.getFileSize();
}

zsystem::process::ProducerFile& ProducerFile::getProducerFile() {
	return producerFile;
}

} /* namespace zsystem4esl */
