#ifndef HW_HTTP_MESSAGE_H
#define HW_HTTP_MESSAGE_H

#include "HTTP/HTTPMessageData.h"
#include <string>
#include <map>
#include "exceptions/Exceptions.h"

namespace HW::HTTP {

    class HTTPMessage {
    protected:
        std::string                         m_version;
        std::map<std::string, std::string>  m_headers;
        std::string                         m_body;

    public:
        virtual std::string toString() = 0;
        virtual void fromString(const std::string &str) = 0;

        void clear();

        void setHTTPversion(const std::string &version);
        std::string getHTTPversion() const;
        void clearHTTPversion();

        void addHeaderLine(const std::string &header, const std::string &info);
        std::string getHeaders() const;
        void removeHeaderLine(const std::string &header);
        void clearHeaders();

        void setBody(const std::string &body);
        std::string getBody() const;
        void clearBody();
    };

    class HTTPRequest : public HTTPMessage {
    protected:
        std::string m_method;
        std::string m_reqTarget;

    public:
        HTTPRequest();
        HTTPRequest(const std::string &request);
        std::string toString() override;
        void fromString(const std::string &str) override;

        void setHTTPmethod(const std::string &method);
        std::string getHTTPmethod() const;

        void setRequestTarget(const std::string &reqTarget);
        std::string getRequestTarget() const;

        bool doKeepAlive() const;
    };

    class HTTPResponse : public HTTPMessage {
    protected:
        StatusCode  m_statusCode;

    public:
        HTTPResponse();
        HTTPResponse(const std::string &response);
        std::string toString() override;
        void fromString(const std::string &str) override;

        void setStatusCode(const StatusCode code);
        StatusCode getStatusCode() const;
    };

    using ConnectionBuffer =  std::vector<uint8_t>;
    HTTPRequest tryReadRequest(const ConnectionBuffer &buffer);
    HTTPResponse tryReadResponse(const ConnectionBuffer &buffer);

} // HW::HTTP

#endif // HW_HTTP_MESSAGE_H