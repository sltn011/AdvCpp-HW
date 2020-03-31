# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10.2)

# Стандарт С++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Название проекта
project(Homework)

# Добавить библиотеку
add_library(Process STATIC src/Process.cpp)
add_library(Exceptions STATIC src/Exceptions.cpp)
add_library(Descriptor STATIC src/Descriptor.cpp)

# Добавление заголовочных файлов
target_include_directories(Process PUBLIC include)
target_include_directories(Exceptions PUBLIC include)
target_include_directories(Descriptor PUBLIC include)

# Добавление поддиректории со своим CMakeLists.txt
add_subdirectory(example)