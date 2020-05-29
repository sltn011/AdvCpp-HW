#ifndef HW_HTTP_BASESERVER_H
#define HW_HTTP_BASESERVER_H

#include "tcp/SocketAsync.h"
#include "tcp/ConnectionAsync.h"
#include "data/Descriptor.h"
#include <sys/epoll.h>
#include <thread>
#include "coroutines/coroutine.h"
#include "HTTP/HTTPMessage.h"
#include <map>
#include <set>
#include <chrono>
#include <sys/signal.h>
#include <atomic>
#include <mutex>
#include "log/Log.h"
#include <sstream>

namespace HW::HTTP {

    using Callback = std::function<void(ConnectionAsync &c)>;
    using Connections = std::unordered_map<int, ConnectionAsync>;
    using Threads = std::vector<std::thread>;
    using Coroutines = std::unordered_map<int, Coroutine::routine_t>;
    using Time_t = std::chrono::system_clock::time_point;
    using Timeout = std::chrono::seconds;

    using ConnTime = std::pair<Time_t, int>;

    struct cmp {
        bool operator()(const ConnTime &a, const ConnTime &b) const;
    };

    using IdleTime = std::set<ConnTime, cmp>;

    class BaseHTTPServer {
    protected:
        class ThreadWork;

        SocketAsync                    m_socket;
        std::vector<ThreadWork>        m_thrWorks;
        Threads                        m_threads;
        size_t                         m_num_threads;
        

        static const size_t m_epollsize = 100;
        BaseHTTPServer(const BaseHTTPServer &rhs) = delete;
        BaseHTTPServer &operator=(const BaseHTTPServer &rhs) = delete;

    public:
        BaseHTTPServer(size_t numThreads);
        virtual ~BaseHTTPServer();
        BaseHTTPServer(BaseHTTPServer &&rhs) noexcept = default;
        void setCallback(Callback callback);
        void setTimeout(const Timeout timeout);
        void setClientEvents(uint32_t events);
        void open(const std::string &ip, const uint16_t port);
        void listen(const int queue_size);
        bool isOpened() const;
        void close();
        void shutdown();
        Address getServerAddress() const;
        void setMaxConnectQueue(const int new_max);
        void run(int epoll_timeout);

        virtual HTTPResponse onRequest(const HTTPRequest &r) = 0;

    };

    class BaseHTTPServer::ThreadWork{
    public:
        int                                       m_server_fd;
        Descriptor                                m_epoll;
        Connections                               m_connections;
        std::array<epoll_event, m_epollsize>      m_events;
        Coroutines                                m_coroutines;
        Callback                                  m_callback;
        IdleTime                                  m_idleTime;
        uint32_t                                  m_clientEvents;
        Timeout                                   m_timeout;
        bool                                      m_shutdown;

        void accept();
        void initEpoll();
        void addToEpoll(int fd, uint32_t events);
        void removeFromEpoll(int fd);
        void handleConnection(int fd, uint32_t events);
        void resetConnectionTime(int fd);
        void dropTimeoutedConnections();
        void disconnectClient(int fd);

        ThreadWork(int server_fd, Timeout idle_time);
        ThreadWork(const ThreadWork &rhs) = delete;
        ThreadWork &operator=(const ThreadWork &rhs) = delete;
        ThreadWork(ThreadWork &&rhs) = default;
        ~ThreadWork();

        void run(int epoll_timeout);
    };

    HTTPRequest readRequest(ConnectionAsync &c);
    void writeResponse(ConnectionAsync &c, HTTPResponse &resp);

} // HW::HTTP

#endif // HW_HTTP_BASESERVER_H