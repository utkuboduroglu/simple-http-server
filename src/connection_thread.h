#ifndef SIMPLEHTTP_CONNECTION_THREAD_H
#define SIMPLEHTTP_CONNECTION_THREAD_H

#include <sys/socket.h>
#include <pthread.h>
#include <cstdlib>
#include <cinttypes>

#include <iostream>
#include "addr_struct.h"

std::uint64_t MAX_RECV_BUFFER_SIZE = 128;

namespace Thread {
    pthread_mutex_t io_mutex;
    /* we specify void* for type here, but we request
     * that data actually be of type sock_fd* */
    void* Communicate(void* data) {
        auto new_fd = *reinterpret_cast<sock_fd*>(data);
        void* buffer = std::malloc(MAX_RECV_BUFFER_SIZE);
        std::memset(buffer, 0, MAX_RECV_BUFFER_SIZE);

        size_t bytes_recv = recv(new_fd, buffer, MAX_RECV_BUFFER_SIZE, 0);
        while (bytes_recv > 0) {
            // we lock console i/o to prevent race conditions
            pthread_mutex_lock(&io_mutex);
            std::cout << '[' << new_fd << "] "
                      << (const char*) buffer << std::flush;
            pthread_mutex_unlock(&io_mutex);

            // let's send out the message we received
            std::string out_msg{"You said: "};
            out_msg.append(reinterpret_cast<const char*>(buffer));

            size_t bytes_sent = send(new_fd, out_msg.c_str(), out_msg.size(), 0);
            if (bytes_sent != out_msg.size()) {
                std::cerr << "We sent out " <<
                          bytes_sent << ", expected "
                          << out_msg.size() << '\n';
            }

            // zero the buffer before reading anything else
            std::memset(buffer, 0, MAX_RECV_BUFFER_SIZE);
            bytes_recv = recv(new_fd, buffer, MAX_RECV_BUFFER_SIZE, 0);
        }

        std::cout << "Connection [" << new_fd << "] closed.\n";
        close(new_fd);
        std::free(buffer);
        // we return the thread after
        // the connection terminated
        return data;
    }
}

#endif //SIMPLEHTTP_CONNECTION_THREAD_H
