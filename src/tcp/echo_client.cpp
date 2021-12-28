#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

//客户端只需要 connect 就可以read write



#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc,char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
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
        write(sock,message,strlen(message));
        str_len=read(sock,message,BUF_SIZE-1);
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
