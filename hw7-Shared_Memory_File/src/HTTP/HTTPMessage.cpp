#include "HTTP/HTTPMessage.hpp"
#include <iostream>

namespace HW::HTTP {

    void HTTPMessage::clear() {
        clearHTTPversion();
        clearHeaders();
        clearBody();
    }

    void HTTPMessage::setHTTPversion(const std::string &version) {
        m_version = version;
    }

    std::string HTTPMessage::getHTTPversion() const {
        return m_version;
    }

    void HTTPMessage::clearHTTPversion() {
        m_version.clear();
    }

    void HTTPMessage::addHeaderLine(const std::string &header, const std::string &info) {
        m_headers[header] = info;
    }

    std::string HTTPMessage::getHeaders() const {
        if (m_headers.size() == 0) {
            return {};
        }
        std::string res;
        for (auto &line : m_headers) {
            res += (line.first + ": " + line.second);
            res += "\r\n";
        }
        return res;
    }

    void HTTPMessage::removeHeaderLine(const std::string &header) {
        m_headers.erase(header);
    }

    void HTTPMessage::clearHeaders() {
        m_headers.clear();
    }

    void HTTPMessage::setBody(const std::string &body) {
        m_body = body;
    }

    std::string HTTPMessage::getBody() const {
        return m_body;
    }

    void HTTPMessage::clearBody() {
        m_body.clear();
    }

    HTTPRequest::HTTPRequest() {}
    
    HTTPRequest::HTTPRequest(const std::string &request) {
        fromString(request);
    }

    std::string HTTPRequest::toString() {
        if ((m_headers.find(Request::ContentLength) == m_headers.end()) && (m_body.size() != 0)) {
            m_headers.insert({Request::ContentLength, std::to_string(m_body.size())});
        }
        std::string res;
        res += (getHTTPmethod() + " ");
        res += (getRequestTarget() + " ");
        res += (HW::HTTP::Protocol::HTTP + "/" + getHTTPversion() + "\r\n");
        res += getHeaders();
        res += "\r\n";
        res += getBody();
        return res;
    }

    void HTTPRequest::fromString(const std::string &str) {
        size_t begin = 0;
        size_t end = str.find("\r\n", begin);
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
        } 
        std::string line = str.substr(begin, end - begin);
        
        end = line.find(" ", begin);
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Method: Invalid string passed!" };
        }
        setHTTPmethod(line.substr(begin, end - begin));

        begin = end + 1;
        end = line.find(" ", begin);
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Target: Invalid string passed!" };
        }
        setRequestTarget(line.substr(begin, end - begin));

        begin = end + 1;
        begin = line.find("/", begin);
        if (begin == std::string::npos) {
            throw HW::ArgumentError{ "Version: Invalid string passed!" };
        }
        begin += 1;
        setHTTPversion(line.substr(begin, line.size() - begin));
        
        begin = line.size() + 2;
        end = str.find("\r\n", begin);
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
        line = str.substr(begin, end - begin);
        while (line.size() != 0) {
            size_t p = line.find(": ");
            if (p == std::string::npos) {
                throw HW::ArgumentError{ "Header: Invalid string passed!" };
            }
            addHeaderLine(line.substr(0, p), line.substr(p + 2, line.size()));
            begin += line.size() + 2;
            end = str.find("\r\n", begin);
            if (end == std::string::npos) {
                throw HW::ArgumentError{ "Header Info: Invalid string passed!" };
            }
            line = str.substr(begin, end - begin);
        }

        begin += 2;
        if (m_headers.find(Request::ContentLength) != m_headers.end()) {
            size_t bodySize = 0;
            sscanf(m_headers[Request::ContentLength].c_str(), "%zd", &bodySize);
            if (str.size() - begin != bodySize) {
                throw HW::ArgumentError{ "Body: Invalid string passed!" };
            }
            setBody(str.substr(begin, bodySize));
        }
    }

    void HTTPRequest::setHTTPmethod(const std::string &method) {
        m_method = method;
    }

    std::string HTTPRequest::getHTTPmethod() const {
        return m_method;
    }

    void HTTPRequest::setRequestTarget(const std::string &reqTarget) {
        m_reqTarget = reqTarget;
    }

    std::string HTTPRequest::getRequestTarget() const {
        return m_reqTarget;
    }

    bool HTTPRequest::doKeepAlive() const {
        if (m_headers.find(Request::Connection) == m_headers.end()) {
            return false;
        }
        std::string params = m_headers.at(Request::Connection);
        for (char &c : params) {
            c = std::tolower(c);
        }
        if (params == "keep-alive") {
            return true;
        }
        return false;
    }

    HTTPResponse::HTTPResponse() : m_statusCode{StatusCode::NONE} {}

    HTTPResponse::HTTPResponse(const std::string &response) : m_statusCode{StatusCode::NONE} {
        fromString(response);
    }

    std::string HTTPResponse::toString() {
        if ((m_headers.find(Response::ContentLength) == m_headers.end()) && (m_body.size() != 0)) {
            m_headers.insert({Response::ContentLength, std::to_string(m_body.size())});
        }
        std::string res;
        res += (HW::HTTP::Protocol::HTTP + "/" + getHTTPversion() + " ");
        res += (std::to_string((int)getStatusCode()) + " ");
        res += (getCodeMessage(getStatusCode()) + "\r\n");
        res += getHeaders();
        res += "\r\n";
        res += getBody();
        return res;
    }

    void HTTPResponse::fromString(const std::string &str) {
        size_t begin = 0;
        size_t end = str.find("\r\n", begin);
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
        std::string line = str.substr(begin, end - begin);

        begin = line.find("/", begin);
        if (begin == std::string::npos) {
            throw HW::ArgumentError{ "Version: Invalid string passed!" };
        }
        begin += 1;
        end = line.find(" ");
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Version: Invalid string passed!" };
        }
        setHTTPversion(line.substr(begin, end - begin));

        begin = end + 1;
        end = line.find(" ", begin);
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Status Code: Invalid string passed!" };
        }
        setStatusCode(static_cast<StatusCode>(std::stoi(line.substr(begin, end - begin))));

        begin = line.size() + 2;
        end = str.find("\r\n", begin);
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
        line = str.substr(begin, end - begin);
        while (line.size() != 0) {
            size_t p = line.find(": ");
            if (p == std::string::npos) {
                throw HW::ArgumentError{ "Header: Invalid string passed!" };
            }
            addHeaderLine(line.substr(0, p), line.substr(p + 2, line.size()));
            begin += line.size() + 2;
            end = str.find("\r\n", begin);
            if (end == std::string::npos) {
                throw HW::ArgumentError{ "Header Data: Invalid string passed!" };
            }
            line = str.substr(begin, end - begin);
        }

        begin += 2;
        if (m_headers.find(Response::ContentLength) != m_headers.end()) {
            size_t bodySize = 0;
            sscanf(m_headers[Response::ContentLength].c_str(), "%zd", &bodySize);
            if (str.size() - begin != bodySize) {
                throw HW::ArgumentError{ "Body: Invalid string passed!" };
            }
            setBody(str.substr(begin, bodySize));
        }
    }

    void HTTPResponse::setStatusCode(const StatusCode code) {
        m_statusCode = code;
    }

    StatusCode HTTPResponse::getStatusCode() const {
        return m_statusCode;
    }

    HTTPRequest tryReadRequest(const ConnectionBuffer &buf) {
        std::string req(buf.begin(), buf.end());
        return HTTPRequest(req);
    }

    HTTPResponse tryReadResponse(const ConnectionBuffer &buf) {
        std::string res(buf.begin(), buf.end());
        return HTTPResponse(res);
    }

} // HW::HTTP