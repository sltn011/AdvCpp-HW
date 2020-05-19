#ifndef HW_HTTP_MESSAGE_H
#define HW_HTTP_MESSAGE_H

#include "HTTP/HTTPMessageData.h"
#include <string>
#include <vector>
#include "exceptions/Exceptions.h"

namespace HW::HTTP {

    class HTTPMessage {
    protected:
        std::string                 m_version;
        std::vector<std::string>    m_headers;
        std::string                 m_body;

    public:
        virtual std::string toString() const = 0;
		virtual void fromString(const std::string &str) = 0;

        void clear();

        void setHTTPversion(const std::string &version);
        std::string getHTTPversion() const;
        void clearHTTPversion();

        void addHeaderLine(const std::string &line);
        std::string getHeaders() const;
        void removeHeaderLine(size_t line);
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
        std::string toString() const override;
		void fromString(const std::string &str) override;

        void setHTTPmethod(const std::string &method);
        std::string getHTTPmethod() const;

        void setRequestTarget(const std::string &reqTarget);
        std::string getRequestTarget() const;
    };

    class HTTPResponse : public HTTPMessage {
    protected:
        StatusCode  m_statusCode;

    public:
        HTTPResponse();
        std::string toString() const override;
		void fromString(const std::string &str) override;

        void setStatusCode(const StatusCode code);
        StatusCode getStatusCode() const;
    };

} // HW::HTTP

#endif // HW_HTTP_MESSAGE_H