#ifndef HW_SERVER_H
#define HW_SERVER_H

#include "Socket.hpp"
#include "Connection.hpp"

namespace HW {

    class Server {
    private:
        Socket  m_socket;
        Address m_addr;

        Server(const Server &rhs) = delete;
        Server &operator=(const Server &rhs) = delete;

    public:
        Server(const std::string &ip, const uint16_t port);
        Server(Server &&rhs) noexcept = default;
        void open();
        void listen(const int queue_size);
        Connection accept();
        bool isOpened() const;
        void close();
        void set_max_connect(const int new_max);
    };

} // HW

#endif // HW_SERVER_H