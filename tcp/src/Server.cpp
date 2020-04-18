#include "Server.h"

namespace HW {

    Server::Server(const std::string &ip, const uint16_t port)
    : m_addr{std::make_pair(ip, port)} {}

    void Server::open() {
        m_socket.open();
        m_socket.bind(m_addr.first, m_addr.second);
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

        Address client = std::make_pair(client_ip, client_port);
        Address server = m_addr;
        return Connection(client_fd, client, server);
    }

    bool Server::isOpened() const {
        return m_socket.isOpened();
    }

    void Server::close() {
        m_socket.close();
    }

    void Server::setMaxConnect(const int new_max) {
        listen(new_max);
    }


} // HW