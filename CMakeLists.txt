# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10.2)

# Название проекта
project(Process)

# Добавить библиотеку
add_library(Process STATIC src/Process.cpp)

# Добавление заголовочных файлов
target_include_directories(Process PUBLIC include)

# Добавление поддиректории со своим CMakeLists.txt
add_subdirectory(example)