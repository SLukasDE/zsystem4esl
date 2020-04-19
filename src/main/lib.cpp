#include <esl/module/Library.h>
#include <zsystem4esl/Module.h>

extern "C" esl::module::Module* esl__module__library__getModule(const std::string& moduleName) {
	return zsystem4esl::getModulePointer(moduleName);
}
