/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-08 08:06:43
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-08 08:54:56
 * @FilePath: /muduo-demo/net/include/DefaultPoller.h
 * @Description: 返回Poller的具体类型指针
 */
#include <mPoll.h>
#include <stdlib.h>
#include <logger.h>
using namespace mmuduo;
using namespace mmuduo::mnet;
mPoll *mPoll::newDefaultPoler(mEventLoop *loop)
{
#ifdef __linux__
    //  生成epoll的实例
    LOG_INFO("USING EPOLL");
    return nullptr;
#elif __APPLE__
    //  生成kqueue的实例
    LOG_INFO("USING KQUEUE");
    return nullptr;
#else
    //  生成epoll的实例
    LOG_INFO("USING POLL");
    return nullptr;
#endif
}