#include "Exceptions.h"

HW::BaseException::BaseException(const std::string _info) : info{_info} {}

const char * HW::BaseException::what() const noexcept {
    return info.c_str();
}


HW::ProcessError::ProcessError(const std::string _info) : BaseException{_info} {}


HW::DescriptorError::DescriptorError(const std::string _info) : BaseException{_info} {}


HW::IOError::IOError(const std::string _info) : BaseException{_info} {}