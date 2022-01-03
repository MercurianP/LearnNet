# tcp原理
TCP传输没有边界，可能调用一次传输40字节，也可能传输10字节。
事实上tcp套接字有缓冲。write之后先发到缓冲，适当的时候发送。read也有缓冲，从缓冲中读取。
缓冲的特性：
1IO缓冲在每个tcp都存在
2IO缓冲在创建套接字时候自动生成
3关闭套接字也会传输输出缓冲中的数据
4关闭套接字会丢失输入缓冲区的数据。

# 一些tips
如果输入缓存大于输出缓存，也没事，因为tcp会控制数据流，有滑动窗口
write()是在数据传输完成时返回。
三次握手 四次挥手

tcp服务端  socket();bind();listen();accept();
客户端      socket；connect；
connect中把IP和端口分配套接字。

udp服务端  socket();bind() recvfrom  sentto
客户端   socket    sendto->第一次会客户端分配地址信息（ip和端口）



# udp
udp没有ACK应答消息，也没有给数据包编序号。
tcp相对于udp，udp缺少流控制。相对速度快。

ip的作用是让主机传递到主机，所以网络层
udp的作用是将udp包给udp套接字。

# 一些tips
udp不需要连接过程，所以不需要调用tcp过程中的listen 和accept
只有创建套接字的过程和数据交换的过程

与tcp一个链接一个套接字不一样，udp套接字只需要一个就可以了。

因为不保持链接状态，所以每一次传输数据的时候都要添加目标地址信息。
sendto**
recvfrom**

## 6.3 UDP的套接字数据传输存在数据边界
输入函数的调用次数和输出函数的调用次数必须一样。
## 6.4connectedUDP and unconnectedUDP
sendto 传输数据分成三个阶段，1向套接字注册目标IP和端口 2传输数据3删除套接字中注册的信息

但是重复调用十分麻烦，简单的办法就是使用已连接套接字。
具体的办法就是调用connect(sock,&serve_adr,sizeof(serve_adr)) adr包含ip和port
需要注意的是，连接的UDP并不是说向套接字建立连接，而是说向套接字注册信息。


# 7断开套接字连接
先前的工作使用close（）完成的，

### 基于TCP的半连接：
完全断开连接会使得无法发送也无法接收，这不够，比如A发送了数据给B，并关闭，那么B回应给A的数据将被销毁。
所以Half-close被提出，只关闭一半。
函数：shutdown(int sock,int howto) 2参数 SHUT_RD输入 SHUT_WR SHUT_RDWR
该函数的目的：可以确定什么时候文件已经接受完成，不然客户端一直接受数据。相对于EOF


# 8域名及网络地址
DNS(domain name system)域名系统：转换IP地址和域名的系统。

ping www.baidu.com就可以知道域名对应的IP地址
nslookup 就可以知道自己计算机对应的额DNS服务器地址

## 8.2域名与ip地址之间的转换
因为客户端使用服务器提供的服务，服务器的ip变化次数比域名少

hostent * gethostbyname(char * hostname)  域名->ip
hostent* gethostbyaddr(char * addr, socklen_t,int family)


# 10 多进程服务器端
并发：实现连接请求受理 1s，平均服务时间2-3s
多进程服务器：通过创建多个进程提供服务
多路复用服务器：通过捆绑并统一管理IO对象提供服务
多线程服务器：通过生成与苦护短等量的线程提供服务


进程的定义：“占用内存空间的正在运行的程序”-程序运行需要加载到内存并运行。
一个核可以运行一个程序，超过核数就变成了分时

## 进程ID
操作系统会为每个进程分配一个进程id： ps au 查看进程及其详细信息









