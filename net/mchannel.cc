/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 17:46:48
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-05 16:40:37
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

void mmuduo::mnet::mChannel::tie(const std::shared_ptr<void> &obj)
{
    _tie = obj;
    _tied = true;
}

mChannel::~mChannel()
{
}

//  因为EeventLoop(作为事件分发器，添加event事件)=>(包含)Channel,所以需要_loop指针对事件的增删
void mmuduo::mnet::mChannel::update()
{
}
