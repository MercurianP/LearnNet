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
### fork
unistd.h
pid_t fork(void)-成功返回进程id 失败返回-1
fork创建调用的进程的副本（复制正在运行的、调用fork函数的进程），两个进程都将执行fork函数调用后的语句，（父进程返回子进程id，子进程返回0）

## 僵尸进程
进程执行完main之后应该被销毁，但是有些进程会变成僵尸进程
### 产生原因
两个例子：1传递参数并调用exit函数2main函数执行return语句并返回值。
这两个的返回值都会给操作系统，操作系统不会销毁子进程，直到把这些值传递给产生该子进程的父进程。还没返回给父进程的进程就是僵尸进程。

所以销毁子进程防止僵尸进程的办法：向父进程传递eixt参数或者return 语句的返回值。
传递的办法是父进程主动发起请求（函数调用）操作系统才会传递该值。也就是说如果父进程没有主动获取子进程的结束状态，那么操作系统会一直保存该子进程，让其处于僵尸状态。

#### 后台执行程序： ./XXX &->&触发后台处理

### 销毁僵尸进程的办法
1:调用wait获得子进程的返回值  -sys/wait.h   pid_t wait(int * statloc)成功返回终止进程ID，失败返回-1

如果调用时子进程终止，那么子进程终止时传递的返回值将保存到改函数的参数中，因为参数只想的单元还包含其他信息，通过WIFEXITED(子进程正常时返回true WEXITSTAUS返回子进程返回值)

statloc通过宏定义传递状态信息

TIPS:调用wait如果没有已终止的子进程，那么程序会处于阻塞状态。！！！！

2:调用waitpid函数（防止阻塞）-sys/wait.h  pid_t waitpid(pid_t pid ,int *statloc, int options)

TIPS
pid     设置终止子进程的ID，-1为任意进程
options 设置 WNOHANG 返回零，退出函数。这就能实现非阻塞

## 信号处理siganal handling->什么时候销毁进程
因为主进程不能一直像在waitpid中那样一直刷新等待，所以采用信号处理机制，操作系统在发生特定事件的时候给

#include< signal.h> void (*signal (int signo, void (*func) (int) )) (int)
函数的返回类型为函数指针 signal是函数名  变量是int 和 *func函数指针
当发生signo（SIGALRM已到通过调用alarm函数注册的时间 SIGINT输入ctrl+c SIGchLD子进程终止） 三种情况时，调用函数指针

注册信号就是调用相关函数，当发生一些该情况时就能调用函数

### sigaction函数：类似于signal函数

## 多任务并发服务器
### 基于进程
通过信号函数 waitpid 终止进程

### 分割IO
客户端通过 两个线程进行读写，这样可以使得数据可以继续进行。

# 11进程通讯
两个进程之间交换数据，其实就是共同使用一片内存
## 管道实现进程间通信（PIPE）
管道不属于进程的资源，其和套接字一样，属于操作系统，不是fork的对象
#include <unistd.h> int pipe(int filedes[2])
[0]通过管道接受数据时使用的文件描述符
[1]发送时使用的文件描述符







