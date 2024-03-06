/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 13:29:54
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-06 20:22:37
 * @FilePath: /muduo-demo/net/include/mchannel.h
 * @Description: 事件处理器，封装fd和event(Demultiplex)
 */
#pragma once
#include <functional>
#include <mnocopyable.h>
#include <memory>
//  Timestamp不再namespace作用域范围内
class Timestamp;
namespace mmuduo
{
    namespace mnet
    {
        //  mEventLoop在mmuduo和mnet的作用域范围内
        class mEventLoop;
        /**
            mChannel为隧道，封装了sockfd和其感兴趣的event，如EPOLLIN、EPOLLOUT
            还绑定了poller返回的具体事件
            EventLoop包含Channel，channel包含fd，events(如EPOLL_IN、EPOLL_OUT)
        */
        class mChannel : mnocopyable
        {
        public:
            using EventCallback = std::function<void()>;
            using ReadEventCallback = std::function<void(Timestamp)>;

            mChannel(mEventLoop *loop, int fd);
            //  fd得到poller通知以后，处理事件的
            void handleEvent(Timestamp receiveTime);

            //  设置回调函数对象
            void setReadCallback(ReadEventCallback cb);
            void setWriteCallback(EventCallback cb);
            void setCloseCallback(EventCallback cb);
            void setErrorCallback(EventCallback cb);

            //  防止当channel被手动remove掉，channel还在执行回调操作
            void tie(const std::shared_ptr<void> &);

            int fd() const { return _fd; }
            void set_revents(int revt) { _revents = revt; }

            void enableReading()
            {
                _events |= KReadEvent;
                update();
            }
            //  ~KReadEvent取反，并使能
            void disableReading()
            {
                _events &= ~KReadEvent;
                update();
            }
            void enableWriting()
            {
                _events |= KReadEvent;
                update();
            }
            void disableWriting()
            {
                _events &= ~KWriteEvent;
                update();
            }
            void disableAll()
            {
                _events = KNoneEvent;
                update();
            }

            //  返回fd当前事件的状态
            bool isNoneEvent() const { return _events == KNoneEvent; }
            bool isReading() const { return _events & KReadEvent; }
            bool isWriting() const { return _events & KWriteEvent; }

            //  Poller的index
            int index() { return _index; }
            void setIndex(int idx) { _index = idx; }

            //  one loop per thread
            mEventLoop *ownerLoop() { return _loop; }

            //  在Channel所属的EventLoop,当前的channel删除掉
            void remove();
            ~mChannel();

        private:
            //  表示当前fd的事件状态
            static const int KNoneEvent;
            static const int KReadEvent;
            static const int KWriteEvent;

            mEventLoop *_loop; //  表示当前事件循环,因为EeventLoop(作为事件分发器，添加event事件),所以需要_loop指针对事件的增删
            const int _fd;     //  _fd,Poller监听对象
            int _events;       //  注册_fd的事件
            int _revents;      //  Poller返回的具体发生事件
            int _index;
            std::weak_ptr<void> _tie; //  观察者，观察强智能指针
            bool _tied;
            bool _eventHandling; // 是否在执行handler事件
            //  因为channel通道里面能获知fd最终发生的具体的事件revents，所以负责调用具体事件的回调操作
            ReadEventCallback _readCallback;
            EventCallback _writeCallback;
            EventCallback _closeCallback;
            EventCallback _errorCallback;

            //  当改变channel所表示fd的events事件后，update负责在poller里面改变fd相应的事件epoll_ctl、kevent
            void update();
            //  处理事件的回调操作
            void handleEventWithGuard(Timestamp receiveTime);
        };
    }
}