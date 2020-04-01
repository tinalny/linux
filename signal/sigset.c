#include <stdio.h>
#include <unistd.h>
#include <signal.h>
//int sigemptyset(sigset_t *set);//清空信号集
//int sigismember(const sigset_t *set, int signum);//判断signum是否在信号集set中
//int sigaddset(sigset_t *set, int signum);//向信号集set中添加信号
//int sigdelset(sigset_t *set, int signum);//从信号集set中删除信号

int main()
{
    sigset_t set;

    sigemptyset(&set);//清空set信号集

    //向信号集中添加SIGINT信号，
    sigaddset(&set,SIGINT);
    //向信号集中添加SIGQUIT信号
    sigaddset(&set,SIGQUIT);

    //判断SIGINT信号是否在信号集中
    //若存在则返回1，若不存在返回0
    int ret = sigismember(&set,SIGINT);
    if(ret == 1)
    {
        printf("信号集set中存在该信号\n");
    }
    if(ret == 0)
    {
        printf("信号集set中不存在该信号\n");
    }
    return 0;
}
