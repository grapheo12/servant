#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "logging/logger.h"
#include "core/init_connection.h"
#define SHOWERROR std::string(strerror(errno))

int EstablishConnection(const char* port, int backlog){
    /* Returns a listening socket
     * Or throws an error if something bad happens.
     * Logs the error before throwing.
     */

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    addrinfo *result;
    int status = getaddrinfo(NULL, port, &hints, &result);

    if (status != 0){
        Log(LogLevel::CRITICAL, gai_strerror(status));
        exit(status);
    }

    char buff[INET6_ADDRSTRLEN];
    int sockfd;
    bool bound = false;
    for (auto tmp = result; tmp != NULL; tmp = tmp->ai_next){
        if (tmp->ai_family == AF_INET){
            inet_ntop(AF_INET, &(((sockaddr_in *)(tmp->ai_addr))->sin_addr), buff, sizeof(buff));
            Log("Trying to connect to IPv4 " + std::string(buff));
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1){
                Log(LogLevel::CRITICAL, SHOWERROR);
            }else{
                Log("Socket created.");
            }
            Log("Trying to bind.");
            int status = bind(sockfd, tmp->ai_addr, tmp->ai_addrlen);
            if (status == -1){
                Log(LogLevel::CRITICAL, SHOWERROR);
                
                Log("Retrying....");
                int yes = 1;
                if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1){
                    Log(LogLevel::CRITICAL, SHOWERROR);
                }else{
                    Log("Bind Successful");
                    bound = true;
                    break;
                }
            }else{
                Log("Bind successful.");
                bound = true;
                break;
            }

        }else if (tmp->ai_family == AF_INET6){
            inet_ntop(AF_INET6, &(((sockaddr_in6 *)(tmp->ai_addr))->sin6_addr), buff, sizeof(buff));
            Log("Skipping IPv6 " + std::string(buff) + " for socket creation.");
        }else{
            Log("Unkown Address Family: " + tmp->ai_family);
        }
    }

    if (bound){
        Log("Listening on Port " + std::string(port));
        if (listen(sockfd, backlog) == -1){
            bound = false;
            Log(LogLevel::CRITICAL, SHOWERROR);
        }
    }

    if (!bound){
        throw errno;
    }

    return sockfd; 
}