#include "tcp/ConnectionAsync.h"

namespace HW {

	ConnectionAsync::ConnectionAsync() : m_opened{false} {}

	ConnectionAsync::ConnectionAsync(const int fd) : m_socket{fd}, m_opened{true} {}

	int ConnectionAsync::getFD() const {
		return m_socket.getFD();
	}

	size_t ConnectionAsync::read(void *data, size_t size) {
		if (!isOpened()) {
			throw HW::DescriptorError("Connection is closed!");
		}
		ssize_t recieved = ::read(m_socket.getFD(), data, size);
		if (recieved < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				return 0;
			}
			throw HW::IOError("Error reading from process!");
		}
		else if (recieved == 0) {
			m_opened = false;
		}
		return static_cast<size_t>(recieved);
	}

	size_t ConnectionAsync::readToBuffer(size_t size) {
		if (!isOpened()) {
			throw HW::DescriptorError("Connection is closed!");
		}
		size_t old_size = m_buffer.size();
		m_buffer.resize(old_size + size);
		size_t recieved = read(m_buffer.data() + old_size, size);
		m_buffer.resize(old_size + recieved);
		return recieved;
	}

	size_t ConnectionAsync::write(const void *data, size_t size) {
		if (!isOpened()) {
			throw HW::DescriptorError("Connection is closed!");
		}
		ssize_t written = ::write(m_socket.getFD(), data, size);
		if (written < 0) {
			throw HW::IOError("Error writing to process!");
		}
		else {
			return static_cast<size_t>(written);
		}
	}

	void ConnectionAsync::close() {
		if (!isOpened()) {
			return;
		}
		m_socket.close();
		m_opened = false;
	}

	bool ConnectionAsync::isOpened() const {
		return m_opened;
	}

	void ConnectionAsync::setTimeout(int seconds, int optname) {
		if (!m_socket.isOpened()) {
			return;
		}
		timeval timeout{ seconds, 0 };
		if (setsockopt(m_socket.getFD(),
			SOL_SOCKET,
			optname,
			&timeout,
			sizeof(timeout)) < 0)
		{
			throw HW::NetworkError("Error setting timeout!");
		}
	}

	void ConnectionAsync::setRecieveTimeout(int seconds) {
		setTimeout(seconds, SO_RCVTIMEO);
	}

	void ConnectionAsync::setSendTimeout(int seconds) {
		setTimeout(seconds, SO_SNDTIMEO);
	}

	void ConnectionAsync::connect(const std::string &ip, const uint16_t port) {
		if (isOpened()) {
			throw HW::NetworkError("Already has opened connection!");
		}

		m_socket.open();

		sockaddr_in sock_addr{};
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(port);
		if (inet_aton(ip.c_str(), &sock_addr.sin_addr) == 0) {
			throw HW::IOError("Error parsing address!");
		}
		if (::connect(m_socket.getFD(), reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr)) < 0) {
			throw HW::NetworkError("Error connecting to " + ip + " " + std::to_string(port));
		}
		m_opened = true;
	}

	Buffer &ConnectionAsync::getBuffer() {
		return m_buffer;
	}

	void ConnectionAsync::clearBuffer() {
		m_buffer.clear();
	}
		
	void ConnectionAsync::setEvent(const uint32_t epoll_event) {
		m_events |= epoll_event;
	}

	bool ConnectionAsync::isEventSet(const uint32_t epoll_event) {
		return m_events & epoll_event;
	}

	void ConnectionAsync::clearEvents() {
		m_events = static_cast<uint32_t>(0);
	}

	Address ConnectionAsync::getDstAddr() const {
		sockaddr_in dst_addr{};
		socklen_t dst_addr_size = sizeof(dst_addr);
		getsockname(m_socket.getFD(), reinterpret_cast<sockaddr*>(&dst_addr), &dst_addr_size);
		std::string dst_ip(inet_ntoa(dst_addr.sin_addr));
		uint16_t dst_port = ntohs(dst_addr.sin_port);
		return std::make_pair(dst_ip, dst_port);
	}

	Address ConnectionAsync::getSrcAddr() const {
		sockaddr_in src_addr{};
		socklen_t src_addr_size = sizeof(src_addr);
		getpeername(m_socket.getFD(), reinterpret_cast<sockaddr*>(&src_addr), &src_addr_size);
		std::string src_ip(inet_ntoa(src_addr.sin_addr));
		uint16_t src_port = ntohs(src_addr.sin_port);
		return std::make_pair(src_ip, src_port);
	}

} // HW