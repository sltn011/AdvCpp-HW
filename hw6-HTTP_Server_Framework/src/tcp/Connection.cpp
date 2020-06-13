#include "tcp/Connection.hpp"

namespace HW {

    Connection::Connection() {}

    Connection::Connection(const int fd, Address &dst, Address &src)
    : m_socket{fd}, m_dst_addr{dst}, m_src_addr{src} {}

    size_t Connection::read(void *data, size_t size) {
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

    size_t Connection::write(const void *data, size_t size) {
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
        m_src_addr.first.clear();
        m_src_addr.second = 0;
        m_dst_addr.first.clear();
        m_dst_addr.second = 0;
    }

    bool Connection::isOpened() const {
        return !m_src_addr.first.empty();
    }

    void Connection::setTimeout(int seconds, int optname) {
        if (!m_socket.isOpened()) {
            return;
        }
        timeval timeout{seconds, 0};
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
        m_src_addr = std::make_pair(ip, port);

        sockaddr_in dst_addr{};
        socklen_t dst_addr_size = sizeof(dst_addr);
        getsockname(m_socket.getFD(), reinterpret_cast<sockaddr*>(&dst_addr), &dst_addr_size);
        std::string dst_ip(inet_ntoa(dst_addr.sin_addr));
        uint16_t dst_port = ntohs(dst_addr.sin_port);
        m_dst_addr = std::make_pair(dst_ip, dst_port);
    }

    Address Connection::getDstAddr() const {
        return m_dst_addr;
    }
        
    Address Connection::getSrcAddr() const {
        return m_src_addr;
    }

} // HW