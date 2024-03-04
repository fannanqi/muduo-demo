set(MODULE_NET_INCLUDES
    ./include
)
include_directories(
    ${MODULE_NET_INCLUDES}
)
set(SOURCES
    mInetAddress.cc
    mchannel.cc
    mEventLoop.cc
    mTcpServer.cc
)

add_library(net ${SOURCES})
target_link_libraries(net muduo-tool)