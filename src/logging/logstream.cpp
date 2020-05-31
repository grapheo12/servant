#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include "logging/logstream.h"

LogStream::LogStream(std::vector<std::string>& paths, bool to_stdout)
    :paths(paths), to_stdout(to_stdout)
{
    files = new std::ofstream[paths.size() + 1];
    mutexes = new std::mutex[paths.size() + 1];
    for (int i = 0; i < paths.size(); i++){
        files[i].open(paths[i], std::ios::out | std::ios::app);
    }

}

LogStream::~LogStream(){
    for (int i = 0; i < paths.size(); i++){
        files[i].close();
    }
    std::cout << "I am destroyed" << std::endl;
}

const LogStream& operator<<(const LogStream& l, std::string& msg){
    for (int i = 0; i < l.paths.size(); i++){
        l.mutexes[i].lock();
        l.files[i] << msg;
        l.files[i].flush();
        l.mutexes[i].unlock();
    }
    if (l.to_stdout){
        l.mutexes[l.paths.size()].lock();
        std::cout << msg;
        std::cout.flush();
        l.mutexes[l.paths.size()].unlock();
    }

    return l;

}

const LogStream& operator<<(const LogStream& l, const char msg){
    for (int i = 0; i < l.paths.size(); i++){
        l.mutexes[i].lock();
        l.files[i] << msg;
        l.files[i].flush();
        l.mutexes[i].unlock();
    }
    if (l.to_stdout){
        l.mutexes[l.paths.size()].lock();
        std::cout << msg;
        std::cout.flush();
        l.mutexes[l.paths.size()].unlock();
    }

    return l;
}

const LogStream& operator<<(const LogStream& l, const std::string& msg){
   for (int i = 0; i < l.paths.size(); i++){
        l.mutexes[i].lock();
        l.files[i] << msg;
        l.files[i].flush();
        l.mutexes[i].unlock();
    }
    if (l.to_stdout){
        l.mutexes[l.paths.size()].lock();
        std::cout << msg;
        std::cout.flush();
        l.mutexes[l.paths.size()].unlock();
    }

    return l;

}

const LogStream& operator<<(const LogStream& l, int msg){
    for (int i = 0; i < l.paths.size(); i++){
        l.mutexes[i].lock();
        l.files[i] << msg;
        l.files[i].flush();
        l.mutexes[i].unlock();
    }
    if (l.to_stdout){
        l.mutexes[l.paths.size()].lock();
        std::cout << msg;
        std::cout.flush();
        l.mutexes[l.paths.size()].unlock();
    }

    return l;

}

