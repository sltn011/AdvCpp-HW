#ifndef HW_SERVER_ASYNC_H
#define HW_SERVER_ASYNC_H

#include "SocketAsync.hpp"
#include "ConnectionAsync.hpp"
#include "Descriptor.hpp"
#include <sys/epoll.h>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <mutex>

namespace HW {

    using Callback = std::function<void(ConnectionAsync &c)>;
    using Connections = std::unordered_map<int, ConnectionAsync>;

    class ServerAsync {
    private:
        SocketAsync         m_socket;
        Callback            m_callback;
        Descriptor          m_epollfd;
        Connections         m_connections;
        uint32_t            m_client_events;
        std::atomic_bool    m_shutdown;
        std::mutex          m_mutex;
        
        static const size_t m_epollsize = 100;

        ServerAsync(const ServerAsync &rhs) = delete;
        ServerAsync &operator=(const ServerAsync &rhs) = delete;
        void initEpoll();
        void addToEpoll(int fd, uint32_t events);
        void removeFromEpoll(int fd);
        void handleConnection(ConnectionAsync &c, uint32_t events);

    public:
        ServerAsync();
        ServerAsync(const Callback callback);
        ~ServerAsync();
        ServerAsync(ServerAsync &&rhs) noexcept = default;
        void setCallback(Callback callback);
        void open(const std::string &ip, const uint16_t port);
        void listen(const int queue_size);
        bool isOpened() const;
        void close();
        Address getServerAddress() const;
        void setMaxConnect(const int new_max);
        void accept();
        void setClientEvents(uint32_t events);
        
        void run(int epollTimeout);
    };

} // HW

#endif // HW_SERVER_ASYNC_H