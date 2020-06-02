#ifndef HW_FILE_MMAP_H
#define HW_FILE_MMAP_H

#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <string>
#include "data/Descriptor.h"
#include "exceptions/Exceptions.h"

namespace HW::File{

    class Mmap {
    private:
        void *m_data;
        size_t m_size;

    public:
        Mmap(std::string &file, bool isShared);
        ~Mmap();
        void *getData() const;
        size_t getSize() const;
    };

} // HW::File

#endif // HW_FILE_MMAP_H 