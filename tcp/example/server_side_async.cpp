#include <iostream>
#include <string>
#include "tcp_async.h"
#include <sys/signal.h>
#include <memory>
#include <cstring>
#include <thread>
#include <atomic>

using msgSize = uint64_t;

void callback(HW::ConnectionAsync &c) {
    if (c.isEventSet(EPOLLIN)) {
        try {
            constexpr uint64_t size = (1 << 16);
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
                if (c.isEventSet(EPOLLOUT)) {
                    c.write(c.getBuffer().data(), c.getBuffer().size());
                }
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
    signal(SIGINT, SIG_IGN);
    try {
        HW::ServerAsync server;
        server.open("127.1.1.1", 8888);
        server.setCallback(callback);
        server.setClientEvents(EPOLLIN | EPOLLOUT | EPOLLRDHUP);
        server.listen(16);
        auto[ip, port] = server.getServerAddress();
        std::cout << "Server " << ip << " " << std::to_string(port) << " is accepting" << std::endl;
        std::thread run_thread(&HW::ServerAsync::run, std::ref(server), 3000);
        std::string command;
        while (std::getline(std::cin, command, '\n')) {
            if (command == "/exit/") {
                std::cout << "Closing" << std::endl;
                server.close();
                break;
            }
        }
        run_thread.join();
        return 0;
    }
    catch(HW::BaseException &e) {
        std::cout << e.what() << std::endl;
        std::cout << errno << std::endl;
    }
    return 0;
}