#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <mutex>

#include "logging/logger.h"
#include "logging/logstream.h"
#define TIMESTRMAXLEN 100

LogStream ss;
std::mutex ms;

void InitLogger(){
    std::vector<std::string> logpaths;
    logpaths.push_back("/tmp/servant.log");
    LogStream buff(logpaths, true);
    ss = buff;
}

void Log(LogLevel lvl, const std::string& msg){
    std::string levelstr;
    switch (lvl){
        case LogLevel::INFO:
            levelstr = "[INFO] ";
            break;
        case LogLevel::DEBUG:
            levelstr = "[DEBUG] ";
            break;
        case LogLevel::WARNING:
            levelstr = "[WARNING] ";
            break;
        case LogLevel::CRITICAL:
            levelstr = "[CRITICAL] ";
            break;
        default:
            levelstr = "[UNKNOWN] ";
            break;
    }

    pid_t id = gettid();

    //TODO: Make Logging configurable


    auto timenow =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buff[TIMESTRMAXLEN];
    strftime(buff, TIMESTRMAXLEN, "%c", localtime(&timenow));
    
    ms.lock();
    ss << levelstr;
    // ss << "[" << __FILE__ << "] "; //Find a way to print the file from where logging.
    ss << "[TID: " << id << " " << buff << "] ";
    ss << msg << "\n";
    ms.unlock();
    
}

void Log(const std::string& msg){
    Log(LogLevel::INFO, msg);
}
