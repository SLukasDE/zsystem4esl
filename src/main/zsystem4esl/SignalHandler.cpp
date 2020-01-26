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

#include <zsystem4esl/SignalHandler.h>
#include <zsystem/SignalHandler.h>

namespace zsystem4esl {

namespace {

zsystem::SignalHandler::Type translateSignalType(esl::system::Interface::SignalType signalType) {
	switch(signalType) {
	case esl::system::Interface::SignalType::hangUp:
		return zsystem::SignalHandler::Type::hangUp;
	case esl::system::Interface::SignalType::interrupt:
		return zsystem::SignalHandler::Type::interrupt;
	case esl::system::Interface::SignalType::quit:
		return zsystem::SignalHandler::Type::quit;
	case esl::system::Interface::SignalType::ill:
		return zsystem::SignalHandler::Type::ill;
	case esl::system::Interface::SignalType::trap:
		return zsystem::SignalHandler::Type::trap;
	case esl::system::Interface::SignalType::abort:
		return zsystem::SignalHandler::Type::abort;
	case esl::system::Interface::SignalType::busError:
		return zsystem::SignalHandler::Type::busError;
	case esl::system::Interface::SignalType::floatingPointException:
		return zsystem::SignalHandler::Type::floatingPointException;
	case esl::system::Interface::SignalType::segmentationViolation:
		return zsystem::SignalHandler::Type::segmentationViolation;
	case esl::system::Interface::SignalType::user1:
		return zsystem::SignalHandler::Type::user1;
	case esl::system::Interface::SignalType::user2:
		return zsystem::SignalHandler::Type::user2;
	case esl::system::Interface::SignalType::alarm:
		return zsystem::SignalHandler::Type::alarm;
	case esl::system::Interface::SignalType::child:
		return zsystem::SignalHandler::Type::child;
	case esl::system::Interface::SignalType::stackFault:
		return zsystem::SignalHandler::Type::stackFault;
	case esl::system::Interface::SignalType::terminate:
		return zsystem::SignalHandler::Type::terminate;
	case esl::system::Interface::SignalType::pipe:
		return zsystem::SignalHandler::Type::pipe;
	default:
		break;
	}

	return zsystem::SignalHandler::Type::unknown;
}
}

void signalHandlerInstall(esl::system::Interface::SignalType signalType, std::function<void()> handler) {
	zsystem::SignalHandler::install(translateSignalType(signalType), handler);
}

void signalHandlerRemove(esl::system::Interface::SignalType signalType, std::function<void()> handler) {
	zsystem::SignalHandler::remove(translateSignalType(signalType), handler);
}

} /* namespace zsystem4esl */
