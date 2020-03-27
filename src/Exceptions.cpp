#include "Exceptions.h"

namespace HW {

	BaseException::BaseException(const std::string _info) : info{ _info } {}

	const char * BaseException::what() const noexcept {
		return info.c_str();
	}


	ProcessError::ProcessError(const std::string _info) : BaseException{ _info } {}


	DescriptorError::DescriptorError(const std::string _info) : BaseException{ _info } {}


	IOError::IOError(const std::string _info) : BaseException{ _info } {}

} // HW

