#include "ServerAsync.h"

namespace HW { 

	ServerAsync::ServerAsync()
	: m_callback{nullptr}, m_client_events{EPOLLRDHUP} {}

	ServerAsync::ServerAsync(const Callback callback)
	: m_callback{callback}, m_client_events{EPOLLRDHUP} {}

	ServerAsync::~ServerAsync() {
		try {
			close();
		}
		catch (...) {}
	}

	void ServerAsync::initEpoll() {
		if (m_epollfd.isOpened()) {
			return;
		}
		int efd = epoll_create(1);
		if (efd < 0) {
			throw HW::DescriptorError("Error creating epoll!");
		}
		m_epollfd.setFD(efd);
	}

	void ServerAsync::setCallback(Callback callback) {
		m_callback = callback;
	}

	void ServerAsync::open(const std::string &ip, const uint16_t port) {
		m_socket.open();
		m_socket.bind(ip, port);
		initEpoll();
		addToEpoll(m_socket.getFD(), EPOLLIN);
	}

	void ServerAsync::listen(const int queue_size) {
		if (isOpened()) {
			if (::listen(m_socket.getFD(), queue_size) < 0) {
				throw HW::NetworkError("Error setting socket as listening!");
			}
		}
	}

	void ServerAsync::accept() {
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
		std::tie(std::ignore, success) = m_connections.emplace(client_fd, ConnectionAsync(client_fd));
		if (!success) {
			throw HW::NetworkError("Error accepting connection!");
		}
		addToEpoll(client_fd, m_client_events);
	}

	bool ServerAsync::isOpened() const {
		return m_socket.isOpened();
	}

	void ServerAsync::close() {
		for (auto& c : m_connections) {
			removeFromEpoll(c.first);
		}
		removeFromEpoll(m_socket.getFD());
		m_socket.close();
		m_epollfd.close();
	}

	Address ServerAsync::getServerAddress() const {
		sockaddr_in src_addr{};
		socklen_t src_addr_size = sizeof(src_addr);
		getsockname(m_socket.getFD(), reinterpret_cast<sockaddr*>(&src_addr), &src_addr_size);
		std::string src_ip(inet_ntoa(src_addr.sin_addr));
		uint16_t src_port = ntohs(src_addr.sin_port);
		return std::make_pair(src_ip, src_port);
	}

	void ServerAsync::setMaxConnect(const int new_max) {
		listen(new_max);
	}

	void ServerAsync::addToEpoll(int fd, uint32_t events) {
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

	void ServerAsync::removeFromEpoll(int fd) {
		if (!m_epollfd.isOpened()) {
			return;
		}
		epoll_event e{};
		epoll_ctl(m_epollfd.getFD(), EPOLL_CTL_DEL, fd, &e);
	}

	void ServerAsync::handleConnection(ConnectionAsync &c, uint32_t events) {
		int fd = c.getFD();
		c.setEvent(events);
		if (m_callback) {
			m_callback(c);
			std::cout << "After callback!" << std::endl;
		}
		if (!c.isOpened() || c.isEventSet(EPOLLRDHUP) || c.isEventSet(EPOLLERR) || c.isEventSet(EPOLLHUP)) {
			removeFromEpoll(fd);
			m_connections.erase(fd);
		}
		else {
			c.clearEvents();
		}
	}

	void ServerAsync::setClientEvents(uint32_t events) {
		m_client_events = events;
	}

	void ServerAsync::run() {
		std::array<epoll_event, m_epollsize> events;
		while(true) {
			if (!isOpened() || !m_epollfd.isOpened()) {
				break;
			}
			int numFDs = epoll_wait(m_epollfd.getFD(), events.data(), m_epollsize, -1);
			if (numFDs < 0) {
				if (errno == EINTR) {
					continue;
				}
				else {
					throw HW::DescriptorError("Error getting descriptors from epoll!");
				}
			}
			for (int i = 0; i < numFDs; ++i) {
				int fd = events.at(i).data.fd;
				uint32_t connectionEvents = events.at(i).events;
				if (fd == m_socket.getFD()) {
					accept();
				}
				else {
					handleConnection(m_connections[fd], connectionEvents);
				}
			}
		}
	}

} // HW
