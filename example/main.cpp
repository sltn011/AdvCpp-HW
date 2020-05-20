#include "HTTP/BaseHTTPServer.h"
#include <iostream>

class Server : public HW::HTTP::BaseHTTPServer {
public:

};

void foo(HW::ConnectionAsync &c) {}

int main() {
    Server s;
    s.open("127.1.1.1", 8888);
    s.setCallback(foo);
    s.listen(5);
    s.setTimeout(std::chrono::seconds{10});
    std::thread t(&Server::run, std::ref(s), 3000);
    t.join();
    return 0;
}