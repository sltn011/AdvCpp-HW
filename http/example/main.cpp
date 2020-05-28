#include "HTTP/BaseHTTPServer.h"
#include <iostream>

using namespace HW::HTTP;

class Server : public BaseHTTPServer {
public:
    HTTPResponse onRequest(const HTTPRequest &r) {
        std::string resp = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nABCDE";
        return HTTPResponse{resp};
    }

    Server(size_t numThreads) 
    : BaseHTTPServer{numThreads}
    {

    }
};

int main() {
    HW::Logger::get_instance().set_global_logger(HW::create_stderr_logger(HW::Level::ALL));
    Server s(4);
    auto callback = [&s](HW::ConnectionAsync &c) {
        std::thread::id t_id = std::this_thread::get_id();
		std::stringstream ss_t_id;
		ss_t_id << t_id;
        if (c.isEventSet(EPOLLIN)) {
            HW::info("Thread " + ss_t_id.str() + " Reading request from " + std::to_string(c.getFD()));
            HTTPRequest req = readRequest(c);
            HW::info("Thread " + ss_t_id.str() + " Handling request from " + std::to_string(c.getFD()));
            HTTPResponse resp = s.onRequest(req);
            HW::info("Thread " + ss_t_id.str() + " Writing response to " + std::to_string(c.getFD()));
            writeResponse(c, resp);
            if (!req.doKeepAlive()) {
                c.close();
            }
        }
    };
    s.setCallback(callback);
    s.setClientEvents(EPOLLIN | EPOLLET);
    s.open("127.1.1.1", 8888);
    s.listen(10);
    s.run(5);
    return 0;
}