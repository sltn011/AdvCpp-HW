#include "HTTP/BaseHTTPServer.h"
#include "bigFile/Reader.h"
#include "bigFile/FileEntry.h"

using namespace HW::HTTP;
using namespace HW::File;

class Server : public BaseHTTPServer {
    Reader<Entry> m_reader;

public:
    Server(std::string filePath, size_t numThreads)
    : BaseHTTPServer{numThreads}
    , m_reader{filePath, false} {}

    HTTPResponse onRequest(const HTTPRequest &req) {
        static const std::string reqFormat = "/?entry=x";
        size_t reqFormatDataPos = reqFormat.find("=") + 1;

        HTTPResponse resp;
        resp.setHTTPversion(Version::v1_1);
        resp.addHeaderLine("Server", "MyFileServer");
        std::string requested = req.getRequestTarget();
        if (requested.size() < reqFormat.size()) {
            resp.setStatusCode(StatusCode::NotFound);
            return resp;
        }
        if (requested.substr(0, 8) != "/?entry=") {
            resp.setStatusCode(StatusCode::NotFound);
            return resp;
        }

        std::stringstream ss;
        ss << requested.substr(reqFormatDataPos, requested.size() - reqFormatDataPos);
        Key key;
        ss >> key;

        Entry *p = m_reader.getEntry(key);
        if (p == nullptr) {
            resp.setStatusCode(StatusCode::NotFound);
            return resp;
        }
        std::string response((char*)p, sizeof(*p));
        Key answerKey;
        memcpy(&answerKey, response.data(), sizeof(answerKey));
        response.erase(0, sizeof(Key));
        resp.setStatusCode(StatusCode::OK);
        resp.setBody(std::to_string(answerKey) + " " + response);
        std::cout << resp.toString();
        return resp;
    }
};

int main() {
    HW::Logger::get_instance().set_global_logger(HW::create_stderr_logger(HW::Level::ALL));
    Server s("data.bin", 4);
    s.open("127.1.1.1", 8888);
    s.listen(1000);
    s.run(5000);
    return 0;
}