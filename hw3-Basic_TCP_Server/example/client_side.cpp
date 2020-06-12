#include <iostream>
#include "tcp.h"
#include <errno.h>

int main() {
    HW::Connection connection;
    connection.setRecieveTimeout(5);
    connection.setSendTimeout(5);
    connection.connect("127.1.1.1", 8888);
    auto[dst_ip, dst_port] = connection.getDstAddr();
    auto[src_ip, src_port] = connection.getSrcAddr();
    std::cout << "Dst: " << dst_ip << ' ' << dst_port << std::endl;
    std::cout << "Src: " << src_ip << ' ' << src_port << std::endl;
    connection.close();
    return 0;
}