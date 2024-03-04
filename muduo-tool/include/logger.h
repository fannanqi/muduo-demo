/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-01 13:46:49
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-04 09:12:54
 * @FilePath: /muduo-demo/muduo-tool/log/include/logger.h
 * @Description: 日志系统
 */
#pragma once
#include "lockqueue.h"
#include <string>

extern std::mutex _loggerMutex;   // 宏定义函数锁的互斥量
extern std::mutex _instanceMutex; // 宏定义函数锁的互斥量
enum LogLevel
{
    INFO,  // 普通信息
    ERROR, // 错误信息
    FATAL, // core信息
    DEBUG, // 调试信息
};

// 日志系统
class Logger
{
private:
    Logger(/* args */);

    Logger(const Logger &) = delete;

    Logger(Logger &&) = delete;

    Logger &operator=(const Logger &) = delete;

    ~Logger();

    int _mloglevel; //  记录日志级别,设置info信息还是error信息

    LockQueue<std::string> _mlckQue; // 日志缓存队列

    tm _logfiletm; // 记录当前文件的时间，判断是否日志文件是否大于20MB

    static Logger *_mlogger;

public:
    //  设置日志级别
    void SetLogLevel(LogLevel level);

    //  设置写日志,把日志写入lockqueue缓冲区当中
    void Log(std::string);

    //  获取日志的单例
    static Logger *GetInstance();
};

//  定义宏
#define LOG_INFO(logmsgformat, ...)                     \
    do                                                  \
    {                                                   \
        _loggerMutex.lock();                            \
        Logger *logger = Logger::GetInstance();         \
        logger->SetLogLevel(INFO);                      \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        std::string cstr(c);                            \
        logger->Log(cstr);                              \
        _loggerMutex.unlock();                          \
    } while (0);

#define LOG_ERR(logmsgformat, ...)                      \
    do                                                  \
    {                                                   \
        _loggerMutex.lock();                            \
        Logger *logger = Logger::GetInstance();         \
        logger->SetLogLevel(ERROR);                     \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger->Log(c);                                 \
        _loggerMutex.unlock();                          \
    } while (0);

#define LOG_FATAL(logmsgformat, ...)                    \
    do                                                  \
    {                                                   \
        _loggerMutex.lock();                            \
        Logger *logger = Logger::GetInstance();         \
        logger->SetLogLevel(FATAL);                     \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger->Log(c);                                 \
        _loggerMutex.unlock();                          \
    } while (0);
#ifdef MUDEBUG
#define LOG_DEBUG(logmsgformat, ...)                    \
    do                                                  \
    {                                                   \
        _loggerMutex.lock();                            \
        Logger *logger = Logger::GetInstance();         \
        logger->SetLogLevel(DEBUG);                     \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger->Log(c);                                 \
        _loggerMutex.unlock();                          \
    } while (0);
#else
#define LOG_DEBUG(logmsgformat, ...)
#endif