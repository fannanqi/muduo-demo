/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 17:46:48
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-06 20:20:14
 * @FilePath: /muduo-demo/net/mchannel.cc
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "mchannel.h"
#include <mEventLoop.h>
#include <mTimestamp.h>
#include <logger.h>
#ifdef __unix__
#include <sys/epoll.h>
#elif __linux__
#include <sys/epoll.h>
#elif __APPLE__
#include <sys/event.h>
#include <sys/types.h>
#endif

using namespace mmuduo;
using namespace mmuduo::mnet;

mChannel::mChannel(mEventLoop *loop, int fd)
    : _loop(loop),
      _fd(fd),
      _events(0),
      _revents(0),
      _index(-1),
      _tied(false)
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
//  channel的tie方法什么时候调用过
void mChannel::tie(const std::shared_ptr<void> &obj)
{
    _tie = obj;
    _tied = true;
}

void mChannel::remove()
{
    //_loop->removeChannel(this);
}

mChannel::~mChannel()
{
}

//  因为EeventLoop(作为事件分发器，添加event事件)=>(包含)Channel,所以需要_loop指针对事件的增删
void mChannel::update()
{
    //_loop->updateChannel(this);
}

void mChannel::handleEvent(Timestamp receiveTime)
{
    if (_tied)
    {
        std::shared_ptr<void> guard = _tie.lock();
        if (guard)
        {
            handleEventWithGuard(receiveTime);
        }
    }
}
//  根据poller通知channel发生的具体事件，由channel负责调用具体的回调操作
void mChannel::handleEventWithGuard(Timestamp receiveTime)
{
    LOG_INFO("channel handleEvent revents:%d", _revents);
#ifdef __linux__
    //  EPOLLHUP表示读和写都关闭状态, 对端读关闭
    if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
    {
        //  断开连接
        if (_closeCallback)
            _closeCallback();
    }
    //  epoll错误事件
    if (_revents & EPOLLERR)
    {
        if (_errorCallback)
            _errorCallback();
    }
    if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (_readCallback)
            _readCallback(receiveTime);
    }
    if (_revents & EPOLLOUT)
    {
        if (_writeCallback)
            _writeCallback();
    }
    _eventHandling = false;
#elif __ubuntu__
    //  EPOLLHUP表示读和写都关闭状态, 对端读关闭
    if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
    {
        //  断开连接
        if (_closeCallback)
            _closeCallback();
    }
    //  epoll错误事件
    if (_revents & EPOLLERR)
    {
        if (_errorCallback)
            _errorCallback();
    }
    if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (_readCallback)
            _readCallback(receiveTime);
    }
    if (_revents & EPOLLOUT)
    {
        if (_writeCallback)
            _writeCallback()
    }
    _eventHandling = false;
#elif __APPLE__
    if (_revents & (!EVFILT_READ))
        //  断开连接
        if (_closeCallback)
            _closeCallback();
    if (_revents & EV_ERROR)
        if (_closeCallback)
            _closeCallback();
    if (_revents & EVFILT_READ)
    {
        if (_readCallback)
            _readCallback(receiveTime);
    }
    if (_revents & EVFILT_WRITE)
    {
        if (_writeCallback)
            _writeCallback();
    }
    _eventHandling = false;
#endif
}
