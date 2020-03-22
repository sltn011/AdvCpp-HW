#ifndef HW_DISCRIPTOR_H
#define HW_DISCRIPTOR_H

#include <sys/unistd.h>
#include <memory>
#include "Exceptions.h"

namespace HW {

    class Descriptor 
    {
    private:

        struct Data {
        public:
            bool opened_;
            int fd_;

            Data(const int fd);
            ~Data() noexcept;
        };

        std::shared_ptr<Data> pData;

    public:
        Descriptor();
        Descriptor(const int fd);
        Descriptor(const Descriptor &rhs);
        Descriptor &operator=(const Descriptor &rhs);

        void setFD(int fd);
        void close();
        int getFD() const;
        bool isOpened() const;
    };

}

#endif // HW_DISCRIPTOR_H