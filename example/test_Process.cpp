#include "Process.h"


using msgSize = uint32_t;

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        std::cerr << "First argument must be process path!" << std::endl;
        return 1;
    }

    std::string path = argv[1];
    std::vector<std::string> procArgs;
    for (int i = 1; i < argc; ++i) {
        procArgs.push_back(argv[i]);
    }


    HW::Process proc(path, procArgs);
    

    std::string message;
    try {
        msgSize size = 0;
        proc.readExact(&size, sizeof(size));
        message.resize(size);
        proc.readExact(message.data(), size);
    } catch(HW::IOError & e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Parent recieved " << message << std::endl;


    message = "Hello, child!";
    std::cout << "Sending message to child" << std::endl;
    msgSize to_send = static_cast<msgSize>(message.size());
    proc.writeExact(&to_send, sizeof(to_send));
    proc.writeExact(message.data(), to_send);


    try {
        msgSize size = 0;
        proc.readExact(&size, sizeof(size));
        message.resize(size);
        proc.readExact(message.data(), size);
    } catch(HW::IOError & e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Parent recieved " << message << std::endl;

    return 0;
}