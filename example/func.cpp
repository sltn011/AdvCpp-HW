#include <iostream>
#include <string>
#include <unistd.h>
#include <bitset>

int main(int argc, char* argv[]) {
    std::string message = "Hello, parent!";
    std::cout << message.size() << message;
    
    std::getline(std::cin, message, '\0');
    message = std::string("Child recieved ") + message;
    std::cout << message.size() << message;
    return 0;
}
