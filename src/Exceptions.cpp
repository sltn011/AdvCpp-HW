#include "Exceptions.h"

HW::BaseException::BaseException(const std::string _info) : info{_info} {}

const char * HW::BaseException::what() const noexcept {
    return info.c_str();
}



HW::ProcessError::ProcessError(const std::string _info) : BaseException{_info} {}

const char * HW::ProcessError::what() const noexcept {
    return info.c_str();
}



HW::DescriptorError::DescriptorError(const std::string _info) : BaseException{_info} {}

const char * HW::DescriptorError::what() const noexcept {
    return info.c_str();
}



HW::IOError::IOError(const std::string _info) : BaseException{_info} {}

const char * HW::IOError::what() const noexcept {
    return info.c_str();
}