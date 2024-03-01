# muduo-demo

Implement the Muduo network library

##  网络IO阶段一

### 数据准备:阻塞和非阻塞

代码案例：
	ssize_t recv(int sockfd,void *buf,size_t len,int flags);
	int size=recv(sockfd,buf,1024,0);
    size == -1 :远端断开连接
    size == 0 && error== EAGAIN :表示连接正常，但是没有数据到来
	size > 0 :表示有数据到来

阻塞：
    sockfd会一直阻塞，等待数据的到来
    调用IO方法的线程进入阻塞状态
    

非阻塞：
    recv会直接返回，不会阻塞该线程
    不会改变线程的状态，通过返回值判断

##  网络IO阶段二

​强调IO的同步和异步

### 数据的读写