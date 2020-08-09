# AdvCpp Mail.ru Courses Homework, Spring 2020  
  
Repository contains homework projects from Mail.ru Advanced C++ Technopark courses from spring 2020.  
Most of the projects are made for OS Linux.  
Each project provides static libraries you can compile and use in your own code.
  
Description of each project provided below. Compilation instruction is provided below.  
Every project has an example/ folder with showcase of how each part works 
  
# hw1-Process
Class HW::Process - wrapper for syscalls fork and exec for executing files  
Class provides IPC with child process using two pipes. Read and write ends of pipes for child are stdin and stdout  
Class guarantees Exception safety and uses RAII idiom  
Class is made for use on OS Linux  
  
# hw2-Logger
Class HW::Logger for logging into stdout, stderr or in file  
Logger provides logging levels in enum HW::Level  
Each level output can be formatted using predefined constants in HW::Format::Color and HW::Format::LogFormat class  
Class Logger uses Singleton pattern  
  
# hw3-Basic_TCP_Server
Simple classes HW::Socket, HW::Connection and HW::Server for establishing internet connection and data transmission using TCP protocol  
Sockets used in classes are blocking  
Classes guarantee Exception safety and uses RAII idiom  
Classes are made for use on OS Linux  
  
# hw4-Async_TCP_Server
Classes HW::SocketAsync, HW::ConnectionAsync и HW::ServerAsync for establishing internet connection and data transmission using TCP protocol  
Sockets used in classes are non-blocking. ServerAsync uses epoll  
Classes guarantee Exception safety and uses RAII idiom  
Classes are made for use on OS Linux  
  
# hw5-Shared_Memory_IPC
Class HW::shmem::SharedMemory provides IPC by creating shared memory block using mmap  
Class HW::shmem::Map is a sorted associative container that creates SharedMemory and places std::map in it  
Map can be used with primitive types and std::string. Map can be used by forked processes  
Operations with map are protected with semaphore  
Map puts its entries into shared memory using custom linear allocator  
Classes guarantee Exception safety and uses RAII idiom  
Classes are made for use on OS Linux  
  
# hw6-HTTP_Server_Framework
Framework provides classes HW::HTTP::HTTPRequest, HW::HTTP::HTTPResponse and HW::HTTP::BaseHTTPServer class that can be used for quickly setting up HTTP server  
Server uses non-blocking connections and works on multithreaded epoll. Server is protected from thundering herd problem  
Each working thread works with its own epoll. Each connection is handled by it's own coroutine  
If provided with timeout value in seconds, server can drop idle connections after accepted connection doesn't act for specified time  
  
```c++
#include "HTTP/BaseHTTPServer.hpp"

using namespace HW::HTTP;

class Server : public BaseHTTPServer {
public:
    Server(size_t numThreads) 
    : BaseHTTPServer{numThreads} {}
    
    HTTPResponse onRequest(const HTTPRequest &r) {
        ... handle request and create response ...
        return HTTPResponse{};
    }
};

int main() {
    // Can be used for logging - HW::Logger::get_instance().set_global_logger(HW::create_stderr_logger(HW::Level::ALL));
    Server s(4);               // Create server with 4 working threads
    s.open("127.1.1.1", 8888); // Opens server with address 127.1.1.1:8888
    s.listen(1000);            // Server can now accept clients (queue size is set to 1000)
    s.run(5000);               // Server is running (epoll wait timeout is set to 5 seconds)
    return 0;
}
```
Server uses singleton logger from hw2  
Classes guarantee Exception safety and uses RAII idiom  
Classes are made for use on OS Linux  
// TODO: Replace coroutines with std coroutines after C++20 release  
  
# hw7-Shared_Memory_File
Classes HW::File::Reader and HW::File::LRUCacheReader that can memory map big binary files filled with  
Entries { uint64_t key; Data data; }  
Readers can quickly give pointer to Entry in file with requested key  
LRUCacheReader contains LRU Cache to access cached Entries at O(1)  
Classes guarantee Exception safety and uses RAII idiom  
Classes are made for use on OS Linux  
  
# Installation and compiling
```
# Clone repo to your PC
git clone https://github.com/sltn011/AdvCpp-HW.git  
cd AdvCpp-HW/

# Choose wanted project folder
cd hw../

# Create Makefile with CMake
cmake CMakeLists.txt

# Compile libraries and executables
make
```
