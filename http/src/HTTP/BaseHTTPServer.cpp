#include "HTTP/BaseHTTPServer.h"

namespace HW {

    namespace HTTP {

        BaseHTTPServer::BaseHTTPServer()
	    : m_callback{nullptr}, m_client_events{EPOLLRDHUP} {}

	    BaseHTTPServer::BaseHTTPServer(const Callback callback)
	    : m_callback{callback}, m_client_events{EPOLLRDHUP} {}

	    BaseHTTPServer::~BaseHTTPServer() {
	    	try {
		    	close();
		    }
		    catch (...) {}
	    }

	    void BaseHTTPServer::initEpoll() {
		    if (m_epollfd.isOpened()) {
		    	return;
		    }
		    int efd = epoll_create(1);
		    if (efd < 0) {
		    	throw HW::DescriptorError("Error creating epoll!");
		    }
		    m_epollfd.setFD(efd);
	    }

	    void BaseHTTPServer::setCallback(Callback callback) {
	    	m_callback = callback;
	    }

	    void BaseHTTPServer::open(const std::string &ip, const uint16_t port) {
	    	m_socket.open();
	    	m_socket.bind(ip, port);
	    	initEpoll();
	    	addToEpoll(m_socket.getFD(), EPOLLIN);
	    }

	    void BaseHTTPServer::listen(const int queue_size) {
	    	if (isOpened()) {
		    	if (::listen(m_socket.getFD(), queue_size) < 0) {
		    		throw HW::NetworkError("Error setting socket as listening!");
		    	}
		    }
	    }

	    void BaseHTTPServer::accept() {
		    if (!isOpened()) {
		    	throw HW::NetworkError("Server closed!");
		    }
		    sockaddr_in client_addr{};
		    socklen_t client_addr_size = sizeof(client_addr);

		    int client_fd = ::accept4(m_socket.getFD(), 
			    reinterpret_cast<sockaddr*>(&client_addr), 
			    &client_addr_size, 
			    SOCK_NONBLOCK);

		    if (client_fd < 0) {
		    	throw HW::NetworkError("Error accepting connection!");
		    }
    		std::string client_ip(inet_ntoa(client_addr.sin_addr));
    		uint16_t client_port = ntohs(client_addr.sin_port);
		
	    	bool success;
    		std::tie(std::ignore, success) = m_connections.try_emplace(client_fd, ConnectionAsync(client_fd));
	    	if (!success) {
		    	throw HW::NetworkError("Error accepting connection!");
    		}
	    	addToEpoll(client_fd, m_client_events);
    		m_connections[client_fd].setRecieveTimeout(5);
	    	m_connections[client_fd].setSendTimeout(5);
    	}

    	bool BaseHTTPServer::isOpened() const {
	    	return m_socket.isOpened();
	    }

	    void BaseHTTPServer::close() {
    		for (auto& c : m_connections) {
    			removeFromEpoll(c.first);
    		}
    		m_connections.clear();
    		removeFromEpoll(m_socket.getFD());
    		m_socket.close();
    		m_epollfd.close();
    	}

    	Address BaseHTTPServer::getServerAddress() const {
    		sockaddr_in src_addr{};
    		socklen_t src_addr_size = sizeof(src_addr);
    		getsockname(m_socket.getFD(), reinterpret_cast<sockaddr*>(&src_addr), &src_addr_size);
    		std::string src_ip(inet_ntoa(src_addr.sin_addr));
    		uint16_t src_port = ntohs(src_addr.sin_port);
    		return std::make_pair(src_ip, src_port);
    	}

    	void BaseHTTPServer::setMaxConnect(const int new_max) {
    		listen(new_max);
    	}

    	void BaseHTTPServer::addToEpoll(int fd, uint32_t events) {
    		if (!m_epollfd.isOpened()) {
    			return;
    		}
    		epoll_event e{};
    		e.events = events;
    		e.data.fd = fd;
    		if (epoll_ctl(m_epollfd.getFD(), EPOLL_CTL_ADD, e.data.fd, &e) < 0) {
    			throw HW::DescriptorError("Error adding connection to epoll!");
    		}
    	}

    	void BaseHTTPServer::removeFromEpoll(int fd) {
    		if (!m_epollfd.isOpened()) {
    			return;
    		}
    		epoll_event e{};
    		epoll_ctl(m_epollfd.getFD(), EPOLL_CTL_DEL, fd, &e);
    	}

    	void BaseHTTPServer::handleConnection(ConnectionAsync &c, uint32_t events) {
    		int fd = c.getFD();
    		c.setEvent(events);
    		if (m_callback) {
    			m_callback(c);
    		}
    		if (!c.isOpened() || c.isEventSet(EPOLLERR)) {
    			removeFromEpoll(fd);
    			m_connections.erase(fd);
    		}
    		else {
    			c.clearEvents();
    		}
    	}

    	void BaseHTTPServer::setClientEvents(uint32_t events) {
    		m_client_events = events;
    	}

    } // HTTP

} // HW