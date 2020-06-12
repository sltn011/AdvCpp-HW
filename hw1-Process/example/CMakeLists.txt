# Исполняемые файлы
add_executable(test_Process test_Process.cpp)
add_executable(func func.cpp)

# Связать цель и библиотеку
target_link_libraries(test_Process Process)
target_link_libraries(test_Process Exceptions)
target_link_libraries(test_Process Descriptor)