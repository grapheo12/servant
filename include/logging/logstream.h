#ifndef SERVANT_LOG_STREAM
#define SERVANT_LOG_STREAM 1

#include <vector>
#include <string>
#include <fstream>
#include <mutex>

class LogStream{
private:
    std::ofstream *files;
    std::vector<std::string> paths;

public:
    std::mutex* mutexes;
    bool to_stdout;
    LogStream(std::vector<std::string>& paths, bool to_stdout);
    LogStream(){}
    ~LogStream();

    friend const LogStream& operator<<(const LogStream&, std::string&);
    friend const LogStream& operator<<(const LogStream&, const char);
    friend const LogStream& operator<<(const LogStream&, int);
    friend const LogStream& operator<<(const LogStream&, const std::string&);
};

const LogStream& operator<<(const LogStream&, std::string&);
const LogStream& operator<<(const LogStream&, const char);
const LogStream& operator<<(const LogStream&, int);
const LogStream& operator<<(const LogStream&, const std::string&);
#endif