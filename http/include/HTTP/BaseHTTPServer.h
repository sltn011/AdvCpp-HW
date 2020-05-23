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

namespace HW::HTTP {

    using Callback = std::function<void(ConnectionAsync &c)>;
    using Connections = std::unordered_map<int, ConnectionAsync>;
	using Time_t = std::chrono::system_clock::time_point;
	using Timeout = std::chrono::seconds;

	using ConnTime = std::pair<Time_t, int>;

	struct cmp {
		bool operator()(const ConnTime &a, const ConnTime &b) const;
	};

	using IdleTime = std::set<ConnTime, cmp>;

    class BaseHTTPServer {
    protected:
    	SocketAsync			m_socket;
    	Callback			m_callback;
    	Descriptor			m_epollfd;
    	Connections			m_connections;
    	uint32_t			m_clientEvents;
		IdleTime			m_idleTime;
		Timeout				m_timeout;
		std::atomic_bool	m_shutdown;

    	static const size_t m_epollsize = 100;
    	BaseHTTPServer(const BaseHTTPServer &rhs) = delete;
    	BaseHTTPServer &operator=(const BaseHTTPServer &rhs) = delete;
    	void initEpoll();
    	void addToEpoll(int fd, uint32_t events);
    	void removeFromEpoll(int fd);
    	void handleConnection(ConnectionAsync &c, uint32_t events);

    public:
    	BaseHTTPServer();
    	BaseHTTPServer(const Callback callback, const Timeout timeout);
    	virtual ~BaseHTTPServer();
    	BaseHTTPServer(BaseHTTPServer &&rhs) noexcept = default;
    	void setCallback(Callback callback);
    	void open(const std::string &ip, const uint16_t port);
    	void listen(const int queue_size);
    	bool isOpened() const;
    	void close();
		void shutdown();
    	Address getServerAddress() const;
    	void setMaxConnect(const int new_max);
		void setTimeout(const Timeout timeout);
    	void accept();
    	void setClientEvents(uint32_t events);
    	void run(int epollTimeout);
    };

} // HW::HTTP

#endif // HW_HTTP_BASESERVER_H