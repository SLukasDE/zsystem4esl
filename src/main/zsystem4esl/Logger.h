#ifndef ZSYSTEM4ESL_LOGGER_H_
#define ZSYSTEM4ESL_LOGGER_H_

#include <esl/logging/Logger.h>
#include <esl/logging/Level.h>


namespace zsystem4esl {

#ifdef ZSYSTEM4ESL_LOGGING_LEVEL_DEBUG
using Logger = esl::logging::Logger<esl::logging::Level::TRACE>;
#else
using Logger = esl::logging::Logger<esl::logging::Level::ERROR>;
#endif

} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_LOGGER_H_ */
