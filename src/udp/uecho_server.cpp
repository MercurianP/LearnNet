//需要说明的是，UDP不存在服务器和客户端  只是因为其提供服务 所以叫做服务端

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char * message);

int main(int argc,char *argv[])
{
    int serv_sock;//服务端的文件描述符
    char message[BUF_SIZE];//发送的缓存区大小
    int str_len;//
    socklen_t clnt_adr_sz;
    sockaddr_in serv_adr,clnt_adr;
    if(argc!=2){
        printf("usage: : %s <port> \n",argv[0]);//文件名
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_DGRAM,0);//定义udpsocket 类似文件
    if(serv_sock==-1)
    {
        error_handling("socket() error");
    }
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    //bind  文件和配置，
    if(bind(serv_sock,(sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
    {
        error_handling("bind() error");

    }
    //接受传输数据，由于没有链接 所以每次发送都需要 附带addr
    while(1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        str_len=recvfrom(serv_sock,message,BUF_SIZE,0,(sockaddr*)&clnt_adr,
                &clnt_adr_sz );
        sendto(serv_sock,message,BUF_SIZE,0,(sockaddr *)&clnt_adr,clnt_adr_sz);

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