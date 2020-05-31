#ifndef HW_HTTP_MESSAGE_DATA_H
#define HW_HTTP_MESSAGE_DATA_H

#include <string>
#include <vector>

namespace HW::HTTP {

    namespace Protocol{

        inline const std::string HTTP = "HTTP";

    } // Protocol

    namespace Version {

        inline const std::string v0_9 = "0.9";
        inline const std::string v1_0 = "1.0";
        inline const std::string v1_1 = "1.1";

    } // Version

    namespace Method {

        inline const std::string OPTIONS    = "OPTIONS";
        inline const std::string GET        = "GET";
        inline const std::string HEAD       = "HEAD";
        inline const std::string POST       = "POST";
        inline const std::string PUT        = "PUT";
        inline const std::string PATCH      = "PATCH";
        inline const std::string DELETE     = "DELETE";
        inline const std::string TRACE      = "TRACE";
        inline const std::string CONNECT    = "CONNECT";

    } // Method

    enum class StatusCode {
        NONE                            = 0,

        Continue                        = 100,
        SwitchingProtocol               = 101,
        Processing                      = 102,

        OK                              = 200,
        Created                         = 201,
        Accepted                        = 202,
        NonAuthorativeInformation       = 203,
        NoContent                       = 204,
        ResetContent                    = 205,
        PartialContent                  = 206,
        MultiStatus                     = 207,
        AlreadyReported                 = 208,
        IMUsed                          = 226,

        MultipleChoices                 = 300,
        MovedPermanently                = 301,
        Found                           = 302,
        SeeOther                        = 303,
        NotModified                     = 304,
        UseProxy                        = 305,
        UNUSED                          = 306,
        TemporaryRedirect               = 307,
        PermanentRedirect               = 308,

        BadRequest                      = 400,              
        Unauthorized                    = 401, 
        PaymentRequired                 = 402, 
        Forbidden                       = 403, 
        NotFound                        = 404, 
        MethodNotAllowed                = 405, 
        NotAcceptable                   = 406, 
        ProxyAuthenticationRequired     = 407, 
        RequestTimeout                  = 408, 
        Conflict                        = 409, 
        Gone                            = 410, 
        LengthRequired                  = 411, 
        PreconditionFailed              = 412, 
        PayloadTooLarge                 = 413, 
        URITooLong                      = 414, 
        UnsupportedMediaType            = 415,  
        RequestedRangeNotSatisfiable    = 416, 
        ExpectationFailed               = 417,        
        ImATeapot                       = 418,
        AuthenticationTimeout           = 419,
        MisdirectedRequest              = 421, 
        UnprocessableEntity             = 422, 
        Locked                          = 423, 
        FailedDependency                = 424, 
        UpgradeRequired                 = 426, 
        PreconditionRequired            = 428, 
        TooManyRequests                 = 429, 
        RequestHeaderFieldsTooLarge     = 431, 
        ConnectionClosedWithoutResponse = 444,
        RetryWith                       = 449,  
        UnavailableForLegalReasons      = 451,   
        ClientClosedRequest             = 499,

        InternalServerError             = 500,
        NotImplemented                  = 501,
        BadGateway                      = 502,
        ServiceUnavailable              = 503,
        GatewayTimeout                  = 504,
        HTTPVersionNotSupported         = 505,
        VariantAlsoNegotiates           = 506,
        InsufficientStorage             = 507,
        LoopDetected                    = 508,
        BandwidthLimitExceeded          = 509,
        NotExtended                     = 510,
        NetworkAuthenticationRequired   = 511,
        UnknownError                    = 520,
        WebServerIsDown                 = 521,
        ConnectionTimedOut              = 522,
        OriginIsUnreachable             = 523,
        ATimeoutOccured                 = 524,
        SSLHadnshakeFailed              = 525,
        NetworkConnectTimeoutError      = 599
    };

	std::string getCodeMessage(const StatusCode code);

    namespace Request {

        inline const std::string AIM                            = "A-IM";
        inline const std::string AcceptCharset                  = "Accept-Charset";
        inline const std::string AcceptDatetime                 = "Accept-DateTime";
        inline const std::string AcceptEncoding                 = "Accept-Encoding";
        inline const std::string AcceptLanguage                 = "Accept-Language";
        inline const std::string AcceptControlRequestMethod     = "Accept-Control-Request-Method";
        inline const std::string AcceptControlRequestHeaders    = "Accept-Control-Request-Headers";
        inline const std::string Authorization                  = "Authorization";
        inline const std::string CacheControl                   = "Cache-Control";
        inline const std::string Connection                     = "Connection";
        inline const std::string ContentEncoding                = "Content-Encoding";
        inline const std::string ContentLength                  = "Content-Length";
        inline const std::string ContentMD5                     = "Content-MD5";
        inline const std::string ContentType                    = "Content-Type";
        inline const std::string Cookie                         = "Cookie";
        inline const std::string Date                           = "Date";
        inline const std::string Expect                         = "Expect";
        inline const std::string Forwarded                      = "Forwarded";
        inline const std::string From                           = "From";
        inline const std::string Host                           = "Host";
        inline const std::string HTTP2Settings                  = "HTTP2-Settings";
        inline const std::string IfMatch                        = "If-Match";
        inline const std::string IfModifiedSince                = "If-Modified-Since";
        inline const std::string IfNoneMatch                    = "If-None-Match";
        inline const std::string IfRange                        = "IfRange";
        inline const std::string IfUnmodifiedSince              = "If-Unmodified-Since";
        inline const std::string MaxForward                     = "Max-Forward";
        inline const std::string Origin                         = "Origin";
        inline const std::string Pragma                         = "Pragma";
        inline const std::string ProxyAuthorization             = "Proxy-Authorization";
        inline const std::string Range                          = "Range";
        inline const std::string Referer                        = "Referer";
        inline const std::string TE                             = "TE";
        inline const std::string Trailer                        = "Trailer";
        inline const std::string TransferEncoding               = "Transfer-Encoding";
        inline const std::string UserAgent                      = "User-Agent";
        inline const std::string Upgrade                        = "Upgrade";
        inline const std::string Via                            = "Via";
        inline const std::string Warning                        = "Warning";
        inline const std::string UpgradeInsecureRequests        = "Upgrade-Insecure-Requests";
        inline const std::string XRequestedWith                 = "X-Requested-With";
        inline const std::string DNT                            = "DNT";
        inline const std::string XForwardedFor                  = "X-Forwarded-For";
        inline const std::string XForwardedHost                 = "X-Forwarded-Host";
        inline const std::string XForwardedProto                = "X-Forwarded-Proto";
        inline const std::string FrontEndHttps                  = "Front-End-Https";
        inline const std::string XHttpMethodOverride            = "X-Http-Method-Override";
        inline const std::string XATTDeviced                    = "X-ATT-Deviced";
        inline const std::string XWapProfile                    = "X-Wap-Profile";
        inline const std::string ProxyConnection                = "Proxy-Connection";
        inline const std::string XUIDH                          = "X-UIDH";
        inline const std::string XCsrfToken                     = "X-Csrf-Token";
        inline const std::string XRequestID                     = "X-Request-ID";
        inline const std::string XCorrelationID                 = "X-Correlation-ID";
        inline const std::string SaveData                       = "Save-Data";

    } // Request

    namespace Response {

        inline const std::string AccessControlAllowOrigin           = "Access-Control-Allow-Origin";
        inline const std::string AccessControlAllowCredentials      = "Access-Control-AllowCredentials";
        inline const std::string AccessControlExposeHeaders         = "Access-Control-Expose-Headers";
        inline const std::string AccessControlMaxAge                = "Access-Control-Max-Age";
        inline const std::string AccessControlAllowMethods          = "Access-Control-Allow-Methods";
        inline const std::string AccessControlAllowHeaders          = "Access-Control-Allow-Headers";
        inline const std::string AcceptPatch                        = "Accept-Patch";
        inline const std::string AcceptRanges                       = "Accept-Ranges";
        inline const std::string Age                                = "Age";
        inline const std::string Allow                              = "Allow";
        inline const std::string AltSvc                             = "Alt-Svc";
        inline const std::string CacheControl                       = "Cache-Control";
        inline const std::string Connection                         = "Connection";
        inline const std::string ContentDesposition                 = "Content-Disposition";
        inline const std::string ContentEncoding                    = "Content-Encoding";
        inline const std::string ContentLanguage                    = "Content-Language";
        inline const std::string ContentLength                      = "Content-Length";
        inline const std::string ContentLocation                    = "Content-Locationg";
        inline const std::string ContentMD5                         = "Content-MD5";
        inline const std::string ContentRange                       = "Content-Range";
        inline const std::string ContentType                        = "Content-Type";
        inline const std::string Date                               = "Date";
        inline const std::string DeltaBase                          = "Delta-Base";
        inline const std::string ETag                               = "ETag";
        inline const std::string Expires                            = "Expires";
        inline const std::string IM                                 = "IM";
        inline const std::string LastModified                       = "Last-Modified";
        inline const std::string Link                               = "Link";
        inline const std::string Location                           = "Location";
        inline const std::string P3P                                = "P3P";
        inline const std::string Pragma                             = "Pragma";
        inline const std::string ProxyAuthenticate                  = "Proxy-Authenticate";
        inline const std::string PublicKeyPins                      = "Public-Key-Pins";
        inline const std::string RetryAfter                         = "Retry-After";
        inline const std::string Server                             = "Server";
        inline const std::string SetCookie                          = "Set-Cookie";
        inline const std::string StrictTransportSecurity            = "Strict-Transport-Security";
        inline const std::string Trailer                            = "Trailer";
        inline const std::string TransferEncoidng                   = "Transfer-Encoding";
        inline const std::string Tk                                 = "Tk";
        inline const std::string Upgrade                            = "Upgrade";
        inline const std::string Vary                               = "Vary";
        inline const std::string Via                                = "Via";
        inline const std::string Warning                            = "Warning";
        inline const std::string WWWAuthenticate                    = "WWW-Authenticate";
        inline const std::string XFrameOptions                      = "X-Frame-Options";
        inline const std::string ContentSecurityPolicy              = "Content-Security-Policy";
        inline const std::string XContentSecurityPolicy             = "X-Content-Security-Policy";
        inline const std::string XWebKitCSP                         = "X-WebKit-CSP";
        inline const std::string Refresh                            = "Refresh";
        inline const std::string Status                             = "Status";
        inline const std::string TimingAllowOrigin                  = "Timing-Allow-Origin";
        inline const std::string XContentDuration                   = "X-Content-Duration";
        inline const std::string XContentTypeOptions                = "X-Content-Type-Options";
        inline const std::string XPoweredBy                         = "X-Powered-By";
        inline const std::string XRequestID                         = "X-Request-ID";
        inline const std::string XCorrelationID                     = "X-Correlation-ID";
        inline const std::string XUACompatible                      = "X-UA-Compatible";
        inline const std::string XXSSProtection                     = "X-XSS-Protection";

    } // Response

} // HTTP

#endif // HW_HTTP_MESSAGE_DATA_H