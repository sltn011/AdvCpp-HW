#ifndef HW_HTTP_BASESERVER_H
#define HW_HTTP_BASESERVER_H

#include "tcp/SocketAsync.h"
#include "tcp/ConnectionAsync.h"
#include "data/Descriptor.h"
#include <sys/epoll.h>
#include <functional>
#include <unordered_map>

namespace HW::HTTP {

    using Callback = std::function<void(ConnectionAsync &c)>;
    using Connections = std::unordered_map<int, ConnectionAsync>;

    class BaseHTTPServer {
    protected:
    	SocketAsync			m_socket;
    	Callback			m_callback;
    	Descriptor			m_epollfd;
    	Connections			m_connections;
    	uint32_t			m_client_events;

    	static const size_t m_epollsize = 100;
    	BaseHTTPServer(const BaseHTTPServer &rhs) = delete;
    	BaseHTTPServer &operator=(const BaseHTTPServer &rhs) = delete;
    	void initEpoll();
    	void addToEpoll(int fd, uint32_t events);
    	void removeFromEpoll(int fd);
    	void handleConnection(ConnectionAsync &c, uint32_t events);

    public:
    	BaseHTTPServer();
    	BaseHTTPServer(const Callback callback);
    	virtual ~BaseHTTPServer();
    	BaseHTTPServer(BaseHTTPServer &&rhs) noexcept = default;
    	void setCallback(Callback callback);
    	void open(const std::string &ip, const uint16_t port);
    	void listen(const int queue_size);
    	bool isOpened() const;
    	void close();
    	Address getServerAddress() const;
    	void setMaxConnect(const int new_max);
    	void accept();
    	void setClientEvents(uint32_t events);
    	virtual void run(int epollTimeout) = 0;
    };

} // HW::HTTP

#endif // HW_HTTP_BASESERVER_H