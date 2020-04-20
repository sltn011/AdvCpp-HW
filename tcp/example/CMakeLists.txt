# Исполняемые файлы
add_executable(client_side client_side.cpp)
add_executable(server_side server_side.cpp)
add_executable(client_side_async client_side_async.cpp)
add_executable(server_side_async server_side_async.cpp)

# Связать цель и библиотеку
target_link_libraries(client_side tcp)
target_link_libraries(server_side tcp)
target_link_libraries(client_side_async tcp)
target_link_libraries(server_side_async tcpAsync)