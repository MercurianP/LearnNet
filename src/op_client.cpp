//this program is operater 

//echo中的回声服务器的 应用层协议是 在收到一个后就断开链接 
//我们在实际的应用过程中针对自己的 服务器需要自己制定应用层协议。
//需要注意：调用close 对方回收到efo

//编写程序前的协议
//1 客户端和服务端建立联系后以1字节传递待算个数
//2 客户端向服务器传递的整数占用4个字节。
//3 传递整数后传递运算符  运算符占用1个字节
//4 选择字符+-*%进行传递
//5 服务器以4字节整数向客户端回传运算结果
//6 客户端得到运算结果后终止链接

//我的理解：每个链接其实就是句柄  服务器和客户端开始的时候定义了sock（包含了端口 ip以及链接方式等）
//后面客户端是connect  服务端是bind listen
//最后客户端 write and read  服务端也是 accept(客户端)-》write and read-》close（客户端）

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4//result size
#define OPSZ 4   //每个整数占用4个字节
void error_handling(char * message);


int main(int argc, char *argv[])
{
    int sock;
    char opmsg[BUF_SIZE];//传输用
    int result, opnd_cnt,i;//第二个是告诉几个数字运算
//1 socket 配置
    sockaddr_in serv_adr;
    if(argc!=3) //两个输入的参数
    {
        printf("usage : %s  <IP> <port>\n ",argv[0]);
        exit(1);
    }
    sock =socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1){
        error_handling("socket() error");
    }
    memset(&serv_adr,0,sizeof(sockaddr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port =htons(atoi(argv[2]));

    //连接服务器 cock的句柄和地址 大小
    if(connect(sock,(sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("connect() error");
    else
        puts("Connecting...");

//2 输入数据 个数 &数据
    fputs("Operand count:" ,stdout);
    scanf("%d",&opnd_cnt);//几个数据
    opmsg[0]=(char)opnd_cnt;

    for(i =0;i<opnd_cnt;i++)
    {
        printf("Operand %d: ",i+1);
        scanf("%d",(int*)&opmsg[i*OPSZ+1]);//把数据存在每个位置。int的大小是4个字节
    }
//2 输入数据 符号
    fgetc(stdin);//从哪里读取数据
    fputs("operation:",stdout);
    scanf("%c",&opmsg[opnd_cnt*OPSZ+1]);
    write(sock,opmsg,opnd_cnt*OPSZ+2);
    read(sock,&result,RLT_SIZE);

    printf("operation result : %d \n",result);
    close(sock);
    return 0;

}

void error_handling(char * message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}