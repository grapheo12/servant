/* Home of the Dispatcher and Worker Classes
 */
#include <iostream>
#include <thread>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "core/conn_handler.h"
#include "logging/logger.h"
#define REQUEST_MAXLEN 10000
#define WAIT_RETRY_MS 100


Worker::Worker(int id)
    :id(id)
    {}

void Worker::operator()(){
    pid = getpid();
    state = WorkerState::READY;
    //Log("Worker spawned.");

    while (state != WorkerState::KILL){
        if (state == WorkerState::READY){
            std::cout << "##$$" << id  << std::endl;
            std::this_thread::yield();
        }else if (state == WorkerState::PROCESS){
            state = WorkerState::BUSY;

            processRequest();

            state = WorkerState::READY;
        }
    }
}

void Worker::processRequest(){
    char request[REQUEST_MAXLEN];
    recv(sockfd, request, sizeof(request), 0);

    //Just echo for now
    int bytes_to_send = strlen(request) * sizeof(char);
    int bytes_sent = 0;
    int buff = 0;
    while (bytes_sent < bytes_to_send){
        buff = send(sockfd, request, sizeof(request), 0);
        if (buff == -1){
            Log(LogLevel::WARNING, "Request Failed");
            break;
        }else{
            bytes_sent += buff;
        }
    }
}

int Worker::request(int sockfd){
    std::stringstream ss; ss << (int)state; ss << " " << id; Log(ss.str());
    if (state == WorkerState::READY){
        this->sockfd = sockfd;
        state = WorkerState::PROCESS;
        return 0;
    }else{
        return -1;
    }
}

void Worker::kill(){
    state = WorkerState::KILL;
}

Dispatcher::Dispatcher(int sockfd, int num_workers)
    :sockfd(sockfd), num_workers(num_workers)
{
    workers = new Worker*[num_workers];
    tarr = new std::thread*[num_workers];
    for (int i = 0; i < num_workers; i++){
        workers[i] = new Worker(i);
        //std::stringstream ss; ss << (int)workers[i]->id; ss << " " << (int)workers[i]->state; Log(ss.str());
        tarr[i] = new std::thread(*workers[i]);
        std::stringstream ss; ss << (int)workers[i]->id; ss << " " << (int)workers[i]->state; Log(ss.str());
    }

    Log("Dispatcher Polling for connections.");

    //TODO: Replace with poll()
    bool pending = false;
    int currfd;
    sockaddr_storage remote_addr;
    socklen_t sz = sizeof(remote_addr);
    while (true){
        if (!pending){
            Log(LogLevel::DEBUG, "Main yahan");
            currfd = accept(sockfd, (sockaddr *)&remote_addr, &sz);
            pending = true;
        }

        for (int i = 0; i < num_workers; i++){
            if (workers[i]->request(currfd) != -1){
                Log("Request sent to worker " + i);
                pending = false;
                break;
            }else{
                Log(LogLevel::DEBUG, "Kat gya yaar");
            }
        }
    }
}

Dispatcher::~Dispatcher(){
    Log(LogLevel::CRITICAL, "Stopping Dispatcher");

    for (int i = 0; i < num_workers; i++)
        workers[i]->kill();

    for (int i = 0; i < num_workers; i++)
        tarr[i]->join();

    delete workers, tarr;

}


