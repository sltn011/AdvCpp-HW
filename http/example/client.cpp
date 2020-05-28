#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "tcp/Connection.h"

using msgSize = uint64_t;

int main() {
    try {
    HW::Connection
        connection1, connection2, connection3,
        connection4, connection5, connection6,
        connection7, connection8, connection9;
    
    connection1.connect("127.1.1.1", 8888);
    connection2.connect("127.1.1.1", 8888);
    connection3.connect("127.1.1.1", 8888);
    connection4.connect("127.1.1.1", 8888);
    connection5.connect("127.1.1.1", 8888);
    connection6.connect("127.1.1.1", 8888);
    connection7.connect("127.1.1.1", 8888);
    connection8.connect("127.1.1.1", 8888);
    connection9.connect("127.1.1.1", 8888);

    std::string s = "POST / HTTP/1.1\r\nServer: Apache\r\nContent-Length: 12\r\n\r\nABCDEFGHIJKL";

    connection1.writeExact(s.data(), s.size());
    connection2.writeExact(s.data(), s.size());
    connection3.writeExact(s.data(), s.size());
    connection4.writeExact(s.data(), s.size());
    connection5.writeExact(s.data(), s.size());
    connection6.writeExact(s.data(), s.size());
    connection7.writeExact(s.data(), s.size());
    connection8.writeExact(s.data(), s.size());
    connection9.writeExact(s.data(), s.size());

    //std::this_thread::sleep_for(std::chrono::seconds{2});

    std::string answer1(1024, '\0');
    std::string answer2(1024, '\0');
    std::string answer3(1024, '\0');
    std::string answer4(1024, '\0');
    std::string answer5(1024, '\0');
    std::string answer6(1024, '\0');
    std::string answer7(1024, '\0');
    std::string answer8(1024, '\0');
    std::string answer9(1024, '\0');

    connection1.read(answer1.data(), 1024);
    connection2.read(answer2.data(), 1024);
    connection3.read(answer3.data(), 1024);
    connection4.read(answer4.data(), 1024);
    connection5.read(answer5.data(), 1024);
    connection6.read(answer6.data(), 1024);
    connection7.read(answer7.data(), 1024);
    connection8.read(answer8.data(), 1024);
    connection9.read(answer9.data(), 1024);

    std::cout << answer1 << "\n" << std::endl;
    std::cout << answer2 << "\n" << std::endl;
    std::cout << answer3 << "\n" << std::endl;
    std::cout << answer4 << "\n" << std::endl;
    std::cout << answer5 << "\n" << std::endl;
    std::cout << answer6 << "\n" << std::endl;
    std::cout << answer7 << "\n" << std::endl;
    std::cout << answer8 << "\n" << std::endl;
    std::cout << answer9 << "\n" << std::endl;
    }
    catch(HW::BaseException &e) {
        std::cout << e.what() << " " << errno << std::endl;
    }

    return 0;
}
