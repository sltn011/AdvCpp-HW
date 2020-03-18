#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdint.h>
#include <stdexcept>

class Process
{
private:
    // Идентификатор процесса
    pid_t pid;

    // Расположение вызванного процесса в системе
    std::string path;

    // Дескрипторы для передачи данных в процесс
    int pipe_in[2];

    // Дескрипторы для получения данных из процесса
    int pipe_out[2];

    // Открыт ли поток вывода из процесса
    bool readableFrom;

    // Открыт ли поток передачи в процесс
    bool writeableTo;

public:
    explicit Process(const std::string& path, char *argv[]);
    ~Process();

    // Передача данных в процесс
    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    
    // Считывание данных из процесса
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);
    
    // Открыт ли поток вывода из процесса
    bool isReadable() const;

    // Открыт ли поток передачи в процесс
    bool isWritable() const;

    // Закрытие потока передачи в процесс
    void closeStdin();

    // Закрытие потоков обмена данными с процессом
    void close();

    // Завершение процесса 
    void abort();

    // PID процесса
    int getPID() const;

    // Имя процесса в системе
    std::string procName() const;
};

#endif // PROCESS_H