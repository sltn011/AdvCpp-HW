#ifndef HW_SOCKET_H
#define HW_SOCKET_H

#include <sys/socket.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include "exceptions/Exceptions.hpp"

namespace HW {

    class Socket {
    private:
        int m_fd;

        Socket(int valid_sfd);
        Socket(const Socket &rhs) = delete;
        Socket &operator=(const Socket &rhs) = delete;

        friend class Connection;
        friend class ConnectionAsync;

    public:
        Socket();
        Socket(Socket &&rhs) noexcept;
        ~Socket() noexcept;
        void open();
        void bind(const std::string &ip, const uint16_t port);
        int getFD() const;
        bool isOpened() const;
        void close();
    };

} // HW

#endif // HW_SOCKET_H