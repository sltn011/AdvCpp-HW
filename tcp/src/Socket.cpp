#include "Socket.h"

namespace HW {

    Socket::Socket() : m_fd{-1} {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            throw HW::DescriptorError("Error creating socket!");
        }
        m_fd = fd;
    }

    Socket::Socket(int valid_sfd) : m_fd{valid_sfd} {}

    Socket::Socket(Socket &&rhs) noexcept : m_fd{rhs.m_fd} {
        rhs.m_fd = -1;
    }

    Socket::~Socket() noexcept {
        try {
            close();
        }
        catch (HW::DescriptorError &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void Socket::open() {
        if (isOpened()) {
            return;
        }
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            throw HW::DescriptorError("Error opening socket!");
        }
        m_fd = fd;
    }

    void Socket::bind(const std::string &ip, const uint16_t port) {
        if (!isOpened()) {
            return;
        }
        sockaddr_in sockaddr_{};
        sockaddr_.sin_family = AF_INET;
        sockaddr_.sin_port = htons(port);
        if (inet_aton(ip.c_str(), &sockaddr_.sin_addr) == 0) {
            throw HW::IOError("Error parsing address!");
        }
        if (::bind(m_fd, reinterpret_cast<sockaddr*>(&sockaddr_), sizeof(sockaddr_)) < 0) {
            throw HW::NetworkError("Error binding IP:Port to socket!");
        }
    }

    int Socket::getFD() const {
        return m_fd;
    }

    bool Socket::isOpened() const {
        return m_fd != -1;
    }
    
    void Socket::close() {
        if (isOpened()) {
           while (::close(m_fd) < 0) {
               if (errno != EINTR) {
                   throw HW::DescriptorError("Error closing socket!");
               }
            }
            m_fd = -1;
        }
    }

} // HW