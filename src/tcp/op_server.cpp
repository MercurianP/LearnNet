//保证数据读齐的回声服务器 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<arpa/inet.h>
#include <sys/socket.h>
#include <iostream>

//读取的时候先读一个 数字  然后循环读取第一个数字个数
#define BUF_SIZE 1024
#define OPSZ 4
void error_handling(char * message);
int caculate(int opnum,int opnds[],char op);


int main(int argc,char* argv[])
{
    int serv_sock,clnt_sock;//两个句柄
    char opinfo[BUF_SIZE];//传输内容
    int result ,opnd_cnt,i;
    int recv_cnt,recv_len;
    sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;
    if(argc!=2){
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }
    //配置服务端
    serv_sock =socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1)
    {
        error_handling("socket() error");
        exit(1);
    }
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));
    //bind和listen
    if(bind(serv_sock,(sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");
    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    clnt_adr_sz=sizeof(clnt_adr);
    //
    for(i=0;i<5;i++)
    {
        opnd_cnt=0;
        clnt_sock=accept(serv_sock,(sockaddr*)&clnt_adr,&clnt_adr_sz);//求
        read(clnt_sock,&opnd_cnt,1);//read a byte
        //std::cout<<opnd_cnt<<std::endl;

        recv_len=0;//读取了长度
        //保证数据读完
        while((opnd_cnt*OPSZ+1)>recv_len)
        {
            recv_cnt=read(clnt_sock,&opinfo[recv_len],BUF_SIZE-1);//read return size
            recv_len+=recv_cnt;
            //std::cout<<recv_len<<std::endl;

        }
        
        result=caculate(opnd_cnt,(int*)opinfo,opinfo[recv_len-1]);
        write(clnt_sock,(char*)&result,sizeof(result));
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

int caculate(int opnum,int opnds[],char op){
    int result = opnds[0], i;
    switch(op)
    {
        case '+':
            for(i=1;i<opnum;i++) result+=opnds[i];
            break;
        case '-':
            for(i=1;i<opnum;i++) result-=opnds[i];
            break;

        case '*':
            for(i=1;i<opnum;i++) result*=opnds[i];
            break;
        case '/':
            for(i=1;i<opnum;i++) result/=opnds[i];
            break;

    }
    std::cout<<result<<std::endl;
    return result;
}