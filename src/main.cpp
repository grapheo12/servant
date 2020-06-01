#include <iostream>
#include <string>
#include <cstdlib>
#include <signal.h>
#include <thread>
#include "core/init_connection.h"
#include "core/conn_handler.h"
#include "logging/logger.h"

#define PORT "2000"
#define BACKLOG 10
#define NUM_WORKERS 4

int main(){
    InitLogger();
    
    try{
        int sockfd = EstablishConnection(PORT, BACKLOG);
        Dispatcher disp(sockfd, NUM_WORKERS);
        disp.AcceptConnection();
    }catch(int e){
        exit(e);    //Generally modules throw errno
    }

    return 0; 
}