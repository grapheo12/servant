#include <string>
#include <cstdlib>
#include <signal.h>
#include "core/init_connection.h"
#include "core/conn_handler.h"
#include "logging/logger.h"

#define PORT "3000"
#define BACKLOG 10
#define NUM_WORKERS 4

Dispatcher *disp;
void sigint_handler(int signum){
    delete disp;
    exit(signum);
}
int main(){
    signal(SIGINT, sigint_handler);
    try{
        int sockfd = EstablishConnection(PORT, BACKLOG);
        disp = new Dispatcher(sockfd, NUM_WORKERS);
    }catch(int e){
        exit(e);    //Generally modules throw errno
    }

    return 0; 
}