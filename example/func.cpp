#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[]) {
    std::cout << "Hello, parent!";
    std::string msg;
    std::getline(std::cin, msg, '\0');
    std::cout << "Child recieved " << msg;
    return 0;
}
