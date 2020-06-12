#include "Exceptions.h"

namespace HW {

	BaseException::BaseException(const std::string &info) : m_info{ info } {}

	const char *BaseException::what() const noexcept {
		return m_info.c_str();
	}

	ArgumentError::ArgumentError(const std::string &info) : BaseException{ info } {}

	ProcessError::ProcessError(const std::string &info) : BaseException{ info } {}

	DescriptorError::DescriptorError(const std::string &info) : BaseException{ info } {}

	IOError::IOError(const std::string &info) : BaseException{ info } {}

	NetworkError::NetworkError(const std::string &info) : BaseException{ info } {}

	IPCError::IPCError(const std::string &info) : BaseException{ info } {}

} // HW

