#include <sys/socket.h>
#include <netdb.h>

#include <cstring>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>
#include <cinttypes>

#include <pthread.h>
//#include <semaphore.h>
// maybe we'll need semaphores?

// instead of these macros, read the configs from some config file
#define APP_PORT    "3490"
#define BACKLOG     10

#include "addr_struct.h"
#include "connection_thread.h"

int main() {
    /* Whew. That’s a bit to absorb in one chunk.
     * Let’s have an example that binds the socket to the host the program
     * is running on, port 3490 */
    struct addrinfo *res;

    /* create a hints object that specifies that
     * we want our ip format to be unspecified, (`AF_UNSPEC`)
     * we want a TCP stream (`SOCK_STREAM`)
     * and that we want getaddrinfo to auto-fill our addr (`AI_PASSIVE`) */
    class addrinfo_t hints{AF_UNSPEC, SOCK_STREAM, AI_PASSIVE};

    // does specifying name: NULL default to localhost?
    int success;
    if ((success = getaddrinfo(nullptr, APP_PORT, &hints, &res)) != 0) {
        std::cerr << "getaddrinfo failed: " << gai_strerror(success) << '\n';
        return EXIT_FAILURE;
    }

    // create a socket and bind a port to said socket
    sock_fd sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int bind_failure = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (bind_failure) {
        std::cerr << "Failed to bind!\n";
        return EXIT_FAILURE;
    }

    /* just so we can both keep track of our threads
     * and have a simple LIFO */
    //std::vector<pthread_t *> thread_lifo;

    // we're initializing a mutex to use with Thread::Communicate io
    pthread_mutex_init(&Thread::io_mutex, nullptr);

    /* Instead of the while loop, if we want concurrent communication with
     * the program, we probably need to split this process to threads */

    // instead of dying after `max_connections` times, maybe die after
    // someone hasn't connected after some period of time
    uint8_t max_connections = 16;
    while (max_connections--) {
        listen(sockfd, BACKLOG);
        // this gets initialized during accept(), no need to
        // initialize it here
        struct sockaddr_storage their_addr;
        socklen_t addr_size{sizeof their_addr};

        // accept waits for a connect() to be made, so
        // anything after this line happens after a connect has been made
        sock_fd new_fd = accept(sockfd,
                                reinterpret_cast<struct sockaddr*>(&their_addr),
                                &addr_size);

        std::cout << "New thread [" << new_fd << "] connected.\n";

        pthread_t thd;
        if (pthread_create(&thd, nullptr, Thread::Communicate, &new_fd)) {
            std::cerr << "Thread creation failed!\n";
            // DON'T CONTINUE; THE NETWORK SOCKET STILL EXISTS!
            continue;
        }
        //thread_lifo.push_back(ptr);
    }

    freeaddrinfo(res);
    return EXIT_SUCCESS;
}