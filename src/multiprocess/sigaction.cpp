//this file is a demo of sigaction  2
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/*
struct sigaction
{
    void (*sa_handler)(int);//调用的
    sigset_t sa_mask;
    int sa_flags;
}

*/



void timeout(int sig)
{
    if(sig==SIGALRM)
    {
        puts("timeout");
    }
    alarm(2);//计时2s
}


int main(int argc,char* argv[])
{
    int i;

    struct sigaction action;
    action.sa_handler=timeout;
    sigemptyset(&action.sa_mask);
    action.sa_flags=0;

    sigaction(SIGALRM,&action,0);//注册函数

    alarm(2);//计时2s SIGALRM->调用akarm函数注册的时间  时间到了操作系统调用函数指针

    for(i=0;i<3;i++)
    {
        puts("wait..");
        sleep(100);
        //sleep 100sec , and the alarm will break it;
    }
    return 0;

}