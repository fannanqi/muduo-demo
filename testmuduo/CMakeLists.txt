include_directories(./)
add_executable(muduo-demo muduo-server.cc)
target_link_libraries(muduo-demo  muduo_net muduo_base pthread)