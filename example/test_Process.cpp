#include "Process.h"

constexpr int BUF_SIZE = 100;

int main(int argc, char *argv[]) {

    if (argv[1] == nullptr) {
        std::cerr << "First argument must be process path!" << std::endl;
        return 1;
    }

    std::string path = argv[1];
    std::vector<std::string> procArgs;
    char **ptr = argv + 2;
    while (*ptr) {
        procArgs.push_back(std::string(*ptr));
        ++ptr;
    }

    HW::Process proc(path, procArgs);
    
    std::string message;
    message.resize(BUF_SIZE);
    size_t t = proc.read(message.data(), BUF_SIZE);
    std::cout << "Read " << t << std::endl;
    message.resize(t);
    std::cout << "Parent recieved " << message << std::endl;

    message = "Hello, child!";
    std::cout << "Sending message to child" << std::endl;
    t = proc.write(message.c_str(), message.size() + 1);
    std::cout << "Wrote " << t << std::endl;

    message.resize(BUF_SIZE);
    t = proc.read(message.data(), BUF_SIZE);
    std::cout << "Read " << t << std::endl;
    message.resize(t);
    std::cout << "Parent recieved " << message << std::endl;

    return 0;
}