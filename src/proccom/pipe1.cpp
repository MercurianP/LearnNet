//1声明pipe（需要注意：她不是进程的，所以复制不会有什么影响，类似于套接字）
//通过调用fork函数，创建子进程 父进程，然后实现通过文件描述符的传递

#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc ,char * argv[])
{
    int fds[2];
    char str[]="Who are you?";
    char buf[BUF_SIZE];
    
    pid_t pid;
    pipe(fds);//int [2],然后生成两个描述符
}
