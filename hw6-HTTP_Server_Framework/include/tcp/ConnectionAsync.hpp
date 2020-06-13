#ifndef HW_CONNECTION_ASYNC_H
#define HW_CONNECTION_ASYNC_H

#include "tcp/SocketAsync.hpp"
#include <string>
#include <vector>

namespace HW {

    using Address =   std::pair<std::string, uint16_t>;
    using Buffer =    std::vector<uint8_t>;

    class ConnectionAsync {
    private:
        SocketAsync m_socket;
        Buffer      m_buffer;
        uint32_t    m_events;
        bool        m_opened;

        ConnectionAsync(const ConnectionAsync &rhs) = delete;
        ConnectionAsync &operator=(const ConnectionAsync &rhs) = delete;

        void setTimeout(int seconds, int optname);

        friend class ServerAsync;

    public:
        ConnectionAsync();
        ConnectionAsync(const int fd);
        ConnectionAsync(ConnectionAsync &&rhs) noexcept = default;
        int getFD() const;
        size_t read(void *data, size_t size);
        size_t readToBuffer(size_t size);
        size_t write(const void *data, size_t size);
        void close();
        bool isOpened() const;
        void setRecieveTimeout(int seconds);
        void setSendTimeout(int seconds);
        void connect(const std::string &ip, const uint16_t port);
        Buffer &getBuffer();
        void clearBuffer();
        void setEvent(const uint32_t epoll_event);
        bool isEventSet(const uint32_t epoll_event);
        void clearEvents();

        Address getDstAddr() const;
        Address getSrcAddr() const;
    };

} // HW

#endif // HW_CONNECTION_ASYNC_H