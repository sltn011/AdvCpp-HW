# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10.2)

# Стандарт С++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Название проекта
project(tcp)
project(tcpAsync)

# Добавить библиотеку
add_library(tcp STATIC src/Socket.cpp src/Connection.cpp src/Server.cpp src/Exceptions.cpp)
add_library(tcpAsync STATIC src/SocketAsync.cpp src/ConnectionAsync.cpp src/ServerAsync.cpp src/Exceptions.cpp src/Descriptor.cpp)

# Добавление заголовочных файлов
target_include_directories(tcp PUBLIC include)
target_include_directories(tcpAsync PUBLIC include)

# Добавление поддиректории со своим CMakeLists.txt
add_subdirectory(example)