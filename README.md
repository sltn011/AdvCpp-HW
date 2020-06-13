# AdvCpp Mail.ru Courses Homework, Spring 2020
  
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
  
For setting server up:  
    1) Create server class that inherits BaseHTTPServer  
    2) Pass number of working threads to BaseHTTPServer constructor  
    3) Overload HTTPResponse BaseHTTPServer::onRequest(const HTTPRequest &r) (namespace HW::HTTP::)  
    4) Create your server object  
    5) Call open method of your server with IPv4 address and port number parameters.  
    6) Set server socket to listening state with listen method and size of pending connections queue  
    7) Call run method of server with epoll timeout parameter in milliseconds  
  
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
