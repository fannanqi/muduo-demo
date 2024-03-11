/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-08 08:49:47
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-11 15:45:38
 * @FilePath: /muduo-demo/net/include/mEpollPoller.h
 * @Description: 封装epoll、kqueue的行为
 */
#pragma once
#include <mPoll.h>
#include <vector>
#include <mTimestamp.h>
#ifdef __linux__
#include <sys/epoll.h>
#elif __APPLE__
#include <sys/event.h>
#include <sys/types.h>
#else
#include <sys/epoll.h>
#include <sys/poll.h>
#endif
/**
    Epoll:
        epoll_create    epoll_ctl(add,mod,del)   epoll_wait
*/
namespace mmuduo
{
    namespace mnet
    {
        class mChannel;
        class mEventLoop;
        class mEpollPoller : public mPoll
        {
        private:
            //  EventList的初始化事件长度
            static const int KInitEventListSize = 16;
            //  关于各种网络IO模型的注册事件
//  def    EventList
#ifdef __linux__
            using EventList = std::vector<epoll_event>;
#elif __APPLE__
            using EventList = std::vector<struct kevent>;
#else
            using EventList = std::vector<event>;
#endif
            //  endef   EventList
            //  epol_create句柄
            int _pollfd;
            //  事件集合，每个event存储sockfd
            EventList events_;
            //  填写活跃的链接（！！！！！！！！！！fixing）
            void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
            //  更新channel的事件 epoll_ctl add/mod/del
            void update(int operation, mChannel *channel);

        public:
            //  对应epoll_create
            mEpollPoller(mEventLoop *loop);

            //  重写基类mPoll的抽象方法,对应epoll_wait
            Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;

            //  对应epoll_ctl
            virtual void updateChannel(mChannel *channel) override;
            virtual void removeChannel(mChannel *channel) override;
            ~mEpollPoller();
        };
    }
}
