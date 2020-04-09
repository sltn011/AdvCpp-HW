# Исполняемые файлы
add_executable(client_side client_side.cpp)
add_executable(server_side server_side.cpp)

add_compile_options(-g)

# Связать цель и библиотеку
target_link_libraries(client_side tcp)
target_link_libraries(server_side tcp)