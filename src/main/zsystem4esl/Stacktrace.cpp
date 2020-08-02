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

#include <zsystem4esl/Stacktrace.h>

namespace zsystem4esl {

std::unique_ptr<esl::stacktrace::Interface::Stacktrace> Stacktrace::create(const esl::object::Values<std::string>&) {
	return std::unique_ptr<esl::stacktrace::Interface::Stacktrace>(new Stacktrace);
}

void Stacktrace::dump(std::ostream& stream) const {
	stream << "Stacktrace:\n";
	for (const auto& entry : backtrace.getElements()) {
		stream << entry << "\n";
	}
}


void Stacktrace::dump(esl::logging::StreamReal& stream, esl::logging::Location location) const {
	stream(location.object, location.function, location.file, location.line) << "\nStacktrace:\n";
	for (const auto& entry : backtrace.getElements()) {
		stream(location.object, location.function, location.file, location.line) << entry << "\n";
	}
}

std::unique_ptr<esl::stacktrace::Interface::Stacktrace> Stacktrace::clone() const {
	return std::unique_ptr<esl::stacktrace::Interface::Stacktrace>(new Stacktrace(*this));
}

} /* namespace zsystem4esl */
