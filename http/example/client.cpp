#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "tcp/Connection.h"

using msgSize = uint64_t;

int main() {
    int cnt = 0;
    try {
    while (true) {
    HW::Connection
        connection1, connection2, connection3, connection4;
    
            connection1.connect("127.1.1.1", 8888);
            /* connection2.connect("127.1.1.1", 8888);
            connection3.connect("127.1.1.1", 8888);
            connection4.connect("127.1.1.1", 8888); */

            std::string s = "POST / HTTP/1.1\r\nServer: Apache\r\nContent-Length: 12\r\n\r\nABCDEFGHIJKL";

            std::cout << "Write 1" << std::endl;
            connection1.writeExact(s.data(), s.size());
            /* std::cout << "Write 2" << std::endl;
            connection2.writeExact(s.data(), s.size());
            std::cout << "Write 3" << std::endl;
            connection3.writeExact(s.data(), s.size());
            std::cout << "Write 4" << std::endl;
            connection4.writeExact(s.data(), s.size()); */

            std::string answer1(1024, '\0');
            /* std::string answer2(1024, '\0');
            std::string answer3(1024, '\0');
            std::string answer4(1024, '\0'); */

            std::cout << "Read 1 " << connection1.read(answer1.data(), 1024) << std::endl;
            /* std::cout << "Read 2 " << connection2.read(answer2.data(), 1024) << std::endl;
            std::cout << "Read 3 " << connection3.read(answer3.data(), 1024) << std::endl;
            std::cout << "Read 4 " << connection4.read(answer4.data(), 1024) << std::endl; */

            /* std::cout << answer1 << "\n" << std::endl;
            std::cout << answer2 << "\n" << std::endl;
            std::cout << answer3 << "\n" << std::endl;
            std::cout << answer4 << "\n" << std::endl; */

            connection1.close();
            /* connection2.close();
            connection3.close();
            connection4.close(); */

            std::cout << ++cnt <<  " ==================================" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds{2});
        }
    }
    catch(HW::BaseException &e) {
        std::cout << e.what() << " " << errno << std::endl;
    }

    return 0;
}
