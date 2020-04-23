#include <iostream>
#include <string>
#include "tcp_async.h"
#include <sys/signal.h>
#include <memory>
#include <cstring>

using msgSize = uint64_t;


class SingletonServer{
private:
    std::unique_ptr<HW::ServerAsync> m_server_global;
    SingletonServer() : m_server_global{new HW::ServerAsync} {}
    SingletonServer(const SingletonServer &rhs) = delete;
    SingletonServer &operator=(const SingletonServer &rhs) = delete;

public:
    static SingletonServer &get_instance() {
        static SingletonServer server_instance{};
        return server_instance;
    }

    HW::ServerAsync &get_global_server() {
        return *m_server_global;
    }
};


void sigint_handler(int signal) {
    std::cout << "\nServer shutdown" << std::endl;
    SingletonServer::get_instance().get_global_server().close();
}


void callback(HW::ConnectionAsync &c) {
    if (c.isEventSet(EPOLLIN)) {
        try {
            constexpr msgSize size = (1 << 16);
            msgSize recieved = c.getBuffer().size();
            if (recieved != size) {
                recieved += c.readToBuffer(size - recieved);
            }
            if (recieved == size) {
                std::cout << "Got: " << recieved << std::endl;
                std::string check_final(sizeof("DISCONNECT") - 1, '\0');
                std::memcpy(check_final.data(), c.getBuffer().data(), check_final.size());
                if (check_final == "DISCONNECT") {
                    std::cout << "Got disconnect message" << std::endl;
                    c.close();
                    return;
                }
                c.write(c.getBuffer().data(), c.getBuffer().size());
                c.clearBuffer();
            }
        }
        catch(HW::BaseException &e) {
            if (errno == EAGAIN || errno == EINTR) {
                return;
            }
            else {
                c.close();
            }
        }
    }
}

int main() {
    signal(SIGINT, sigint_handler);
    try {
    SingletonServer::get_instance().get_global_server().open("127.1.1.1", 8888);
    SingletonServer::get_instance().get_global_server().setCallback(callback);
    SingletonServer::get_instance().get_global_server().setClientEvents(EPOLLIN | EPOLLRDHUP);
    SingletonServer::get_instance().get_global_server().listen(16);
    auto[ip, port] = SingletonServer::get_instance().get_global_server().getServerAddress();
    std::cout << "Server " << ip << " " << std::to_string(port) << " is accepting" << std::endl;
    SingletonServer::get_instance().get_global_server().run();
    SingletonServer::get_instance().get_global_server().close();
    }
    catch(HW::BaseException &e) {
        std::cout << e.what() << std::endl;
        std::cout << errno << std::endl;
    }
    return 0;
}