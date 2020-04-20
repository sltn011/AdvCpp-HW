#include <iostream>
#include <string>
#include "tcp_async.h"
#include <sys/signal.h>

using msgSize = uint64_t;

HW::ServerAsync *ptr = nullptr;

void sigint_handler(int signal) {
    std::cout << "Server shutdown" << std::endl;
    if (ptr) {
        ptr->close();
    }
}

void callback(HW::ConnectionAsync &c) {
    if (c.isEventSet(EPOLLIN)) {
        msgSize msg_size = 0;
        try {
            c.readExact(&msg_size, sizeof(msg_size));
            c.readExactToBuffer(msg_size);
        }
        catch (HW::BaseException) {
            if (errno == EAGAIN) {
                callback(c);
            }
        }
        std::cout << "Recieved: ";
        for (size_t i = 0; i < msg_size; ++i) {
            std::cout << c.getBuffer().at(i);
        }
        std::cout << " from ";
        auto[ip, port] = c.getDstAddr();
        std::cout << ip << " " << std::to_string(port) << std::endl;
        c.clearBuffer();
    }
}

int main() {
    signal(SIGINT, sigint_handler);
    HW::ServerAsync server;
    ptr = &server;
    server.open("127.1.1.1", 8888);
    server.setCallback(callback);
    server.setClientEvents(EPOLLIN | EPOLLRDHUP);
    server.listen(16);
    auto[ip, port] = server.getServerAddress();
    std::cout << "Server " << ip << " " << std::to_string(port) << " is accepting" << std::endl;
    server.run();
    server.close();
    return 0;
}