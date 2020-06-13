#ifndef HW_PROCESS_H
#define HW_PROCESS_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include "Descriptor.hpp"
#include "Exceptions.hpp"

namespace HW {

    class Process
    {
    private:
        pid_t       m_pid;
        std::string m_path;
        Descriptor  m_fdIn;
        Descriptor  m_fdOut;

    public:
        Process(const std::string &procPath, std::vector<std::string> &argv);
        ~Process() noexcept;
        size_t write(const void *data, size_t len);
        void writeExact(const void *data, size_t len);
        size_t read(void *data, size_t len);
        void readExact(void *data, size_t len);
        bool isReadable() const;
        bool isWritable() const;
        void closeStdin();
        void close();
        void abort();
        int getPID() const;
        std::string procName() const;
    };

} // HW



#endif // HW_PROCESS_H