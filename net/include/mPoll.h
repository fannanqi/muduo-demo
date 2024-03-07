/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-06 23:21:12
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-07 08:13:04
 * @FilePath: /muduo-demo/net/include/mPoll.h
 * @Description:muduo库中多路事件分发器的核心IO复用模块
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#pragma once
#include <mnocopyable.h>
#include <vector>
#include <unordered_map>
#include <mTimestamp.h>
namespace mmuduo
{
    namespace mnet
    {
        class mChannel;
        class mEventLoop;
        class mPoll : public mnocopyable
        {
        private:
            mEventLoop *_ownerLoop;

        protected:
            //  ChannelMap的key表示为channel的fd，value表示fd所属的Channel通道类型
            using ChannelMap = std::unordered_map<int, mChannel *>;
            ChannelMap _channels;

        public:
            using ChannelList = std::vector<mChannel *>;
            mPoll(mEventLoop *loop);

            //  给所有IO复用保留统一的接口(激活channels)
            virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
            //  更新channel接口
            virtual void updateChannel(mChannel *channel) = 0;
            //  删除channel接口
            virtual void removeChannel(mChannel *channel) = 0;
            //  判断参数channel是否在当前Poller当中
            virtual bool hasChannel(mChannel *channel) const;
            //  mEventLoop可以通过该接口获取默认的IO复用的具体实现
            static mPoll *newDefaultPoler(mEventLoop *loop);
            virtual ~mPoll() = default;
        };

    }
}