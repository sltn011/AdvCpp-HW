#include "Connection.h"

namespace HW {

    Connection::Connection() : m_opened{false} {}

    Connection::Connection(const std::string & ip, const uint16_t port)
    : m_dst_addr(ip), m_dst_port(port), m_opened{false} {}

    Connection::Connection(const int fd, std::pair<std::string, uint16_t> &dst, std::pair<std::string, uint16_t> &src)
    : m_socket{fd}, m_dst_addr{dst.first}, m_src_addr{src.first}, 
    m_dst_port{dst.second}, m_src_port{src.second}, m_opened{true} {}

    size_t Connection::read(void* data, size_t size) {
        if (!isOpened()) {
			throw HW::DescriptorError("Connection is closed!");
		}
        ssize_t recieved = ::read(m_socket.getFD(), data, size);
		if (recieved < 0) {
			throw HW::IOError("Error reading from process!");
		}
		else {
			return static_cast<size_t>(recieved);
		}
    }

    void Connection::readExact(void* data, size_t size) {
        if (!isOpened()) {
			throw HW::DescriptorError("Connection is closed!");
		}
        size_t total_read = 0;
		while (total_read != size) {
			size_t recieved = read(static_cast<uint8_t*>(data) + total_read, size - total_read);
			if (recieved == 0) {
				throw HW::IOError("End of file reached before reading required number of bytes!");
			}
			total_read += recieved;
		}
    }

    size_t Connection::write(const void* data, size_t size) {
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

    void Connection::writeExact(const void* data, size_t size) {
        if (!isOpened()) {
			throw HW::DescriptorError("Connection is closed!");
		}
        size_t total_written = 0;
		while (total_written != size) {
			total_written += write(static_cast<const uint8_t*>(data) + total_written, size - total_written);
		}
    }

    void Connection::close() {
        if (!isOpened()) {
            return;
        }
        try {
            m_socket.close();
            m_opened = false;
            m_src_addr.clear();
            m_src_port = 0;
        }
        catch (HW::DescriptorError &e) {
            throw;
        }
    }

    bool Connection::isOpened() const {
        return m_opened;
    }

    void Connection::set_timeout(int seconds) {
        if (!m_socket.isOpened()) {
            return;
        }
        timeval timeout{};
        timeout.tv_sec = seconds;
        timeout.tv_usec = 0;
        if (setsockopt(m_socket.getFD(),
                        SOL_SOCKET,
                        SO_SNDTIMEO,
                        &timeout,
                        sizeof(timeout)) < 0) 
        {
            throw HW::NetworkError("Error setting timeout!");
        }
    }

    bool Connection::connect(const std::string & addr, const uint16_t port) {
        if (isOpened()) {
            return false;
        }
        if (!m_socket.isOpened()) {
            try {
                m_socket.open();
            }
            catch (HW::DescriptorError &e) {
                throw;
            }
        }
        sockaddr_in sock_addr{};
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_port = htons(port);
        if (inet_aton(addr.c_str(), &sock_addr.sin_addr) == 0) {
            throw HW::IOError("Error parsing address!");
        }
        if (::connect(m_socket.getFD(), reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr)) < 0) {
            return false;
        }
        m_src_addr = addr;
        m_src_port = port;
        m_opened = true;
        return true;
    }

    void Connection::setDstAddr(const std::string &addr) {
        m_dst_addr = addr;
    }

    void Connection::setDstPort(const uint16_t port) {
        m_dst_port = port;
    }

    std::pair<std::string, uint16_t> Connection::getDstData() const {
        return std::make_pair(m_dst_addr, m_dst_port);
    }
        
    std::pair<std::string, uint16_t> Connection::getSrcData() const {
        return std::make_pair(m_src_addr, m_src_port);
    }

} // HW