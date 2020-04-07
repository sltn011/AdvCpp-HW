# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10.2)

# Стандарт С++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Название проекта
project(log)

# Файлы
add_library(log STATIC src/Loggers.cpp src/log.cpp)
target_include_directories(log PUBLIC include)

add_subdirectory(example)
