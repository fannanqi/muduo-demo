/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-01 13:47:00
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-03 19:41:58
 * @FilePath: /muduo-demo/muduo-tool/log/include/lockqueue.h
 * @Description:日志队列
 */
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
// 异步写日志的日志队列
template <typename T>
class LockQueue
{
private:
    //  日志消息队列
    std::queue<T> _queue;

    //  队列锁的互斥量
    std::mutex _mutex;

    //  条件变量
    std::condition_variable _condvariable;

public:
    void Push(const T &);
    T Pop();
    LockQueue(/* args */);
    ~LockQueue();
};

//  插入日志消息
template <typename T>
void LockQueue<T>::Push(const T &data)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(data);
    _condvariable.notify_all();
}

//  拿取日志消息
template <typename T>
T LockQueue<T>::Pop()
{
    std::unique_lock<std::mutex> lock(_mutex);
    //  日志队列为空，线程进入wait状态
    _condvariable.wait(lock, [this]()
                       { 
            if (!_queue.empty())
                return true;
            return false; });
    //  返回一个局部变量
    T data = _queue.front();
    _queue.pop();
    lock.unlock();
    return data;
}

template <typename T>
LockQueue<T>::LockQueue()
{
}

template <typename T>
LockQueue<T>::~LockQueue()
{
}