#include "HTTP/BaseHTTPServer.hpp"
#include <iostream>

using namespace HW::HTTP;

class Server : public BaseHTTPServer {
public:
    HTTPResponse onRequest(const HTTPRequest &r) {
        std::string msg = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nABCDE";
        HTTPResponse resp{msg};
        if (r.doKeepAlive()) {
            resp.addHeaderLine(Response::Connection, "Keep-Alive");
        }
        return resp;
    }

    Server(size_t numThreads) 
    : BaseHTTPServer{numThreads}
    {

    }
};

int main() {
    HW::Logger::get_instance().set_global_logger(HW::create_stderr_logger(HW::Level::ALL));
    Server s(4);
    s.open("127.1.1.1", 8888);
    s.listen(1000);
    s.run(5000);
    return 0;
}