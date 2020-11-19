#include "tcp/Connection.hpp"
#include "HTTP/HTTPMessage.hpp"
#include "bigFile/FileEntry.hpp"
#include <iostream>
#include <cstring>

constexpr size_t answerSize = 128;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Use ./client KEY" << std::endl;
    }
    std::string requested(argv[1], sizeof(HW::File::Key));
    requested = "/?entry=" + requested;
    HW::HTTP::HTTPRequest req;
    req.setHTTPversion(HW::HTTP::Version::v1_1);
    req.setHTTPmethod(HW::HTTP::Method::GET);
    req.setRequestTarget(requested);

    HW::Connection c;
    c.connect("127.1.1.1", 8888);
    std::string msg = req.toString();
    c.write(msg.data(), msg.size());

    std::string answer(answerSize, '\0');
    c.readExact(answer.data(), answer.size());

    HW::File::Key key;
    memcpy(&key, answer.data(), sizeof(HW::File::Key));

    std::cout << answer << std::endl;
}