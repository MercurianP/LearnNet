// 迭代回声服务器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<arpa/inet.h>
#include <sys/socket.h>
#include <iostream>

#define BUF_SIZE 1024

//using namespace std;
//错误处理函数
void error_handling(char* message);

int main(int argc,char * argv[])
{
    int serv_sock, clnt_sock;//句柄‘
    char message[BUF_SIZE];
    int str_len,i;

    sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2)
    {
        std::cout<<"Usage "<<argv[0]<<"port \n"<<std::endl;
        exit(1);
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    if(serv_sock==-1)error_handling("socket() error");
    
    memset(&serv_adr,0,sizeof(serv_adr));//开空间
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));
    
    if(bind(serv_sock,(sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handling("bind() error");

    clnt_adr_sz=sizeof(clnt_adr);

    //处理五个客户端链接，依次向五个客户端提供服务。
    for(i=0;i<5;i++){
        clnt_sock=accept(serv_sock,(sockaddr*)&clnt_adr,&clnt_adr_sz);
        if(clnt_sock==-1)
            error_handling("accept() error");
        else
            std::cout<<"connect client "<<i+1<<std::endl;
        //回声功能：原封不动的将内容返回，如果客户端的调用close，循环变成假
        while((str_len=read(clnt_sock,message,BUF_SIZE))!=0){
            write(clnt_sock,message,str_len);
        }

        close(clnt_sock);

    }
    close(serv_sock);

    return 0;
}

void error_handling(char * message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}