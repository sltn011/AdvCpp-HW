#ifndef HW_CONNECTION_H
#define HW_CONNECTION_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Socket.h"
#include <string>
#include <vector>


namespace HW {

    class Connection {
    private:
        Socket      m_socket;
        std::string m_dst_addr;
        std::string m_src_addr; // server
        uint16_t    m_dst_port;
        uint16_t    m_src_port;
        bool        m_opened;

        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;

        // Используется Server::accept
        Connection(const int fd, std::pair<std::string, uint16_t> &dst, std::pair<std::string, uint16_t> &src);

        friend class Server;

    public:
        Connection();
        Connection(const std::string & ip, const uint16_t port);
        Connection(Connection &&rhs) noexcept = default;
        size_t read(void* data, size_t size);
        void readExact(void* data, size_t size);
        size_t write(const void* data, size_t size);
        void writeExact(const void* data, size_t size);
        void close();
        bool isOpened() const;
        void set_timeout(int seconds);
        bool connect(const std::string & addr, const uint16_t port);
        void setDstAddr(const std::string &addr);
        void setDstPort(const uint16_t port);

        std::pair<std::string, uint16_t> getDstData() const;
        std::pair<std::string, uint16_t> getSrcData() const;
    };

} // HW

#endif // HW_CONNECTION_H