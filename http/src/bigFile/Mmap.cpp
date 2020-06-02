#include "bigFile/Mmap.h"

namespace HW::File{

    Mmap::Mmap(std::string &file, bool isShared) {
        Descriptor fd(::open(file.c_str(), O_RDONLY));
        struct stat fstat;
        if (::fstat(fd.getFD(), &fstat) < 0) {
            throw DescriptorError("Error getting descriptor status!");
        }
        m_size = fstat.st_size;
        int shared_flag = isShared ? MAP_SHARED : MAP_PRIVATE;
        void *filemmap = ::mmap(
            nullptr,
            m_size,
            PROT_READ,
            shared_flag,
            fd.getFD(),
            0
        );
        if (filemmap == MAP_FAILED) {
            throw DescriptorError("Failed to map file to memory!");
        }
        m_data = filemmap;
    }

    Mmap::~Mmap() {
        ::munmap(m_data, m_size);
    }

    void *Mmap::getData() const {
        return m_data;
    }

    size_t Mmap::getSize() const {
        return m_size;
    }

} // HW::File