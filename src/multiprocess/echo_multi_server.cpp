//回声服务器 多进程实现
//父进程accept受理连接请求
//获取的套接字描述符传递给子进程
//子进程提供服务

//该程序对应的客户端在 tcp echo _client2


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30
void error_handling(char * message);
void read_childproc_killchildproc(int sig);
void error_handling(char * message);

int main(int argc,char* argv[])
{
    //注册
    pid_t pid;
    struct sigaction act;
    act.sa_handler=read_childproc_killchildproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    int state = sigaction(SIGCHLD,& act,0);//注册结束子进程的函数

    //socket
    if(argc!=2)
    {
        printf("usage: %s <port>\n",argv[0]);
        exit(1);
    }
    socklen_t adr_sz;
    int str_len;
    char buf[BUF_SIZE];
    int serv_sock,clnt_sock;
    sockaddr_in serv_adr,clnt_adr;
    serv_sock=socket(PF_INET,SOCK_STREAM,0);//tcp
    //端口
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));
    //bind listen
    if(bind(serv_sock,(sockaddr *)&serv_adr,sizeof(serv_adr))==-1)//socket和端口ip等绑定
        error_handling("bind() error");
    if(listen(serv_sock, 5))//开始监听
        error_handling("listen() error");

    //等待客户端的连接
    while(1)
    {
        adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock,(sockaddr*)&clnt_adr,&adr_sz);//客户端请求连接
        if(clnt_sock==-1)
            continue;
        else
            printf("a new clnt connect \n");
        
        // 连接进来之后创建子进程进行处理
        pid = fork(); 
        if(pid==-1)//false
        {
            close(clnt_sock);//build child proc false;关闭连接
            continue;
        }
        
        if(pid ==0)//child
        {
            //进程是复制出来的
            close(serv_sock);//***套接字是操作系统所有的，复制的是描述符，都关闭了才会使得套接字关闭，所以需要关闭多余的套接字剩下一个。
            while((str_len=read(clnt_sock,buf,BUF_SIZE))!=0)//quit clnt will read 0
                write(clnt_sock,buf,str_len);
            
            close(clnt_sock);
            puts("clinet disconnected... \n");
            return 0;//调用信号处理函数
        }
        else//parent
        {
            close(clnt_sock);

        }

    }    

    close(serv_sock);
    return 0;
}






void read_childproc_killchildproc(int sig)//杀死子进程的程序
{
    int status;
    pid_t id= waitpid(-1,&status,WNOHANG);//任意子程序，退出的状态，非阻塞；
    if(WIFEXITED(status))
    {
        printf("removed proc id : %d \n",id);//waitpid's status
        printf("child send : %d \n",WEXITSTATUS(status));//子进程返回值
    }
}

void error_handling(char * message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}