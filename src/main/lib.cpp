#include <esl/module/Library.h>
#include <zsystem4esl/Module.h>

esl::module::Library::GetModule esl__module__library__getModule = &zsystem4esl::getModule;
