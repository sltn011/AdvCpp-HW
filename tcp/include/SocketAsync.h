#ifndef HW_SOCKET_ASYNC_H
#define HW_SOCKET_ASYNC_H

#include <sys/socket.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include "Exceptions.h"

namespace HW {

    class SocketAsync {
    private:
        int m_fd;

        SocketAsync(int valid_sfd);
        SocketAsync(const SocketAsync &rhs) = delete;
        SocketAsync &operator=(const SocketAsync &rhs) = delete;

        friend class Connection;
        friend class ConnectionAsync;

    public:
        SocketAsync();
        SocketAsync(SocketAsync &&rhs) noexcept;
        ~SocketAsync() noexcept;
        void open();
        void bind(const std::string &ip, const uint16_t port);
        int getFD() const;
        bool isOpened() const;
        void close();
    };

} // HW

#endif // HW_SOCKET_ASYNC_H