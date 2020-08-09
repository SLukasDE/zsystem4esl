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

#include <zsystem4esl/io/FileDescriptor.h>

#include <esl/utility/String.h>

namespace zsystem4esl {
namespace io {

bool FileDescriptor::getIsRead(bool isRead, const esl::object::Values<std::string>& values) {
	if(values.hasValue("read")) {
		std::string value = esl::utility::String::toLower(values.getValue("read"));
	    if(!value.empty()) {
	    	isRead = (value != "0" && value != "false");
	    }
	}

	return isRead;
}

bool FileDescriptor::getIsWrite(bool isWrite, const esl::object::Values<std::string>& values) {
	if(values.hasValue("write")) {
		std::string value = esl::utility::String::toLower(values.getValue("write"));
	    if(!value.empty()) {
	    	isWrite = (value != "0" && value != "false");
	    }
	}

	return isWrite;
}

bool FileDescriptor::getDoOverwrite(bool doOverwrite, const esl::object::Values<std::string>& values) {
	if(values.hasValue("overwrite")) {
		std::string value = esl::utility::String::toLower(values.getValue("overwrite"));
	    if(!value.empty()) {
	    	doOverwrite = (value != "0" && value != "false");
	    }
	}

	return doOverwrite;
}

} /* namespace io */
} /* namespace zsystem4esl */
