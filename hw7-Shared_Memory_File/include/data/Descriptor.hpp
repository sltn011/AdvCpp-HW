#ifndef HW_DISCRIPTOR_H
#define HW_DISCRIPTOR_H

#include <sys/unistd.h>
#include "exceptions/Exceptions.hpp"
#include <iostream>

namespace HW {

    class Descriptor 
    {
    private:
        int m_fd;

    public:
        Descriptor();
        Descriptor(const int fd);
        ~Descriptor();
        Descriptor(const Descriptor &rhs) = delete;
        Descriptor &operator=(const Descriptor &rhs) = delete;
        Descriptor(Descriptor &&rhs) = default;

        void setFD(int fd);
        void close();
        int getFD() const;
        bool isOpened() const;
    };

}

#endif // HW_DISCRIPTOR_H