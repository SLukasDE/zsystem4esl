#include <zsystem4esl/Module.h>

#include <esl/module/Library.h>
#include <esl/Module.h>

extern "C" esl::module::Module* esl__module__library__getModule(const std::string& moduleName) {
	if(moduleName == "esl") {
		return &esl::getModule();
	}
	return &zsystem4esl::getModule();
}
