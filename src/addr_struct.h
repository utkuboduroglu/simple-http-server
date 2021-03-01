#ifndef SIMPLEHTTP_ADDR_STRUCT_H
#define SIMPLEHTTP_ADDR_STRUCT_H

#include <cstring>
#include <netdb.h>

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
