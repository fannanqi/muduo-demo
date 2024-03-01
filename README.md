# muduo-demo

Implement the Muduo network library

##  网络IO阶段一

### 数据准备:阻塞和非阻塞

代码案例：

```c++
ssize_t recv(int sockfd,void *buf,size_t len,int flags);
int size=recv(sockfd,buf,1024,0);
return:
  size == -1 :远端断开连接
  size == 0 && error== EAGAIN :表示连接正常，但是没有数据到来
  size > 0 :表示有数据到来
```



阻塞：
    sockfd会一直阻塞，等待数据的到来
    调用IO方法的线程进入阻塞状态
    

非阻塞：
    recv会直接返回，不会阻塞该线程
    不会改变线程的状态，通过返回值判断

##  网络IO阶段二


强调IO的同步和异步

### 数据的读写:同步IO和异步IO

同步IO：

​	用户进程发出IO调用，去获取IO设备数据，双方的数据要经过内核缓冲区同步，完全准备好后，再复制返回到用户进程。 而复制返回到用户进程会导致请求进程阻塞，直到I/O操作完成。

同步IO举例：recv函数 

异步IO:

​	用户进程发出IO调用，去获取IO设备数据，并不需要同步，内核直接复制到进程，整个过程不导致请求进程阻塞。

异步IO举例：recv在读取数据缓存区的同时，将数据cpoy到buff，以及记录signal信号