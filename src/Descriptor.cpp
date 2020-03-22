#include "Descriptor.h"

HW::Descriptor::Data::Data(const int fd) : opened_{true}, fd_{fd} {}

HW::Descriptor::Data::~Data() noexcept {
    ::close(fd_);
}



HW::Descriptor::Descriptor() : pData{nullptr} {}

HW::Descriptor::Descriptor(const int fd) : pData{nullptr} {
    pData = std::make_shared<Data>(fd);
}

HW::Descriptor::Descriptor(const Descriptor & rhs) {
    pData = rhs.pData;
}

HW::Descriptor & HW::Descriptor::operator=(const Descriptor &rhs) {
    pData = rhs.pData;
    return *this;
}

void HW::Descriptor::setFD(int fd) {
    pData = std::make_shared<Data>(fd);
}
        
void HW::Descriptor::close() {
    if (pData->opened_) {
        if(::close(pData->fd_) < 0) {
            throw HW::DescriptorError("Failure closing descriptor");
        }
        pData->opened_ = false;
    }
}
        
int HW::Descriptor::getFD() const {
    if (pData) {
        return pData->fd_;
    } else {
        return -1;
    }
}
bool HW::Descriptor::isOpened() const {
    if (pData) {
        return pData->opened_;
    } else {
        return false;
    }
}