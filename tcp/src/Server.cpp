#include "Server.h"

namespace HW {

    Server::Server(const std::string & ip, const uint16_t port)
    : m_server_ip{ip}, m_server_port{port}, m_opened{false} {}

    void Server::open() {
        if (isOpened()) {
            return;
        }
        try {
            m_socket.open();
            m_socket.bind(m_server_ip, m_server_port);
            m_opened = true;
        }
        catch (HW::DescriptorError &e) {
            throw;
        }
        catch (HW::IOError &e) {
            throw;
        }
        catch (HW::NetworkError &e) {
            throw;
        }
    }

    void Server::listen(const int queue_size) {
        if (isOpened()) {
            if (::listen(m_socket.getFD(), queue_size) < 0) {
                throw HW::NetworkError("Error setting socket as listening!");
            }
        }
    }

    Connection Server::accept() {
        if (!isOpened()) {
            throw HW::NetworkError("Server closed!");
        }
        sockaddr_in client_addr{};
        socklen_t client_addr_size = sizeof(client_addr);
        int client_fd = ::accept(m_socket.getFD(), reinterpret_cast<sockaddr*>(&client_addr), &client_addr_size);
        if (client_fd < 0) {
            throw HW::NetworkError("Error accepting connection!");
        }
        std::string client_ip(inet_ntoa(client_addr.sin_addr));
        uint16_t client_port = ntohs(client_addr.sin_port);

        std::pair client = std::make_pair(client_ip, client_port);
        std::pair server = std::make_pair(m_server_ip, m_server_port);
        return Connection(client_fd, client, server);
    }

    bool Server::isOpened() const {
        return m_opened;
    }

    void Server::close() {
        try {
            m_socket.close();
            m_opened = false;
        }
        catch (HW::DescriptorError &e) {
            throw;
        }
    }

    void Server::set_max_connect(const int new_max) {
        try {
            listen(new_max);
        }
        catch (HW::NetworkError &e) {
            throw;
        }
    }


} // HW