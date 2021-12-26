#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

//客户端只需要 connect 就可以read write
//echo_cloent的问题 question:传输的是字符串，并且一次性发完。发完就read！！实际上很大的话分批次发送
//我们添加接受的计数，


#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc,char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    //改动1
    int recv_len, recv_cnt;
    sockaddr_in serv_adr;
    
    if(argc!=3){
        printf("usage %s <IP> <port>\n",argv[0]);
        exit(1);
    }
    //建立客户端socket. ip地址类型（4）  链接类型（有链接） 协议（自动推算）
    sock =socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1)
        error_handling("socket() error");
    //新建类
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;//ipv4
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);//服务器ip地址
    serv_adr.sin_port=htons(atoi(argv[2]));//端口

    //等待链接 服务器
    if(connect(sock,(sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("connect() error!");
    else//已经连上
        puts("Connected...");
    while(1)
    {
        fputs("Input message(Q to quit:",stdout);
        fgets(message,BUF_SIZE,stdin);
        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
            break;
        str_len = write(sock,message,strlen(message));

        recv_len=0;
        
        //不是一次性读完 限制条件就是让读到的数据的总和要大于等于发送的。这就涉及到分包
        while(recv_len<str_len)
        {
            recv_cnt=read(sock,message,BUF_SIZE-1);
            if(recv_cnt==-1)
                error_handling("read() error");
            recv_len+=recv_cnt;    

        }
        //读完之后再将最后一位置0
        message[str_len]=0;
        printf("message from server:%s",message);
    }
    close(sock);

    return 0;

}



void error_handling(char * message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
