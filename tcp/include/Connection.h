#ifndef HW_CONNECTION_H
#define HW_CONNECTION_H

#include "Socket.h"
#include <string>

namespace HW {

    using Address = std::pair<std::string, uint16_t>;

    class Connection {
    private:
        Socket  m_socket;
        Address m_dst_addr; // client
        Address m_src_addr; // server

        Connection(const Connection &rhs) = delete;
        Connection &operator=(const Connection &rhs) = delete;

        // Используется Server::accept
        Connection(const int fd, Address &dst, Address &src);

        void setTimeout(int seconds, int optname);

        friend class Server;

    public:
        Connection();
        Connection(Connection &&rhs) noexcept = default;
        size_t read(void *data, size_t size);
        void readExact(void *data, size_t size);
        size_t write(const void *data, size_t size);
        void writeExact(const void *data, size_t size);
        void close();
        bool isOpened() const;
        void setRecieveTimeout(int seconds);
        void setSendTimeout(int seconds);
        void connect(const std::string &ip, const uint16_t port);

        Address getDstAddr() const;
        Address getSrcAddr() const;
    };

} // HW

#endif // HW_CONNECTION_H