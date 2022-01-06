//3 防止僵尸进程

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

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

int main(int argc,char *argv[])
{
    pid_t pid;
    //注册消息
    struct sigaction act;
    act.sa_handler=read_childproc_killchildproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaction(SIGCHLD,&act,0);

    //create child proc
    pid=fork();
    if(pid==0)//child proc
    {
        puts("hi, i am child process");
        sleep(10);
        return 12;
    }
    else//parent
    {
        printf("child proc id: %d \n",pid);
        pid = fork();//second child proc
        if(pid==0)//child proc
        {
            puts("hi, i am child proc ");
            sleep(10);
            exit(24);
        }else
        {
            int i;
            for(i=0;i<5;i++)
            {
                puts("wait...");
                sleep(5);
            }

        }
    }
    return 0;//主程序die
    
}