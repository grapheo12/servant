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
{
    wt = new std::thread(threadRunner, this);
}

void threadRunner(Worker* w){
    w->pid = gettid();
    w->state = WorkerState::READY;

    std::stringstream ss;
    ss << "Worker id " << w->id << " spawned";
    Log(ss.str());

    while (w->state != WorkerState::KILL){
        if (w->state == WorkerState::READY){
            std::this_thread::yield();
        }else if (w->state == WorkerState::PROCESS){
            w->state = WorkerState::BUSY;

            w->processRequest();

            w->state = WorkerState::READY;
        }
    }
}

void Worker::processRequest(){
    char request[REQUEST_MAXLEN];
    int bytes_to_send = recv(sockfd, request, sizeof(request), 0);

    //Just echo for now
    int bytes_sent = 0;
    int buff = 0;
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(10s);
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
    wt->join();
}

Dispatcher::Dispatcher(int sockfd, int num_workers)
    :sockfd(sockfd), num_workers(num_workers)
{
    workers = new Worker*[num_workers];
    for (int i = 0; i < num_workers; i++){
        workers[i] = new Worker(i);
    }

    Log("Dispatcher Polling for connections.");

    //TODO: Replace with poll()
    bool pending = false;
    int currfd;
    sockaddr_storage remote_addr;
    socklen_t sz = sizeof(remote_addr);
    while (true){
        if (!pending){
            currfd = accept(sockfd, (sockaddr *)&remote_addr, &sz);
            pending = true;
        }

        for (int i = 0; i < num_workers; i++){
            if (workers[i]->request(currfd) != -1){
                std::stringstream ss;
                ss << "Request sent to worker " << i;
                Log(ss.str());
                pending = false;
                break;
            }else{
               // Log(LogLevel::DEBUG, "Kat gya yaar");
            }
        }
    }
}

Dispatcher::~Dispatcher(){
    Log(LogLevel::CRITICAL, "Stopping Dispatcher");

    for (int i = 0; i < num_workers; i++)
        workers[i]->kill();

    delete workers;

}


