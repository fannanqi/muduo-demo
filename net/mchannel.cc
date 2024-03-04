/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 17:46:48
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-04 22:02:01
 * @FilePath: /muduo-demo/net/mchannel.cc
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "mchannel.h"
using namespace mmuduo;
using namespace mnet;
mChannel::mChannel(mEventLoop *loop, int fd)
{
}

void mChannel::setReadCallback(ReadEventCallback cb)
{
    _readCallback = std::move(cb);
}

void mChannel::setWriteCallback(EventCallback cb)
{
    _writeCallback = std::move(cb);
}

void mChannel::setCloseCallback(EventCallback cb)
{
    _closeCallback = std::move(cb);
}

void mChannel::setErrorCallback(EventCallback cb)
{
    _errorCallback = std::move(cb);
}

mChannel::~mChannel()
{
}
