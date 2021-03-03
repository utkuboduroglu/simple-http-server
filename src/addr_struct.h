#ifndef SIMPLEHTTP_ADDR_STRUCT_H
#define SIMPLEHTTP_ADDR_STRUCT_H

#include <cstring>
#include <netdb.h>

// custom type for socket file descriptors
using sock_fd = int;

struct recv_pack {
    sock_fd their_fd;
    struct sockaddr_storage their_addr;
};

class addrinfo_t : public addrinfo {
public:
    addrinfo_t(int family, int socktype, int flags) {
        std::memset(this, 0, sizeof *this);
        this->ai_family     = family;
        this->ai_socktype   = socktype;
        this->ai_flags      = flags;
    }
};

#endif //SIMPLEHTTP_ADDR_STRUCT_H
