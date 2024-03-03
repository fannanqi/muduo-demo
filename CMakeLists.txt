cmake_minimum_required(VERSION 3.15)
project(muduo)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 11)
set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
include_directories(
    ${PROJECT_SOURCE_DIR}/muduo-tool/include
    ${PROJECT_SOURCE_DIR}/net/include
)
link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)
add_subdirectory(muduo-tool)
add_subdirectory(testmuduo)
add_subdirectory(net)