/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-03 17:12:41
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-04 08:37:49
 * @FilePath: /muduo-demo/muduo-tool/timestamp/include/Timestamp.h
 * @Description:时间系统
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <iostream>
#include <string>
class Timestamp
{
private:
#ifdef _linux_
    int64_t _microSecondsSinceEpoch;
#elif __APPLE__
    long _microSecondsSinceEpoch;
#endif

public:
    //  explicit防止隐式类型转换
    explicit Timestamp(/* args */);
    explicit Timestamp(int64_t microSecondsSinceEpoch);
    static Timestamp now();
    std::string toString() const;
    ~Timestamp();
};
