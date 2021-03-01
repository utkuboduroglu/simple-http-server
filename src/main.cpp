#include <sys/socket.h>
#include <netdb.h>

#include <cstring>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cinttypes>

// custom type for socket file descriptors
using sock_fd = int;

// instead of these macros, read the configs from some config file
#define APP_PORT    "3490"
#define BACKLOG     10
#define MAX_RECV_BUFFER_SIZE 128

#include "addr_struct.h"

int main() {
    /* Whew. That’s a bit to absorb in one chunk.
     * Let’s have an example that binds the socket to the host the program
     * is running on, port 3490 */
    struct addrinfo *res;

    /* create a hints object that specifies that
     * we want our ip format to be unspecified, (`AF_UNSPEC`)
     * we want a TCP stream (`SOCK_STREAM`)
     * and that we want getaddrinfo to auto-fill our addr (`AI_PASSIVE`) */
    struct addrinfo_t hints{AF_UNSPEC, SOCK_STREAM, AI_PASSIVE};

    // does specifying name: NULL default to localhost?
    int success;
    if ((success = getaddrinfo(NULL, APP_PORT, &hints, &res)) != 0) {
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

    /* Instead of the while loop, if we want concurrent communication with
     * the program, we probably need to split this process to threads */

    // this is just so that the program stops after max_connections times
    uint8_t max_connections = 16;
    while (max_connections--) {
        listen(sockfd, BACKLOG);
        struct sockaddr_storage their_addr;
        socklen_t addr_size{sizeof their_addr};
        sock_fd new_fd = accept(sockfd,
                                reinterpret_cast<struct sockaddr*>(&their_addr),
                                &addr_size);

        void* buffer = malloc(MAX_RECV_BUFFER_SIZE);
        std::memset(buffer, 0, MAX_RECV_BUFFER_SIZE);

        size_t bytes_recv = recv(new_fd, buffer, MAX_RECV_BUFFER_SIZE, 0);
        while (bytes_recv > 0) {
            std::cout << '[' << new_fd << "] "
                      << (const char*) buffer;

            // let's send out the message we received
            std::string out_msg{"You said: "};
            out_msg.append(reinterpret_cast<const char*>(buffer));

            size_t bytes_sent = send(new_fd, out_msg.c_str(), out_msg.size(), 0);
            if (bytes_sent != out_msg.size()) {
                std::cerr << "We sent out " <<
                          bytes_sent << ", expected "
                          << out_msg.size() << '\n';
            }

            bytes_recv = recv(new_fd, buffer, MAX_RECV_BUFFER_SIZE, 0);
        }

        std::cout << "Connection closed.\n";
        close(new_fd);
        free(buffer);
    }

    freeaddrinfo(res);
    return EXIT_SUCCESS;
}