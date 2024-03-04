/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 13:29:54
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-04 15:33:06
 * @FilePath: /muduo-demo/net/mchannel.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <iostream>
#include <mnocopyable.h>

class mEventLoop;
class Timestamp;
namespace mmuduo
{
    namespace mnet
    {
        /**
            mChannel为隧道，封装了sockfd和其感兴趣的event，如EPOLLIN、EPOLLOUT
            还绑定了poller返回的具体事件
        */
        class mChannel : mnocopyable
        {
        public:
            using EventCallback = std::function<void()>;
            using ReadEventCallback = std::function<void(Timestamp)>;

            mChannel(mEventLoop *loop, int fd);
            void handleEvent(Timestamp receiveTime);
            ~mChannel();

        private:
            //  表示当前fd的事件状态
            static const int KNoneEvent;
            static const int KReadEvent;
            static const int KWriteEvent;

            mEventLoop *_loop; //  表示当前事件循环
            const int _fd;     //  _fd,Poller监听对象
            int _events;       //  注册_fd的事件
            int _revents;      //  Poller返回的具体发生事件
        };

    }
}