#ifndef SERVANT_CONN_HANDLER
#define SERVANT_CONN_HANDLER 1

#include <thread>
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
public:
    pid_t pid;
    WorkerState state;
    int id;
    std::thread *wt;
    Worker(int id);
    void processRequest();
    int request(int sockfd);
    void kill();
};

class Dispatcher{
private:
    int sockfd;
    Worker **workers; //Array of workers
    int num_workers;
public:
    Dispatcher(int sockfd, int num_workers);
    ~Dispatcher();
};

void threadRunner(Worker*);

#endif