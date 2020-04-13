#include "Socket.h"

namespace HW {

    Socket::Socket() {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            throw HW::DescriptorError("Error creating socket!");
        }
        m_fd = fd;
        m_opened = true;
    }

    Socket::Socket(int valid_sfd) : m_fd{valid_sfd}, m_opened{true} {}

	Socket::Socket(Socket &&rhs) noexcept : m_fd{ rhs.m_fd }, m_opened{ rhs.m_opened } {
		rhs.m_fd = -1;
		rhs.m_opened = false;
    }

    Socket::~Socket() noexcept {
        if (isOpened()) {
           while (::close(m_fd) < 0) {
               if (errno != EINTR) {
                   std::cerr << ("Error closing socket!") << std::endl;
               }
            }
            m_opened = false;
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
        m_opened = true;
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
        return m_opened;
    }
    
    void Socket::close() {
        if (isOpened()) {
           while (::close(m_fd) < 0) {
               if (errno != EINTR) {
                   throw HW::DescriptorError("Error closing socket!");
               }
            }
            m_fd = -1;
            m_opened = false;
        }
    }

} // HW