#ifndef HW_CONNECTION_H
#define HW_CONNECTION_H

#include "Socket.hpp"
#include <string>

namespace HW {

    using Address = std::pair<std::string, uint16_t>;
    using Buffer = std::vector<uint8_t>;

    class Connection {
    private:
        Socket m_socket;
        Buffer      m_buffer;
        bool        m_opened;

        Connection(const Connection &rhs) = delete;
        Connection &operator=(const Connection &rhs) = delete;

        void setTimeout(int seconds, int optname);

        friend class Server;

    public:
        Connection();
        Connection(const int fd);
        Connection(Connection &&rhs) noexcept = default;
        int getFD() const;
        size_t read(void *data, size_t size);
        void readExact(void *data, size_t size);
        size_t readToBuffer(size_t size);
        size_t write(const void *data, size_t size);
        void writeExact(const void *data, size_t size);
        void close();
        bool isOpened() const;
        void setRecieveTimeout(int seconds);
        void setSendTimeout(int seconds);
        void connect(const std::string &ip, const uint16_t port);
        Buffer &getBuffer();
        void clearBuffer();

        Address getDstAddr() const;
        Address getSrcAddr() const;
    };

} // HW

#endif // HW_CONNECTION_H