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
    :id(id), wul(wm)
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
        w->wcv.wait(w->wul, [w]{ return (w->state != WorkerState::PROCESS && w->state != WorkerState::KILL); });
        if (w->state == WorkerState::KILL) {
            std::cout << "Got a kill signal. Let me die in peace." << std::endl;
            break;
        }else if (w->state == WorkerState::PROCESS){
            std::cout << "Ogo requrest eseche\n";
            w->state = WorkerState::BUSY;

            w->processRequest();

            w->state = WorkerState::READY;
            std::cout << "Ami ekhane" << w->id << "\n";
        }
    }
}

void Worker::processRequest(){
    char request[REQUEST_MAXLEN];
    int bytes_to_send = recv(sockfd, request, sizeof(request), 0);
    std::cout << "RW" << std::endl;
    //Just echo for now
    int bytes_sent = 0;
    int buff = 0;
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(1s);
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
    std::cout<<"t"<<id<<"\n";
    if (state == WorkerState::READY){
        std::cout<<"a"<<id<<"\n";
        this->sockfd = sockfd;
        state = WorkerState::PROCESS;
        return 0;
    }else{
        return -1;
    }
}

void Worker::kill(){
    state = WorkerState::KILL;
    wcv.notify_one();
    std::cout << "Main wahan ja chuka hun" << std::endl;
    wt->join();
    std::cout << "Tu jahan reh chuka hai" << std::endl;
}

Dispatcher::Dispatcher(int sockfd, int num_workers)
    :sockfd(sockfd), num_workers(num_workers)
{
    workers = new Worker*[num_workers];
    for (int i = 0; i < num_workers; i++){
        workers[i] = new Worker(i);
    }
}

void Dispatcher::AcceptConnection(){
    Log("Dispatcher Polling for connections.");

    //TODO: Replace with poll()
    bool pending = false;
    int currfd;
    sockaddr_storage remote_addr;
    socklen_t sz = sizeof(remote_addr);
    int idx_to_assign = 0;
    while (true){
        if (!pending){
            currfd = accept(sockfd, (sockaddr *)&remote_addr, &sz);
            pending = true;
        }

        for (int i = 0; i < num_workers; i++){
            if (workers[(i + idx_to_assign) % num_workers]->request(currfd) != -1){
                workers[(i + idx_to_assign) % num_workers]->wcv.notify_one();
                std::stringstream ss;
                ss << "Request sent to worker " << ((i + idx_to_assign) % num_workers);
                Log(ss.str());
                pending = false;
                idx_to_assign = (i + idx_to_assign + 1) % num_workers;
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

}


