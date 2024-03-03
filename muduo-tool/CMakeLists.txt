set(SOURCES
    ./log/logger.cc
    ./timestamp/mTimestamp.cc
    
)


add_subdirectory(log)
add_subdirectory(test)
add_subdirectory(timestamp)
add_library(muduo-tool SHARED ${SOURCES})
target_link_libraries(muduo-tool  pthread)