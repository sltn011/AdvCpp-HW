#include "tcp/Connection.hpp"

namespace HW {

    Connection::Connection() : m_opened{false} {}

    Connection::Connection(const int fd) : m_socket{fd}, m_opened{true} {}

    int Connection::getFD() const {
        return m_socket.getFD();
    }

    size_t Connection::read(void *data, size_t size) {
        if (!isOpened()) {
            throw HW::DescriptorError("Connection is closed!");
        }
        ssize_t recieved = ::read(m_socket.getFD(), data, size);
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return (recieved > 0) ? recieved : 0;
        }
        if (recieved < 0) {
            throw HW::IOError("Error reading from process!");
        }
        else if (recieved == 0) {
            m_opened = false;
        }
        return static_cast<size_t>(recieved);
    }

    void Connection::readExact(void *data, size_t size) {
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

    size_t Connection::readToBuffer(size_t size) {
        if (!isOpened()) {
            throw HW::DescriptorError("Connection is closed!");
        }
        size_t old_size = m_buffer.size();
        m_buffer.resize(old_size + size);
        size_t recieved = 0;
        try {
            recieved = read(m_buffer.data() + old_size, size);
        }
        catch(BaseException &e) {
            m_buffer.resize(old_size);
            throw;
        }
        m_buffer.resize(old_size + recieved);
        return recieved;
    }

    size_t Connection::write(const void *data, size_t size) {
        if (!isOpened()) {
            throw HW::DescriptorError("Connection is closed!");
        }
        ssize_t written = ::write(m_socket.getFD(), data, size);
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return (written > 0) ? written : 0;
        }
        if (written < 0) {
            throw HW::IOError("Error writing to process!");
        }
        else {
            return static_cast<size_t>(written);
        }
    }

    void Connection::writeExact(const void *data, size_t size) {
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
        m_socket.close();
        m_opened = false;
    }

    bool Connection::isOpened() const {
        return m_opened;
    }

    void Connection::setTimeout(int seconds, int optname) {
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

    void Connection::setRecieveTimeout(int seconds) {
        setTimeout(seconds, SO_RCVTIMEO);
    }

    void Connection::setSendTimeout(int seconds) {
        setTimeout(seconds, SO_SNDTIMEO);
    }

    void Connection::connect(const std::string &ip, const uint16_t port) {
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

    Buffer &Connection::getBuffer() {
        return m_buffer;
    }

    void Connection::clearBuffer() {
        m_buffer.clear();
    }

    Address Connection::getDstAddr() const {
        sockaddr_in dst_addr{};
        socklen_t dst_addr_size = sizeof(dst_addr);
        getsockname(m_socket.getFD(), reinterpret_cast<sockaddr*>(&dst_addr), &dst_addr_size);
        std::string dst_ip(inet_ntoa(dst_addr.sin_addr));
        uint16_t dst_port = ntohs(dst_addr.sin_port);
        return std::make_pair(dst_ip, dst_port);
    }

    Address Connection::getSrcAddr() const {
        sockaddr_in src_addr{};
        socklen_t src_addr_size = sizeof(src_addr);
        getpeername(m_socket.getFD(), reinterpret_cast<sockaddr*>(&src_addr), &src_addr_size);
        std::string src_ip(inet_ntoa(src_addr.sin_addr));
        uint16_t src_port = ntohs(src_addr.sin_port);
        return std::make_pair(src_ip, src_port);
    }

} // HW