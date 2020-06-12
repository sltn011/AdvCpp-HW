#ifndef HW_EXCEPTIONS_H
#define HW_EXCEPTIONS_H

#include <exception>
#include <string>

namespace HW {

    class BaseException : public std::exception 
    {
    private:
        std::string m_info;

    public:
        BaseException(const std::string &info);
        const char *what() const noexcept override;
    };

	class ArgumentError : public BaseException
	{
	public:
		ArgumentError(const std::string &info);
	};

    class DescriptorError : public BaseException 
    {
    public:
        DescriptorError(const std::string &info);
    };

    class ProcessError : public BaseException 
    {
    public:
        ProcessError(const std::string &info);
    };

    class IOError : public BaseException 
    {
    public:
        IOError(const std::string &info);
    };

    class NetworkError : public BaseException 
    {
    public:
        NetworkError(const std::string &info);
    };

    class IPCError : public BaseException
    {
    public:
        IPCError(const std::string &info);
    };

} // HW

#endif // HW_EXCEPTIONS_H