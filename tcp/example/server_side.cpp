#include <iostream>
#include "tcp.h"
#include <errno.h>

int main(int argc, char *argv[]) {
    try {
        HW::Server server("127.1.1.1", 8888);
        server.open();
        server.listen(5);
        std::cout << "accepting:\n";
        HW::Connection connection = server.accept();
        std::cout << "Connection: " << connection.isOpened() << std::endl;
        auto[dst_adr, dst_port] = connection.getDstAddr();
        auto[src_adr, src_port] = connection.getSrcAddr();
        std::cout << "Dst: " << dst_adr << ' ' << dst_port << std::endl;
        std::cout << "Src: " << src_adr << ' ' << src_port << std::endl;
        server.close();
    }
    catch (HW::BaseException &e) {
        std::cout << e.what() << std::endl;
        std::cout << "Errno: " << errno << std::endl;
    }
    
    return 0;
}