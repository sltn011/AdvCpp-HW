#ifndef HW_SERVER_H
#define HW_SERVER_H

#include "Socket.h"
#include "Connection.h"
#include <vector>
#include <memory>


namespace HW {

    class Server {
    private:
        Socket      m_socket;
        std::string m_server_ip;
        uint16_t    m_server_port;
        bool        m_opened;

    public:
        Server(const std::string & ip, const uint16_t port);
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