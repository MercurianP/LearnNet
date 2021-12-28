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
    //先把变量的定义定义在前面
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    sockaddr_in serv_adr,from_adr;
    if(argc!=3){
        printf("usage : %s  <ip> <port>",argv[0]);
        exit(1);
    }

    sock= socket(PF_INET,SOCK_DGRAM,0);
    if(sock==-1) error_handling("socket() error");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));

    while(1)
    {
        fputs("insert message (q to quit):",stdout);
        fgets(message,sizeof(message),stdin);
        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
            break;
        sendto(sock,message,sizeof(message),0,(sockaddr*)&serv_adr,sizeof(serv_adr));
        adr_sz=sizeof(from_adr);
        str_len=recvfrom(sock,message,BUF_SIZE,0,(sockaddr*)&from_adr,&adr_sz);
        message[str_len]=0;//变成string
        printf("Message form serve: %s",message);
    }
    close(sock);
    return 0;
    



}
void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
