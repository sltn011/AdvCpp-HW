#ifndef HW_EXCEPTIONS_H
#define HW_EXCEPTIONS_H

#include <exception>
#include <string>

namespace HW {

    class BaseException : public std::exception 
    {
    protected:
        std::string info;

    public:
        BaseException(const std::string _info);
        const char * what() const noexcept;
    };

    class DescriptorError : public BaseException 
    {
    public:
        DescriptorError(const std::string _info);
        const char * what() const noexcept;
    };

    class ProcessError : public BaseException 
    {
    public:
        ProcessError(const std::string _info);
        const char * what() const noexcept;
    };

    class IOError : public BaseException 
    {
    public:
        IOError(const std::string _info);
        const char * what() const noexcept;
    };

}

#endif // HW_EXCEPTIONS_H