#include "HTTP/HTTPMessage.h"
#include <iostream>

int main() {
    HW::HTTP::HTTPRequest request;
    std::string s = 
    "GET /rootDesc.xml HTTP/1.1\r\n"
    "Cache-Control: no-cache\r\n"
    "Connection: Close\r\n"
    "Pragma: no-cache\r\n"
    "Accept: text/xml, application/xml\r\n"
    "Host: 192.168.1.1:49280\r\n"
    "\r\n";
    request.fromString(s);
    std::cout << request.toString() << "\n\n" << std::endl;
    std::cout << std::boolalpha << (s == request.toString()) << std::endl;
}