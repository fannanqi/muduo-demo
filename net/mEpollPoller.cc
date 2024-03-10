/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-08 13:53:09
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-10 22:10:28
 * @FilePath: /muduo-demo/net/mEpollPoller.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "mEpollPoller.h"
#include <mEventLoop.h>
#include <mchannel.h>
#include <logger.h>
#include <errno.h>
#include <unistd.h>

using namespace mmuduo;
using namespace mmuduo::mnet;
using namespace std;
namespace
{
    const int kNew = -1;    //  某单个channel还没在poller中添加 channel的成员_index=-1
    const int kAdded = 1;   //  某单个channel已经在poller中添加
    const int kdeleted = 1; //  某单个channel已经在poller中删除
}
#ifdef __linux__
mEpollPoller::mEpollPoller(mEventLoop *loop)
    : mPoll(loop),
      _pollfd(::epoll_create1(EPOLL_CLOEXEC)),
      events_(KInitEventListSize)

{
    if (_pollfd < 0)
    {
        LOG_FATAL("epoll_create1 error:%s", ::strerror(errno));
    }
}
#elif __APPLE__
mEpollPoller::mEpollPoller(mEventLoop *loop)
    : mPoll(loop),
      _epollfd(::kqueue()),
      events_(KInitEventListSize)
{
}
#else
mEpollPoller::mEpollPoller(mEventLoop *loop)
    : mPoll(loop),
      _epollfd(::epoll_create1(EPOLL_CLOEXEC)),
      events_(KInitEventListSize)
{
}
#endif
void mEpollPoller::update(int operation, mChannel *channel)
{
#ifdef __linux__
    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = channel->events();
    //  event.data.ptr存储了fd对应的channel(channel里面包含了事件)
    event.data.ptr = channel;
    int fd = channel->fd();
    //  epoll_ctl failed
    if (::epoll_ctl(_pollfd, operation, fd, &event) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            LOG_ERR("epoll_ctl del error:%s", strerror(errno));
        }
        //  epoll_ctl add mod failed
        else
        {
            LOG_FATAL("epoll_ctl add、mod error:%s", strerror(errno));
        }
    }
#elif __APPLE__
    kevent event;
    memset(&event, 0, sizeof(event));
    event
#endif
}
Timestamp mEpollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    return std::nullptr;
}
//  channel update remove =>EventLoop updateChannel removeChannel
void mEpollPoller::updateChannel(mChannel *channel)
{
    const int index = channel->index();
    const int fd = channel->fd();
    LOG_INFO("fd=%d, events=%d, index=%d", fd, channel->events(), index);
    if (index == kNew || index == kdeleted)
    {
        //  channel还没有在poller上进行添加操作
        if (index == kNew)
        {
            int _curfd = fd;
            _channels.at(_curfd) = channel;
        }
        channel->setIndex(kAdded);
#ifdef __linux__
        update(EPOLL_CTL_ADD, channel);
#elif __APPLE__
        update(EV_ADD | EV_ENABLE, channel);
#else
        update(EPOLL_CTL_ADD, channel);
#endif
    }
    //  channel已经在poller上添加过了
    else
    {
        //  channel删除文件操作
        if (channel->isNoneEvent())
        {
#ifdef __linux__
            update(EPOLL_CTL_DEL, channel);
#elif __APPLE__
            update(EV_DELETE, channel);
#else
            update(EPOLL_CTL_DEL, channel);
#endif
            channel->setIndex(kdeleted);
        }
        //  更改channel上的操作方式
        else
        {
#ifdef __linux__
            update(EPOLL_CTL_MOD, channel);
#elif __APPLE__
            //  这里需要做判断，认定为更改操作
            update(EV_DELETE | EV_DISABLE, channel);
#else
            update(EPOLL_CTL_DEL, channel);
#endif
        }
    }
}
void mEpollPoller::removeChannel(mChannel *channel)
{
}
bool mEpollPoller::hasChannel(mChannel *channel) const
{
    return false;
}
mEpollPoller::~mEpollPoller()
{
    ::close(_pollfd);
}
