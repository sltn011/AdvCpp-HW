#include "Process.h"


/*
 * constexpr - доступна во время компиляции
 * static - существует пока запущена программа, доступна только в этом файле
 */
static constexpr int BUF_SIZE = 100;

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
    /* procArgs.push_back(nullptr); - не обязательно, в конструкторе процесса 
     * nullptr добавляется перед вызовом execl
     */

    HW::Process proc(path, procArgs);
    
    std::string message;
    message.resize(BUF_SIZE);
    try {
        std::string len(2, '\0'); // 2 потому что у меня BUF_SIZE = 100, как сделать лучше не знаю(
        // Читаю длину сообщения, приходится так потому что std::cout сайз сообщения преобразует в чары
        // потом строку len преобразую в число байт которые надо считать из процесса
        // Пробовал записывать размер сообщения через bitset и записывать в size_t но получал EFAULT
        proc.readExact(len.data(), len.size()); 
        proc.readExact(message.data(), std::stoi(len));
    } catch(HW::IOError & e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Parent recieved " << message << std::endl;


    message = "Hello, child!";
    std::cout << "Sending message to child" << std::endl;
    proc.writeExact(message.c_str(), message.size() + 1);


    message.resize(BUF_SIZE);
    try {
        std::string len(2, '\0');
        proc.readExact(len.data(), len.size());
        proc.readExact(message.data(), std::stoi(len));
    } catch(HW::IOError & e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Parent recieved " << message << std::endl;

    return 0;
}