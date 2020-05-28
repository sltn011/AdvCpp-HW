#include "HTTP/BaseHTTPServer.h"

namespace HW {

    namespace HTTP {
		constexpr uint32_t	serverSocketEvents	= EPOLLIN | EPOLLEXCLUSIVE;
		constexpr Timeout	defTimeout			{5};

		bool cmp::operator()(const ConnTime &a, const ConnTime &b) const {
			return a.first < b.first;
		}

	    BaseHTTPServer::BaseHTTPServer(size_t numThreads) {
			size_t maxConcThreads = std::thread::hardware_concurrency();
			if (numThreads > maxConcThreads || numThreads == 0) {
				m_num_threads = maxConcThreads;
			}
			else {
				m_num_threads = numThreads;
			}
			trace("Number of working threads set to " + std::to_string(m_num_threads));
			for (size_t i = 0; i < m_num_threads; ++i) {
				m_thrWorks.emplace_back(m_socket.getFD(), defTimeout);
			}
		}

	    BaseHTTPServer::~BaseHTTPServer() {}

	    void BaseHTTPServer::setCallback(Callback callback) {
	    	for (auto &t : m_thrWorks) {
				t.m_callback = callback;
			}
			trace("Changed callback");
	    }

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
					error("Thread " + ss_t_id.str() + " Error setting socket as listening! errno:" + std::to_string(errno));
		    		throw HW::NetworkError("Error setting socket as listening!");
		    	}
				info("Server socket set to listening state with queue size " + std::to_string(queue_size));
		    }
	    }

    	bool BaseHTTPServer::isOpened() const {
	    	return m_socket.isOpened();
	    }

		void BaseHTTPServer::shutdown() {
			for (auto &t : m_thrWorks) {
				t.m_shutdown = true;
			}
			trace("Server shutdowning");
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
			for (auto &t : m_thrWorks) {
				t.m_timeout = timeout;
			}
		}

    	void BaseHTTPServer::setClientEvents(uint32_t events) {
    		for (auto &t : m_thrWorks) {
				t.m_clientEvents = events;
			}
			trace("Changed events to set for clients");
    	}

		void BaseHTTPServer::run(int epollTimeout) {
			for (size_t i = 0; i < m_num_threads; ++i) {
				m_threads.emplace_back(&BaseHTTPServer::ThreadWork::run, std::ref(m_thrWorks[i]), epollTimeout);
				std::thread::id t_id = m_threads.back().get_id();
				std::stringstream ss_t_id;
				ss_t_id << t_id;
				trace("Thread " + ss_t_id.str() + " created and running");
			}
			for (size_t i = 0; i < m_num_threads; ++i) {
				m_threads[i].join();
				std::thread::id t_id = m_threads[i].get_id();
				std::stringstream ss_t_id;
				ss_t_id << t_id;
				trace("Thread " + ss_t_id.str() + " joined");
			}
		}

		void BaseHTTPServer::ThreadWork::accept() {
		    sockaddr_in client_addr{};
		    socklen_t client_addr_size = sizeof(client_addr);
		    int client_fd = ::accept4(m_server_fd, 
			    reinterpret_cast<sockaddr*>(&client_addr), 
			    &client_addr_size, 
			    SOCK_NONBLOCK
			);
		    if (client_fd < 0) {
				std::thread::id t_id = std::this_thread::get_id();
				std::stringstream ss_t_id;
				ss_t_id << t_id;
				warn("Thread " + ss_t_id.str() + " Error accepting connection! errno:" + std::to_string(errno));
		    	throw HW::NetworkError("Error accepting connection!");
		    }
			m_connections.emplace(client_fd, ConnectionAsync(client_fd));
			addToEpoll(client_fd, m_clientEvents);
			Coroutine::routine_t r_client = Coroutine::create(m_callback, std::ref(m_connections[client_fd]));
			m_coroutines.emplace(client_fd, r_client);
			m_idleTime.emplace(std::make_pair(std::chrono::system_clock::now(), client_fd));
    	}

		BaseHTTPServer::ThreadWork::ThreadWork(int server_fd, Timeout idle_time)
		: m_server_fd{server_fd}
		, m_timeout{idle_time}
		, m_shutdown{false} {}

		BaseHTTPServer::ThreadWork::~ThreadWork() {
			try {
				for(auto& d : m_connections) {
					disconnectClient(d.first);
				}
			}
			catch(...) {}
		}
		
		void BaseHTTPServer::ThreadWork::initEpoll() {
		    int efd = epoll_create(1);
			debug("Epoll created");
		    if (efd < 0) {
				std::thread::id t_id = std::this_thread::get_id();
				std::stringstream ss_t_id;
				ss_t_id << t_id;
				error("Thread " + ss_t_id.str() + " Error creating epoll! errno:" + std::to_string(errno));
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
			info("Descriptor " + std::to_string(fd) + " added to epoll");
    	}

		void BaseHTTPServer::ThreadWork::removeFromEpoll(int fd) {
    		epoll_event e{};
    		epoll_ctl(m_epoll.getFD(), EPOLL_CTL_DEL, fd, &e);
			info("Connection " + std::to_string(fd) + " removed from epoll");
    	}

		void BaseHTTPServer::ThreadWork::handleConnection(int fd, uint32_t events) {
			std::thread::id t_id = std::this_thread::get_id();
			std::stringstream ss_t_id;
			ss_t_id << t_id;
			debug("Thread " + ss_t_id.str() + " handling connection " + std::to_string(fd));

    		m_connections[fd].setEvent(events);
			Coroutine::routine_t r_id = m_coroutines[fd];
    		if (m_callback) {
				trace("Coroutine " + std::to_string(r_id) + " resumed for connection " + std::to_string(fd));
    			Coroutine::resume(r_id);
    		}
			if (!m_connections[fd].isOpened()) {
				info("Disconnecting " + std::to_string(fd));
				disconnectClient(fd);
			}
    		else {
				m_connections[fd].clearEvents();
			}
			debug("Thread " + ss_t_id.str() + " finished handling connection " + std::to_string(fd));
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
			for (auto i = m_idleTime.begin(); i != m_idleTime.end(); ++i) {
				Timeout timeDiff = std::chrono::duration_cast<Timeout>(std::chrono::system_clock::now() - i->first);
				if (timeDiff > m_timeout) {
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
		}

		void BaseHTTPServer::ThreadWork::run(int epollTimeout) {
			std::thread::id t_id = std::this_thread::get_id();
			std::stringstream ss_t_id;
			ss_t_id << t_id;
			info("Thread " + ss_t_id.str() + " working");

			initEpoll();
			addToEpoll(m_server_fd, serverSocketEvents);
			m_shutdown = false;

			try {
				while (!m_shutdown) {
					if (!m_epoll.isOpened()) {
						return;
					}
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
						int fd = m_events.at(i).data.fd;
						uint32_t connectionEvents = m_events.at(i).events;
						if (fd == m_server_fd) {
							debug("Thread " + ss_t_id.str() + " accepting");
							accept();
							debug("Thread " + ss_t_id.str() + " finished accepting");
						}
						else {
							handleConnection(fd, connectionEvents);
							if(m_connections.find(fd) != m_connections.end()) {
								resetConnectionTime(fd);
							}
						}
					}
					dropTimeoutedConnections();
				}
			}
			catch (std::exception &e) {
				for(auto& d : m_connections) {
					disconnectClient(d.first);
				}
				error("Thread " + ss_t_id.str() + " finished working with error " + 
					e.what() + " errno: " + std::to_string(errno));
				return;
			}
			for(auto& d : m_connections) {
				disconnectClient(d.first);
			}
			info("Thread " + ss_t_id.str() + " finished working");
		}

		HTTPRequest readRequest(ConnectionAsync &c) {
			while (true) {
				try {
					size_t recieved = c.readToBuffer(256);
					trace("Read " + std::to_string(recieved) + " from connection " + std::to_string(c.getFD()));
				}
				catch(BaseException &e) {
					if (errno != EAGAIN) {
						throw;
					}
					try {
						std::string req(c.getBuffer().begin(), c.getBuffer().end());
						return HTTPRequest{req};
					}
					catch(BaseException &e) {
						trace("Mot full request recieved from " + std::to_string(c.getFD()) + ". Yielding");
						Coroutine::yield();
					}
				}
			}
		}

		void writeResponse(ConnectionAsync &c, HTTPResponse &resp) {
			std::string msg = resp.toString();
			while (msg.size() != 0) {
				try {
					size_t sent = c.write(msg.data(), msg.size());
					trace("Wrote " + std::to_string(sent) + " to connection " + std::to_string(c.getFD()));
					msg.erase(0, sent);
				}
				catch (BaseException &e) {
					if (errno != EAGAIN) {
						throw;
					}
					trace("Mot full request sent to " + std::to_string(c.getFD()) + ". Yielding");
					Coroutine::yield();
				}
			}
		}

    } // HTTP

} // HW