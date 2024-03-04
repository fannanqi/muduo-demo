/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 09:23:50
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-04 22:59:22
 * @FilePath: /muduo-demo/net/include/mEventLoop.h
 * @Description:事件分发器，开启事件循环epoll_wait
 */
#pragma once

namespace mmuduo
{
    namespace mnet
    {
        //  时间循环类  主要包含两大模块 Channel Poller(epoll的抽象)
        class mEventLoop
        {
        private:
            /* data */
        public:
            mEventLoop(/* args */);
            ~mEventLoop();
        };

    }
}