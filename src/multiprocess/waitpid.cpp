#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char * argv[])
{
    int status ;
    pid_t pid=fork();
    if(pid==0)//子进程
    {
        sleep(15);//15s
        return 24;
    }
    else{
        while(!waitpid(-1,&status,WNOHANG))//父进程一直等待，睡眠3秒钟
        {
            sleep(3);
            puts("sleep 3 sec");
        }
        if(WIFEXITED(status))//返回值有
        {
            printf("child send %d \n",WEXITSTATUS(status));

        }

    }
    return 0;

}