#include "logger.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <memory>
#include <mTimestamp.h>
std::mutex _loggerMutex;
std::mutex _instanceMutex;
Logger *Logger::_mlogger = nullptr;
Logger::Logger()
{

    //  启动专门写日志线程
    std::thread writeLogTask([&]()
                             {
        for (;;)
        {
            //  获取当天的日期，然后获取日志信息，写入相应的日志文件当中
            time_t _now=time(nullptr);
            tm *_nowtime=localtime(&_now);
            char file_name[128];
            // 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
	        struct stat statbuf;
            sprintf(file_name,"%d-%d-%d-log.txt",_nowtime->tm_year+1900,_nowtime->tm_mon+1,_nowtime->tm_mday);
            
            //  与当前日期不相同，直接赋值
            if (_logfiletm.tm_year!=_nowtime->tm_year+1900)
            {
                _logfiletm.tm_year=_nowtime->tm_year+1900;
            }
            else if(_logfiletm.tm_mon!=_nowtime->tm_mon+1)
            {
                _logfiletm.tm_mon=_nowtime->tm_mon+1;
            }
            else if(_logfiletm.tm_mday!=_nowtime->tm_mday)
            {
                _logfiletm.tm_mday=_nowtime->tm_mday;
            }
            //  与当前日期相同
            else
            {
                //  判断是否超过20MB
                // 提供文件名字符串，获得文件属性结构体
                stat(file_name, &statbuf);
                // 获取文件大小
                size_t filesize = statbuf.st_size;
                if(filesize>=20*1048576)
                {
                    sprintf(file_name,"%d-%d-%d-%d-log.txt",_nowtime->tm_yday+1900,_nowtime->tm_mon+1,_nowtime->tm_mday,_nowtime->tm_hour);
                }

            }
            //  进行文件操作
            std::ofstream ofs;
            ofs.open(file_name,std::ios::app);
            if(!ofs.is_open())
            {
                std::cout<<"logger file: "<<file_name<<" open error\n";
                exit(EXIT_FAILURE);
            }
            
            //  拿取消息队列的日志消息
            std::string msg=_mlckQue.Pop();
            Timestamp _timePtr = Timestamp::now();
            char time_buf[128]={0};
            std::string typemsg;
            switch (_mloglevel)
            {
            case INFO:
                typemsg= "INFO";
                break;
            case ERROR:
                typemsg= "ERROR";
                break;
            case FATAL:
                typemsg= "FATAL";
                break;
            case DEBUG:
                typemsg= "DEBUG";
                break;  
            default:
                typemsg = "NULL";
                break;
            }
            snprintf(time_buf,128,"%s =>[%s]",_timePtr.toString().c_str(),typemsg.c_str());
            msg.insert(0,time_buf);
            //  将日志写入文件当中
            ofs<<msg<<std::endl;
            //  关闭文件
            ofs.close();
            
        } });
    //  设置分离线程
    writeLogTask.detach();
}

Logger::~Logger()
{
}

void Logger::SetLogLevel(LogLevel level)
{
    _mloglevel = level;
}

void Logger::Log(std::string msg)
{
    _mlckQue.Push(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(70));
}

Logger *Logger::GetInstance()
{
    if (_mlogger == nullptr)
    {
        std::unique_lock<std::mutex> _instanceLock(_instanceMutex);
        if (_mlogger == nullptr)
        {
            _mlogger = new Logger();
        }
    }
    return _mlogger;
}