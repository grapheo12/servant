#ifndef SERVANT_CONN_HANDLER
#define SERVANT_CONN_HANDLER 1

#include <thread>
#include <mutex>
#include <condition_variable>
enum class WorkerState{
    READY = 1,
    BUSY = 3,
    KILL = -1,
    PROCESS = 2
};

class Worker{
private:
    //pid_t pid;
    //WorkerState state;
    int sockfd;
    std::mutex wm;
    std::unique_lock<std::mutex> wul;
public:
    std::condition_variable wcv;
    pid_t pid;
    WorkerState state;
    int id;
    std::thread *wt;
    Worker(int id);
    void processRequest();
    int request(int sockfd);
    void kill();

    friend void threadRunner(Worker*);
};

class Dispatcher{
private:
    int sockfd;
    Worker **workers; //Array of workers
    int num_workers;
public:
    Dispatcher(int sockfd, int num_workers);
    ~Dispatcher();
    void AcceptConnection();
};

void threadRunner(Worker*);

#endif