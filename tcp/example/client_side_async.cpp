#include <iostream>
#include <string>
#include "tcp.h"
#include <errno.h>

using msgSize = uint64_t;

int main() {
    HW::Connection connection;
    connection.setSendTimeout(5);
    connection.setRecieveTimeout(5);
    connection.connect("127.1.1.1", 8888);
    std::string msg;
    std::getline(std::cin, msg, '\n');
    msgSize msg_size = static_cast<msgSize>(msg.size());
    connection.writeExact(&msg_size, sizeof(msg_size));
    connection.writeExact(msg.data(), msg_size);
    sleep(3);
    return 0;
}