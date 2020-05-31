#include <iostream>
#include "logging/logger.h"

int main(){
    std::cout << "Running Test on Logger" << std::endl;
    Log(LogLevel::INFO, "Info");
    Log(LogLevel::DEBUG, "Debug");
    Log(LogLevel::WARNING, "Warning");
    Log(LogLevel::CRITICAL, "Critical");

    Log("Hello World");
    
}