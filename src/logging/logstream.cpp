#include <iostream>
#include <fstream>
#include <vector>
#include "logging/logstream.h"

LogStream::LogStream(std::vector<std::string>& paths, bool to_stdout)
    :paths(paths), to_stdout(to_stdout)
{
    files = new std::ofstream[paths.size()];
    for (int i = 0; i < paths.size(); i++){
        files[i].open(paths[i], std::ios::out | std::ios::app);
    }
}

LogStream::~LogStream(){
    for (int i = 0; i < paths.size(); i++){
        files[i].close();
    }
}

const LogStream& operator<<(const LogStream& l, std::string& msg){
    for (int i = 0; i < l.paths.size(); i++){
        l.files[i] << msg;
        l.files[i].flush();
    }
    if (l.to_stdout){
        std::cout << msg;
        std::cout.flush();
    }

    return l;

}

const LogStream& operator<<(const LogStream& l, const char msg){
    for (int i = 0; i < l.paths.size(); i++){
        l.files[i] << msg;
        l.files[i].flush();
    }

    if (l.to_stdout){
        std::cout << msg;
        std::cout.flush();
    }

    return l;

}

const LogStream& operator<<(const LogStream& l, const std::string& msg){
    for (int i = 0; i < l.paths.size(); i++){
        l.files[i] << msg;
        l.files[i].flush();
    }

    if (l.to_stdout){
        std::cout << msg;
        std::cout.flush();
    }

    return l;

}

const LogStream& operator<<(const LogStream& l, int msg){
    for (int i = 0; i < l.paths.size(); i++){
        l.files[i] << msg;
        l.files[i].flush();
    }

    if (l.to_stdout){
        std::cout << msg;
        std::cout.flush();
    }

    return l;

}

