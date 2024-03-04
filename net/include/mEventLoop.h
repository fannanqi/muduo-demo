/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 09:23:50
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-04 09:40:50
 * @FilePath: /muduo-demo/net/mEventLoop.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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