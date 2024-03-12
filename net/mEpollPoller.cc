/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-08 13:53:09
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-11 18:36:00
 * @FilePath: /muduo-demo/net/mEpollPoller.cc
 * @Description: 这里进行channnel的状态的改变，epoll_ctl、EV_SET操作
 */
#include "mEpollPoller.h"
#include <mEventLoop.h>
#include <mchannel.h>
#include <logger.h>
#include <mTimestamp.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#define MUDEBUG
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
      _pollfd(::kqueue()),
      events_(KInitEventListSize)
{
    if (_pollfd < 0)
    {
        LOG_FATAL("epoll_create1 error:%s", ::strerror(errno));
    }
}
#else
mEpollPoller::mEpollPoller(mEventLoop *loop)
    : mPoll(loop),
      _epollfd(::epoll_create1(EPOLL_CLOEXEC)),
      events_(KInitEventListSize)
{
    if (_pollfd < 0)
    {
        LOG_FATAL("epoll_create1 error:%s", ::strerror(errno));
    }
}
#endif
void mEpollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    for (int i = 0; i < numEvents; i++)
    {
#ifdef __linux__
        mChannel *channel = static_cast<mChannel *>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
#elif __APPLE__
        mChannel *channel = static_cast<mChannel *>(events_[i].udata);
        channel->set_revents(events_[i].filter);
#else
        mChannel *channel = static_cast<mChannel *>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
#endif
        activeChannels->push_back(channel); //   EventLoop拿到poller给他返回所有发生事件的channel
    }
}
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
    if (operation == EPOLL_CTL_MOD)
        LOG_INFO("The channel fd:%d update,the operation is:EPOll_CTL_MOD", fd);
    if (operation == EPOLL_CTL_ADD)
        LOG_INFO("The channel fd:%d update,the operation is:EPOLL_CTL_ADD", fd);
    if (operation == EPOLL_CTL_DEL)
        LOG_INFO("The channel fd:%d update,the operation is:EPOLL_CTL_DEL", fd);

#elif __APPLE__
    struct kevent event;
    memset(&event, 0, sizeof(event));
    /*
    // struct kevent结构体
        struct kevent {
        uintptr_t       ident;//该事件关联的文件描述符，如socket中的fd句柄

        可以指定监听类型
        如EVFILT_READ=读，EVFILT_WRITE=写，EVFILT_TIMER=定时器事件，EVFILT_SIGNAL=信号，EVFILT_USER=用户自定义事件
        int16_t         filter;
        操作方式
        EV_ADD 添加，EV_DELETE 删除，EV_ENABLE 激活，EV_DISABLE 不激活
        uint16_t        flags;
        uint32_t        fflags;         // 第二种操作方式，NOTE_TRIGGER 立即激活等等
        intptr_t        data;           // int 型的用户数据，socket 里面它是可读写的数据长度
        void            *udata;         // 指针类型的数据，你可以携带任何想携带的附加数据。比如对象
    };
        EV_SET(&kev, ident, filter, flags, fflags, data, udata);
        int kevent(int kq, const struct kevent *changelist,
                    int nchanges, struct kevent *eventlist,
                    int nevents, const struct timespec *timeout);
   */
    int fd = channel->fd();
    EV_SET(&event, fd, channel->events(), operation, 0, 0, channel);
    if (operation == (EV_ADD | EV_ENABLE))
        LOG_INFO("The channel fd:%d update,the operation is:EV_ADD|EV_ENABLE", fd);
    if (operation == EV_DISABLE)
        LOG_INFO("The channel fd:%d update,the operation is:EV_DISABLE", fd);
    if (operation == EV_DELETE)
        LOG_INFO("The channel fd:%d update,the operation is:EV_DELETE", fd);
#else
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
    if (operation == EPOll_CTL_MOD)
        LOG_INFO("The channel fd:%d update,the operation is:EPOll_CTL_MOD", fd);
    if (operation == EPOLL_CTL_ADD)
        LOG_INFO("The channel fd:%d update,the operation is:EPOLL_CTL_ADD", fd);
    if (operation == EPOLL_CTL_DEL)
        LOG_INFO("The channel fd:%d update,the operation is:EPOLL_CTL_DEL", fd);
#endif
}
Timestamp mEpollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    LOG_INFO("%s => fd total count:%d", __FUNCTION__, static_cast<int>(_channels.size()));
    int numEvents = 0;
    int savedErrno = errno;
    Timestamp now = Timestamp::now();
#ifdef __linux__
    numEvents = ::epoll_wait(_pollfd, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    switch (numEvents)
    {
    case -1:
        if (savedErrno != EINTR)
        {
            errno = savedErrno;
            LOG_ERR("mEpollPoller::poll error!,the line:%d", __LINE__);
        }
        break;
    case 0:
        LOG_DEBUG("epoll_wait time out!");
        break;
    default:
        LOG_INFO("epoll_wait listen count:%d events happedned", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size())
            events_.resize(2 * events_.size());
        break;
    }
#elif __APPLE__
    struct timespec wait_time;
    wait_time.tv_sec = static_cast<time_t>(timeoutMs / 1000);
    wait_time.tv_nsec = static_cast<long>(timeoutMs % 1000);
    numEvents = ::kevent(_pollfd, &*events_.begin(), static_cast<int>(events_.size()), NULL, 0, &wait_time);

    switch (numEvents)
    {
    case -1:
        if (savedErrno != EINTR)
        {
            errno = savedErrno;
            LOG_ERR("mEpollPoller::poll error!,the line:%d", __LINE__);
        }
        break;
    case 0:
        LOG_DEBUG("kevent time out!");
        break;
    default:
        LOG_INFO("kevent listen count:%d events happedned", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size())
            events_.resize(2 * events_.size());
        break;
    }
#else
    numEvents = ::epoll_wait(_pollfd, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    switch (numEvents)
    {
    case -1:
        if (savedErrno != EINTR)
        {
            errno = savedErrno;
            LOG_ERR("mEpollPoller::poll error!,the line:%d", __LINE__);
        }
        break;
    case 0:
        LOG_DEBUG("epoll_wait time out!");
        break;
    default:
        LOG_INFO("epoll_wait listen count:%d events happedned", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size())
            events_.resize(2 * events_.size());
        break;
    }
#endif
    return now;
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
            //  _channels添加channel
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
            update(EV_DISABLE, channel);
#else
            update(EPOLL_CTL_MOD, channel);
#endif
        }
    }
}
void mEpollPoller::removeChannel(mChannel *channel)
{
    int fd = channel->fd();
    LOG_INFO("%s=> fd:%d", __FUNCTION__, fd);
    if (_channels.find(fd) == _channels.end())
    {
        LOG_ERR("%s=> out of range!", __FUNCTION__);
        return;
    }
    if (_channels.at(fd) != channel)
    {
        LOG_ERR("%s=> this _channels discover the channel!", __FUNCTION__);
        return;
    }
    int index = channel->index();
    //  _channels删除channel
    _channels.erase(fd);
    if (index == kAdded)
    {
#ifdef __linux__
        update(EPOLL_CTL_DEL, channel);
#elif __APPLE__
        update(EV_DELETE, channel);
#else
        update(EPOLL_CTL_DEL, channel);
#endif
    }
    channel->setIndex(kNew);
}
mEpollPoller::~mEpollPoller()
{
    ::close(_pollfd);
}
