#include "HTTP/BaseHTTPServer.h"

namespace HW {

    namespace HTTP {
        constexpr uint32_t  serverSocketEvents = EPOLLIN | EPOLLEXCLUSIVE;
        constexpr uint32_t  clientSocketEvents = EPOLLIN | EPOLLOUT | EPOLLET;
        constexpr size_t    readSize           = 256;

        bool cmp::operator()(const ConnTime &a, const ConnTime &b) const {
            return a.first < b.first;
        }

        BaseHTTPServer::BaseHTTPServer(size_t numThreads) {
            size_t maxConcThreads = std::thread::hardware_concurrency();
            if (numThreads > maxConcThreads || numThreads == 0) {
                numThreads = maxConcThreads;
            }
            for (size_t i = 0; i < numThreads; ++i) {
                m_threads.emplace_back(this);
            }
            info("Number of working threads set to " + std::to_string(numThreads));
        }

        BaseHTTPServer::~BaseHTTPServer() {}

        void BaseHTTPServer::open(const std::string &ip, const uint16_t port) {
            m_socket.open();
            trace("Server socket created, fd = " + std::to_string(m_socket.getFD()));
            m_socket.bind(ip, port);
            trace("Address " + ip + ":" + std::to_string(port) + " binded to server socket");
            info("Server opened with address " + ip + ":" + std::to_string(port));
        }

        void BaseHTTPServer::listen(const int queue_size) {
            if (isOpened()) {
                if (::listen(m_socket.getFD(), queue_size) < 0) {
                    std::thread::id t_id = std::this_thread::get_id();
                    std::stringstream ss_t_id;
                    ss_t_id << t_id;
                    error("Thread " + ss_t_id.str() + " Error setting socket as listening!");
                    throw HW::NetworkError("Error setting socket as listening!");
                }
                info("Server socket set to listening state with queue size " + std::to_string(queue_size));
            }
        }

        bool BaseHTTPServer::isOpened() const {
            return m_socket.isOpened();
        }

        void BaseHTTPServer::shutdown() {
            m_shutdown = false;
            info("Server shutdowning");
        }

        Address BaseHTTPServer::getServerAddress() const {
            sockaddr_in src_addr{};
            socklen_t src_addr_size = sizeof(src_addr);
            getsockname(m_socket.getFD(), reinterpret_cast<sockaddr*>(&src_addr), &src_addr_size);
            std::string src_ip(inet_ntoa(src_addr.sin_addr));
            uint16_t src_port = ntohs(src_addr.sin_port);
            return std::make_pair(src_ip, src_port);
        }

        void BaseHTTPServer::setMaxConnectQueue(const int new_max) {
            listen(new_max);
        }

        void BaseHTTPServer::setTimeout(const Timeout timeout) {
            m_timeout = timeout;
        }

        void BaseHTTPServer::run(int epollTimeout) {
            for (size_t i = 0; i < m_threads.size(); ++i) {
                m_shutdown = false;
                m_threads[i].m_thread = std::thread(&BaseHTTPServer::ThreadWork::run, std::ref(m_threads[i]), epollTimeout);
                std::thread::id t_id = m_threads[i].m_thread.get_id();
                std::stringstream ss_t_id;
                ss_t_id << t_id;
                info("Thread " + ss_t_id.str() + " created and running");
            }
            for (size_t i = 0; i < m_threads.size(); ++i) {
                m_threads[i].m_thread.join();
                std::thread::id t_id = m_threads[i].m_thread.get_id();
                std::stringstream ss_t_id;
                ss_t_id << t_id;
                info("Thread " + ss_t_id.str() + " joined");
            }
        }

        BaseHTTPServer::ThreadWork::ThreadWork(BaseHTTPServer *pServer)
        : m_pServer{pServer} {}

        BaseHTTPServer::ThreadWork::~ThreadWork() {
            try {
                for(auto& d : m_connections) {
                    disconnectClient(d.first);
                }
            }
            catch(...) {}
        }

        void BaseHTTPServer::ThreadWork::accept() {
            sockaddr_in client_addr{};
            socklen_t client_addr_size = sizeof(client_addr);
            int client_fd = ::accept4(m_pServer->m_socket.getFD(), 
                reinterpret_cast<sockaddr*>(&client_addr), 
                &client_addr_size, 
                SOCK_NONBLOCK
            );
            if (client_fd < 0) {
                std::thread::id t_id = std::this_thread::get_id();
                std::stringstream ss_t_id;
                ss_t_id << t_id;
                warn("Thread " + ss_t_id.str() + " Error accepting connection!");
                return;
            }
            m_connections.emplace(client_fd, ConnectionAsync(client_fd));
            addToEpoll(client_fd, clientSocketEvents);
            Coroutine::routine_t r_client = Coroutine::create(connectionCallback, m_pServer, std::ref(m_connections[client_fd]));
            m_coroutines.emplace(client_fd, r_client);
            m_idleTime.emplace(std::make_pair(std::chrono::system_clock::now(), client_fd));
        }

        
        void BaseHTTPServer::ThreadWork::initEpoll() {
            int efd = epoll_create(1);
            debug("Epoll created");
            if (efd < 0) {
                std::thread::id t_id = std::this_thread::get_id();
                std::stringstream ss_t_id;
                ss_t_id << t_id;
                error("Thread " + ss_t_id.str() + " Error creating epoll!");
                throw HW::DescriptorError("Error creating epoll!");
            }
            m_epoll.setFD(efd);
        }

        void BaseHTTPServer::ThreadWork::addToEpoll(int fd, uint32_t events) {
            epoll_event e{};
            e.events = events;
            e.data.fd = fd;
            if (epoll_ctl(m_epoll.getFD(), EPOLL_CTL_ADD, e.data.fd, &e) < 0) {
                std::thread::id t_id = std::this_thread::get_id();
                std::stringstream ss_t_id;
                ss_t_id << t_id;
                error("Thread " + ss_t_id.str() + "Error adding connection to epoll! efd:" +
                        std::to_string(m_epoll.getFD()) + " fd:" + std::to_string(fd));
                throw HW::DescriptorError("Error adding connection to epoll!");
            }
            debug("Descriptor " + std::to_string(fd) + " added to epoll");
        }

        void BaseHTTPServer::ThreadWork::removeFromEpoll(int fd) {
            epoll_event e{};
            epoll_ctl(m_epoll.getFD(), EPOLL_CTL_DEL, fd, &e);
            debug("Connection " + std::to_string(fd) + " removed from epoll");
        }

        void BaseHTTPServer::ThreadWork::handleConnection(int fd, uint32_t events) {
            m_connections[fd].setEvent(events);
            Coroutine::routine_t r_id = m_coroutines[fd];
            trace("Coroutine " + std::to_string(r_id) + " resumed for connection " + std::to_string(fd));
            Coroutine::resume(r_id);
            if (!m_connections[fd].isOpened()) {
                debug("Disconnecting " + std::to_string(fd));
                disconnectClient(fd);
            }
            else {
                m_connections[fd].clearEvents();
                if (m_connections[fd].getBuffer().empty()) {
                    trace("New coroutine created for connection " + std::to_string(fd));
                    m_coroutines[fd] = Coroutine::create(connectionCallback, m_pServer, std::ref(m_connections[fd]));
                }
            }
        }

        void BaseHTTPServer::ThreadWork::resetConnectionTime(int fd) {
            auto t = std::find_if(
                m_idleTime.begin(),
                m_idleTime.end(),
                [fd](auto &i) {
                    return i.second == fd; 
                }
            );
            t = m_idleTime.erase(t);
            m_idleTime.emplace_hint(t, std::make_pair(std::chrono::system_clock::now(), fd));
            trace("Idle time reseted for connection " + std::to_string(fd));
        }

        void BaseHTTPServer::ThreadWork::dropTimeoutedConnections() {
			trace("Dropping timeouted connections");
            for (auto i = m_idleTime.begin(); i != m_idleTime.end(); ++i) {
                Timeout timeDiff = std::chrono::duration_cast<Timeout>(std::chrono::system_clock::now() - i->first);
                if (timeDiff > m_pServer->m_timeout) {
                    info("Connection " + std::to_string(i->second) + " timeouted");
                    disconnectClient(i->second);
                }
                else {
                    m_idleTime.erase(m_idleTime.begin(), i);
                    break;
                }
            }
        }

        void BaseHTTPServer::ThreadWork::disconnectClient(int fd) {
            removeFromEpoll(fd);
            m_connections[fd].close();
            trace("Connection " + std::to_string(fd) + " closed");
            m_connections.erase(fd);
            trace("Connection " + std::to_string(fd) + " removed from server");
            m_coroutines.erase(fd);
            trace("Coroutined for connection " + std::to_string(fd) + " removed");
            auto t = std::find_if(
                m_idleTime.begin(),
                m_idleTime.end(),
                [fd](auto &i) {
                    return i.second == fd; 
                }
            );
            m_idleTime.erase(t);
            trace("Connection " + std::to_string(fd) + " removed from timeout");
            debug("Connection " + std::to_string(fd) + " disconnected");
        }

        void BaseHTTPServer::ThreadWork::run(int epollTimeout) {
            std::thread::id t_id = std::this_thread::get_id();
            std::stringstream ss_t_id;
            ss_t_id << t_id;

            initEpoll();
            addToEpoll(m_pServer->m_socket.getFD(), serverSocketEvents);
            info("Thread " + ss_t_id.str() + " working");

            while (!m_pServer->m_shutdown) {
                try {
                    if (!m_epoll.isOpened()) {
                        return;
                    }
					dropTimeoutedConnections();
                    int numFDs = epoll_wait(m_epoll.getFD(), m_events.data(), m_epollsize, epollTimeout);
                    if (numFDs == 0) {
                        continue;
                    }
                    if (numFDs < 0) {
                        if (errno == EINTR) {
                            continue;
                        }
                        else {
                            error("Error getting descriptor from epoll " + std::to_string(m_epoll.getFD()));
                            throw HW::DescriptorError("Error getting descriptors from epoll!");
                        }
                    }
                    debug("Thread " + ss_t_id.str() + " woke up");
                    for (int i = 0; i < numFDs; ++i) {
                        errno = 0;
                        trace("Got: " + std::to_string(numFDs) + " handling: " + std::to_string(i + 1));
                        int fd = m_events.at(i).data.fd;
                        uint32_t connectionEvents = m_events.at(i).events;
                        if (fd == m_pServer->m_socket.getFD()) {
                            debug("Thread " + ss_t_id.str() + " accepting");
                            accept();
                            debug("Thread " + ss_t_id.str() + " finished accepting");
                        }
                        else {
                            debug("Thread " + ss_t_id.str() + " handling connection " + std::to_string(fd));
                            handleConnection(fd, connectionEvents);
                            if(m_connections.find(fd) != m_connections.end()) {
                                resetConnectionTime(fd);
                            }
                            debug("Thread " + ss_t_id.str() + " finished handling connection " + std::to_string(fd));
                        }
                    }
                }
                catch (HW::BaseException &e) {
                    warn("Thread " + ss_t_id.str() + " got an error " + e.what());
                    continue;
                }
            }
            for(auto& d : m_connections) {
                disconnectClient(d.first);
            }
            info("Thread " + ss_t_id.str() + " finished working");
        }

        void connectionCallback(BaseHTTPServer *pServer, HW::ConnectionAsync &c) {
            std::thread::id t_id = std::this_thread::get_id();
            std::stringstream ss_t_id;
            ss_t_id << t_id;
            HW::debug("Thread " + ss_t_id.str() + " Reading request from " + std::to_string(c.getFD()));
            HTTPRequest req = readRequest(c);
            HW::debug("Thread " + ss_t_id.str() + " Handling request from " + std::to_string(c.getFD()));
            HTTPResponse resp = pServer->onRequest(req);
            HW::debug("Thread " + ss_t_id.str() + " Writing response to " + std::to_string(c.getFD()));
            writeResponse(c, resp);
            if (!req.doKeepAlive()) {
                c.close();
            }
        }

        HTTPRequest readRequest(ConnectionAsync &c) {
            while (true) {
                errno = 0;
                size_t recieved = c.readToBuffer(readSize);
                trace("Read " + std::to_string(recieved) + " from connection " + std::to_string(c.getFD()));
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    try {
                        std::string req(c.getBuffer().begin(), c.getBuffer().end());
                        return HTTPRequest{req};
                    }
                    catch(ArgumentError &e) {
                        trace("Not full request recieved from " + std::to_string(c.getFD()) + ". Yielding");
                        Coroutine::yield();
                        continue;
                    }
                }
            }
        }

        void writeResponse(ConnectionAsync &c, HTTPResponse &resp) {
            std::string msg = resp.toString();
            size_t msg_size = msg.size();
            while (msg.size() != 0) {
                errno = 0;
                size_t sent = c.write(msg.data(), msg.size());
                trace("Wrote " + std::to_string(sent) + " to connection " + std::to_string(c.getFD()));
                msg.erase(0, sent);
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    trace("Not full request sent to " + std::to_string(c.getFD()) +
                    " Only sent " + std::to_string(msg_size - msg.size()) + " of " + std::to_string(msg_size) + ". Yielding");
                    Coroutine::yield();
                }
            }
            c.getBuffer().clear();
        }

    } // HTTP

} // HW