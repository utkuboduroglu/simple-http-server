#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <cstring>
#include <unistd.h>

#include <iostream>
#include <iterator>
#include <string>
#include <cinttypes>

/* the headers below do not
 * exist in windows machines */
#include <arpa/inet.h>
#include <netinet/in.h>

// custom type for socket file descriptors
using sock_fd = int;

#define APP_PORT    "3490"
#define BACKLOG     10

int main() {
    /* Whew. That’s a bit to absorb in one chunk.
     * Let’s have an example that binds the socket to the host the program
     * is running on, port 3490 */
    struct addrinfo hints, *res;

    /* create a hints object that specifies that
     * we want our ip format to be unspecified, (`AF_UNSPEC`)
     * we want a TCP stream (`SOCK_STREAM`)
     * and that we want getaddrinfo to auto-fill our addr (`AI_PASSIVE`) */
    std::memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;

    // check whether
    int success;
    if ((success = getaddrinfo(NULL, APP_PORT, &hints, &res)) != 0) {
        std::cerr << "getaddrinfo failed: " << gai_strerror(success) << '\n';
        return EXIT_FAILURE;
    }

    // create a socket and bind a port to said socket
    sock_fd sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    listen(sockfd, BACKLOG);

    struct sockaddr_storage their_addr;
    socklen_t addr_size{sizeof their_addr};
    sock_fd new_fd = accept(sockfd,
                            reinterpret_cast<struct sockaddr*>(&their_addr),
                            &addr_size);

    size_t MAX_BUFFER_SIZE = 128;
    void* buffer = malloc(MAX_BUFFER_SIZE);

    size_t bytes_recv = recv(new_fd, buffer, MAX_BUFFER_SIZE, 0);
    while (bytes_recv > 0) {
        std::cout << (const char*) buffer;
        bytes_recv = recv(new_fd, buffer, MAX_BUFFER_SIZE, 0);
    }

    std::cout << "Connection closed.\n";
    close(new_fd);
    free(buffer);

    freeaddrinfo(res);
    return EXIT_SUCCESS;
}