/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-02 18:38:43
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-03 14:01:05
 * @FilePath: /muduo-demo/testmuduo/muduo-server.h
 * @Description:基于muduo网络库实现服务端开发示例
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <iostream>
#include <string>
#include <memory>
using namespace muduo;
/**
 * 1、组合Tcpserver对象
 * 2、创建EventLoopp事件循环对象的指针
 * 3、明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
 * 4、在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
 * 5、设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
 */
class ChatServer
{
private:
    net::TcpServer _server;
    net::EventLoop *_loop;

    //  专门处理用户的连接创建和断开
    void onConnection(const net::TcpConnectionPtr &);

    //  专门处理用户的读写事件
    void onMessage(const net::TcpConnectionPtr &, //   连接
                   net::Buffer *,                 //   缓冲区
                   ::Timestamp);                  //   接收到数据的时间信息

public:
    ChatServer(net::EventLoop *loop,               // 事件循环
               const net::InetAddress &listenAddr, // IP+Port
               const std::string &nameArg);        // 服务器的名字

    //  开启事件循环
    void start();
    ~ChatServer();
};