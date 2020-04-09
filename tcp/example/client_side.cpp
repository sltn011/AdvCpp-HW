#include <iostream>
#include "tcp.h"
#include <errno.h>

int main() {
    try {
        HW::Connection connection("1.1.1.1", 8888);
        connection.set_timeout(5);
        std::cout << "Connected: " << std::boolalpha << connection.connect("127.1.1.1", 8888) << std::endl;
        connection.close();
    }
    catch (HW::BaseException &e) {
        std::cout << e.what() << std::endl;
        std::cout << "Errno: " << errno << std::endl;
    }
    
    return 0;
}