#include <iostream>
#include <string>
#include <sys/unistd.h>

using msgSize = uint32_t;

void writeSize(const msgSize size) {
    const msgSize * pSize = &size;
    for (size_t i = 0; i < sizeof(size); ++i) {
        std::cout << *(reinterpret_cast<const char*>(pSize) + i);
    }
}

msgSize readSize() {
    msgSize size = 0;
    std::cin.read(reinterpret_cast<char*>(&size), sizeof(size));
    return size;
}

int main(int argc, char* argv[]) {
    std::string message = "Hello, parent!";
    writeSize(static_cast<msgSize>(message.size()));
    std::cout << message;

    msgSize size = readSize();
    std::string newMessage(size, '\0');
    std::cin.read(newMessage.data(), size);

    newMessage = std::string("Child recieved ") + newMessage;
    writeSize(static_cast<msgSize>(newMessage.size()));
    std::cout << newMessage;
    return 0;
}
