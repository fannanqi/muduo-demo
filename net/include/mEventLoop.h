/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 09:23:50
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-12 19:49:13
 * @FilePath: /muduo-demo/net/include/mEventLoop.h
 * @Description:事件分发器，开启事件循环epoll_wait
 */
#pragma once
#include <mnocopyable.h>
#include <functional>
#include <atomic>
#include <vector>
#include <mTimestamp.h>
#include <memory>
#include <mutex>

namespace mmuduo
{
    namespace mnet
    {
        class mchannel;
        class mPoll;
        //  时间循环类  主要包含两大模块 Channel Poller(epoll、kqueue的抽象)
        class mEventLoop
        {
        public:
            using Functor = std::function<void>();
            mEventLoop(/* args */);
            ~mEventLoop();

        private:
            using ChannelList = std::vector<mchannel *>;
            std::atomic_bool _looping;                  //    原子操作，通过CAS实现的
            std::atomic_bool _quit;                     //    标识退出loop循环
            std::atomic_bool _callingPendingFunctors;   //    标识当前loop是否有需要执行的回调操作
            const pid_t _threadId;                      //    记录当前loop所在线程的id
            Timestamp _pollReturnTime;                  //    poller返回事件的channels的时间点
            std::unique_ptr<mPoll> _poller;             //    mPoll指针
            int _wakeFd;                                //    充当mainreactor和subreadctor之间的事件通知,唤醒作用eventfd()，例如libevent的socket_pair
            std::unique_ptr<mchannel *> _wakeupChannel; //
            ChannelList _activeChannels;
            mchannel *_currentActiveChannel;
            std::vector<Functor> _pendingFunctors; //    存储loop需要执行的所有回调操作
            std::mutex _mutex;                     //    互斥锁，维护容器的线程操作
        };

    }
}