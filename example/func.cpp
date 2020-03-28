#include <iostream>
#include <string>

using msgSize = uint64_t;

void writeSize(const msgSize size) {
    std::cout.write(reinterpret_cast<const char*>(&size), sizeof(size));
    std::cout.flush();
}

msgSize readSize() {
    msgSize size = 0;
    std::cin.read(reinterpret_cast<char*>(&size), sizeof(size));
    return size;
}

int main(int argc, char* argv[]) {
    std::string message = "Hello, parent!";
    writeSize(static_cast<msgSize>(message.size()));
    std::cout << message << std::flush;

    msgSize size = readSize();
    std::string newMessage(size, '\0');
    std::cin.read(newMessage.data(), size);

    newMessage = std::string("Child recieved ") + newMessage;
    writeSize(static_cast<msgSize>(newMessage.size()));
    std::cout << newMessage << std::flush;
    return 0;
}
