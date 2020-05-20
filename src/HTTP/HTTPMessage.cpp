#include "HTTP/HTTPMessage.h"

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

    void HTTPMessage::addHeaderLine(const std::string &line) {
        m_headers.push_back(line);
    }

    std::string HTTPMessage::getHeaders() const {
		if (m_headers.size() == 0) {
			return {};
		}
        std::string res;
        for (auto &line : m_headers) {
            res += line;
            res += "\r\n";
        }
        return res;
    }

    void HTTPMessage::removeHeaderLine(size_t line) {
        if (line >= m_headers.size()) {
            return;
        }
        m_headers.erase(m_headers.begin() + line);
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

    std::string HTTPRequest::toString() const {
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
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
		setHTTPmethod(line.substr(begin, end - begin));

		begin = end + 1;
		end = line.find(" ", begin);
        if (end == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
		setRequestTarget(line.substr(begin, end - begin));

		begin = end + 1;
		begin = line.find("/", begin);
		if (begin == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
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
			addHeaderLine(line);
			begin += line.size() + 2;
			end = str.find("\r\n", begin);
			if (end == std::string::npos) {
                throw HW::ArgumentError{ "Invalid string passed!" };
            }
			line = str.substr(begin, end - begin);
		}

		begin += 2;
		setBody(str.substr(begin, str.size() - begin));
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

    HTTPResponse::HTTPResponse() : m_statusCode{StatusCode::NONE} {}

    std::string HTTPResponse::toString() const {
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
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
		begin += 1;
		end = line.find(" ");
		if (end == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
		setHTTPversion(line.substr(begin, end - begin));

		begin = end + 1;
		end = line.find(" ", begin);
		if (end == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
		setStatusCode(static_cast<StatusCode>(std::stoi(line.substr(begin, end - begin))));

		begin = line.size() + 2;
		end = str.find("\r\n", begin);
		if (end == std::string::npos) {
            throw HW::ArgumentError{ "Invalid string passed!" };
        }
		line = str.substr(begin, end - begin);
		while (line.size() != 0) {
			addHeaderLine(line);
			begin += line.size() + 2;
			end = str.find("\r\n", begin);
			if (end == std::string::npos) {
                throw HW::ArgumentError{ "Invalid string passed!" };
            }
			line = str.substr(begin, end - begin);
		}

		begin += 2;
		setBody(str.substr(begin, str.size() - begin));
	}

    void HTTPResponse::setStatusCode(const StatusCode code) {
        m_statusCode = code;
    }

    StatusCode HTTPResponse::getStatusCode() const {
        return m_statusCode;
    }

} // HW::HTTP