/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 13:35:32
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-08 15:33:37
 * @FilePath: /muduo-demo/muduo-tool/include/mnocopyable.h
 * @Description: 公共的禁止拷贝构造函数，防止调用拷贝构造函数引发异常
 */
#pragma once
namespace mmuduo
{
    class mnocopyable
    {
    public:
        mnocopyable(const mnocopyable &) = delete;
        mnocopyable &operator=(const mnocopyable &) = delete;

    protected:
        mnocopyable(/* args */) {}
        virtual ~mnocopyable() {}
    };
}