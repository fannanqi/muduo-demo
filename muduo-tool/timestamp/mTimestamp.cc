/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-03 17:13:25
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-03 21:43:48
 * @FilePath: /muduo-demo/muduo-tool/timestamp/mTimestamp.cc
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "mTimestamp.h"
#include <time.h>
Timestamp::Timestamp(/* args */) : _microSecondsSinceEpoch(0)
{
}

Timestamp::~Timestamp()
{
}
Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
{
    this->_microSecondsSinceEpoch = microSecondsSinceEpoch;
}
Timestamp Timestamp::now()
{
    return Timestamp(time(nullptr));
}
std::string Timestamp::toString() const
{
    char buf[128] = {0};
    tm *_time = localtime(&this->_microSecondsSinceEpoch);
    snprintf(buf, 128, "%4d/%02d/%02d-%02d:%02d::%02d",
             _time->tm_year + 1900,
             _time->tm_mon + 1,
             _time->tm_mday,
             _time->tm_hour,
             _time->tm_min,
             _time->tm_sec);
    return buf;
}
