# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10.2)

# Стандарт С++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Название проекта
project(tcp)

# Добавить библиотеку
add_library(tcp STATIC src/Socket.cpp src/Connection.cpp src/Server.cpp src/Exceptions.cpp)

# Добавление заголовочных файлов
target_include_directories(tcp PUBLIC include)

# Добавление поддиректории со своим CMakeLists.txt
add_subdirectory(example) 