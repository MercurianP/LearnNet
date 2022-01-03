#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char * message);

int main(int argc,char* argv[]){
    int sock;
    char message_1[]="hi";
    char message_2[]="I am the new udp server";
    char message_3[]="goodbye";
    
    //serve_adr
    sockaddr_in your_adr;
    if(argc!=3)
    {
        printf("usage : %s <ip> <port>",argv[0]);
        exit(1);
    }

    //define socket;
    sock=socket(PF_INET,SOCK_DGRAM,0);
    if(sock==-1)
        error_handling("socket() error ");
    
    memset(&your_adr,0,sizeof(your_adr));
    your_adr.sin_family=AF_INET;
    your_adr.sin_addr.s_addr=inet_addr(argv[1]);
    your_adr.sin_port=htons(atoi(argv[2]));

    sendto(sock,message_1,sizeof(message_1),0,(sockaddr*)&your_adr,sizeof(your_adr));

    sendto(sock,message_2,sizeof(message_2),0,(sockaddr*)&your_adr,sizeof(your_adr));

    sendto(sock,message_3,sizeof(message_3),0,(sockaddr*)&your_adr,sizeof(your_adr));

    close(sock);

    return 0;

}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}