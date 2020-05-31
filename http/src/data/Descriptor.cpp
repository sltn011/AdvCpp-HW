#include "data/Descriptor.h"

namespace HW {

	Descriptor::Descriptor() : m_fd{-1} {}

	Descriptor::Descriptor(const int fd) : m_fd{fd} {}

	Descriptor::~Descriptor() {
		try {
			close();
		}
		catch (HW::DescriptorError &e) {
			std::cerr << e.what() << std::endl;
		}
	}

	void Descriptor::setFD(int fd) {
		close();
		m_fd = fd;
	}

	void Descriptor::close() {
		if (!isOpened()) {
			return;
		}
		while (::close(m_fd) < 0) {
            if (errno != EINTR) {
               throw HW::DescriptorError("Error closing socket!");
           }
        }
		m_fd = -1;
	}

	int Descriptor::getFD() const {
		return m_fd;
	}
	
	bool Descriptor::isOpened() const {
		return m_fd != -1;
	}

} // HW

