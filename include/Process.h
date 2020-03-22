#ifndef HW_PROCESS_H
#define HW_PROCESS_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>
#include "Descriptor.h"
#include "Exceptions.h"

namespace HW {

    class Process
    {
    private:
        // Идентификатор процесса
        pid_t pid;

        // Расположение вызванного процесса в системе
        std::string path;

        // Дескриптор для передачи данных в процесс
        Descriptor fd_in;

        // Дескриптор для получения данных из процесса
        Descriptor fd_out;

    public:
        Process(const std::string& procPath, std::vector<std::string> & argv);
        ~Process() noexcept;

        // Передача данных в процесс
        size_t write(const void* data, size_t len);
        void writeExact(const void* data, size_t len);
    
        // Считывание данных из процесса
        size_t read(void* data, size_t len);
        void readExact(void* data, size_t len);
    
        // Открыт ли поток вывода из процесса
        bool isReadable() const noexcept;

        // Открыт ли поток передачи в процесс
        bool isWritable() const noexcept;

        // Закрытие потока передачи в процесс
        void closeStdin();

        // Закрытие потоков обмена данными с процессом
        void close();

        // Завершение процесса 
        void abort();

        // PID процесса
        int getPID() const noexcept;

        // Имя процесса в системе
        std::string procName() const noexcept;
    };

} // HW



#endif // HW_PROCESS_H