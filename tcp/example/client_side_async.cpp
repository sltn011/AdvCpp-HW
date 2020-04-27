#include <iostream>
#include <string>
#include "tcp.h"

using msgSize = uint64_t;

int main() {
    HW::Connection connection;
    connection.connect("127.1.1.1", 8888);
    std::string msg;
    std::string buf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456"; // 32 bits
    for (int i = 0; i < (25*1024*1024/32); ++i) {
        msg += buf;
    }

    msgSize msg_size = static_cast<msgSize>(msg.size()); // 25 Mb
    std::cout << "Size of message: " << msg_size << std::endl;

    uint64_t step = (1<<16);
    msg_size = (msg_size/step + 1) * step;
    msg.resize(msg_size, '\0');

    std::string got_back;
    got_back.resize(msg_size);
    
    try {
        for (msgSize i = 0; i < msg_size; i += step) {
            connection.writeExact(msg.data() + i, step);
            std::cout << "Sent " << i + step << std::endl;

            connection.readExact(got_back.data() + i, step);
            std::cout << "Got message back" << std::endl;
        }

        std::string msg_final = "DISCONNECT";
        msg_final.resize(step, '\0');
        connection.writeExact(msg_final.data(), step);
        std::cout << "Sent disconnect message" << std::endl;

        connection.close();
        bool success = true;
        for (msgSize i = 0; i < msg_size; ++i) {
            if (msg.at(i) != got_back.at(i)) {
                success = false;
            }
        }
        std::cout << "Message: " << (success ? "OK" : "Failure") << std::endl;
    }
    catch (HW::BaseException &e) {
        std::cout << e.what() << std::endl;
        std::cout << errno << std::endl;
    }
    return 0;
}