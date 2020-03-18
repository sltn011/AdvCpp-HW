#include "Process.h"

enum { BUF_SIZE = 100 };

int main(int argc, char *argv[]) {
    Process proc("./func", argv);
    
    char msg_rec[BUF_SIZE];
    int t = proc.read(msg_rec, BUF_SIZE - 1);
    std::cout << "Read " << t << std::endl;
    msg_rec[t] = '\0';
    std::cout << "Parent recieved " << msg_rec << std::endl;

    char msg_sent[] = "Hello, child!";
    std::cout << "Sending message to child" << std::endl;
    t = proc.write(msg_sent, strlen(msg_sent) + 1);
    std::cout << "Wrote " << t << std::endl;

    t = proc.read(msg_rec, BUF_SIZE - 1);
    std::cout << "Read " << t << std::endl;
    msg_rec[t] = '\0';
    std::cout << "Parent recieved " << msg_rec << std::endl;

    return 0;
}