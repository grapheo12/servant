#ifndef SERVANT_LOGGER
#define SERVANT_LOGGER 1

#include <string>

enum class LogLevel{
    INFO = 1,
    DEBUG = 2,
    WARNING = 3,
    CRITICAL = 4
};

void Log(LogLevel, const std::string&);
void Log(const std::string&);

#endif