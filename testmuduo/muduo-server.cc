#include "muduo-server.h"

void ChatServer::onConnection(const net::TcpConnectionPtr &conn)
{
    if (conn->connected())
        //  打印对端的连接信息
        std::cout << "address: " << conn->peerAddress().toIpPort() << " ->  " << conn->localAddress().toIpPort() << std::endl;
    else
    {
        std::cout << "failed\n";
        conn->shutdown();
    }
}

void ChatServer::onMessage(const net::TcpConnectionPtr &conn, net::Buffer *buffer, ::Timestamp time)
{
    std::string buf = buffer->retrieveAllAsString();
    std::cout << "recv data:" << buf << " time:" << time.toString() << std::endl;
    conn->send("ok");
}

ChatServer::ChatServer(net::EventLoop *loop,
                       const net::InetAddress &listenAddr,
                       const std::string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    //  给服务器注册用户连接的创建和断开回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection,
                                            this,
                                            std::placeholders::_1));

    //  给服务器注册用户读写事件回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage,
                                         this,
                                         std::placeholders::_1,
                                         std::placeholders::_2,
                                         std::placeholders::_3));

    //  设置服务器端的线程数量,1个I/O线程，2个worker线程
    _server.setThreadNum(3);
}

void ChatServer::start()
{
    _server.start();
}

ChatServer::~ChatServer()
{
}
int main(void)
{

    ::net::EventLoop loop;
    ::net::InetAddress addr("127.0.0.1", 6000);
    std::unique_ptr<ChatServer> server(new ChatServer(&loop, addr, "chatserver"));
    server->start(); //  listenfd    epoll_ctl=>epoll
    loop.loop();     //  epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件
    return 0;
}
