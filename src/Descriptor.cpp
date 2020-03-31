#include "Descriptor.h"

namespace HW {

	Descriptor::Data::Data(const int fd) : opened_{ true }, fd_{ fd } {}

	Descriptor::Data::~Data() noexcept {
		::close(fd_);
	}


	Descriptor::Descriptor() {}

	Descriptor::Descriptor(const int fd) : pData{ std::make_shared<Data>(fd) } {}

	Descriptor::Descriptor(const Descriptor & rhs) {
		pData = rhs.pData;
	}

	Descriptor & HW::Descriptor::operator=(const Descriptor &rhs) {
		pData = rhs.pData;
		return *this;
	}

	void Descriptor::setFD(int fd) {
		pData = std::make_shared<Data>(fd);
	}

	void Descriptor::close() {
		if (pData == nullptr) {
			return;
		}
		if (pData->opened_) {
			if (::close(pData->fd_) < 0) {
				throw HW::DescriptorError("Failure closing descriptor");
			}
			pData->opened_ = false;
		}
	}

	int Descriptor::getFD() const {
		if (pData) {
			return pData->fd_;
		}
		else {
			return -1;
		}
	}
	bool Descriptor::isOpened() const {
		if (pData) {
			return pData->opened_;
		}
		else {
			return false;
		}
	}

} // HW

