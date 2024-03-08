/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-08 13:53:09
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-08 15:33:01
 * @FilePath: /muduo-demo/net/mEpollPoller.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "mEpollPoller.h"
#include <mEventLoop.h>
#include <mchannel.h>
using namespace mmuduo;
using namespace mmuduo::mnet;

namespace
{
    const int kNew = -1;    //  某单个channel还没被添加
    const int kAdded = 1;   //  某单个channel已经被添加
    const int kdeleted = 1; //  某单个channel已经被删除
}
#ifdef __linux__
mEpollPoller::mEpollPoller(mEventLoop *loop)
    : mPoll(loop),
      _epollfd(::epoll_create1(EPOLL_CLOEXEC)),
      events_(KInitEventListSize)

{
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
Timestamp mEpollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    return nullptr;
}
void mEpollPoller::updateChannel(mChannel *channel)
{
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
}
