#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
/*
   int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
   功能：
   检查或修改信号阻塞集，根据 how 指定的方法对进程的阻塞集合进行修改，新的信号阻塞集由 set 指定，
   而原先的信号阻塞集合由 oldset 保存。

   参数：
how: 信号阻塞集合的修改方法，有 3 种情况：
SIG_BLOCK：向信号阻塞集合中添加 set 信号集，新的信号掩码是set和旧信号掩码的并集。
SIG_UNBLOCK：从信号阻塞集合中删除 set 信号集，从当前信号掩码中去除 set 中的信号。
SIG_SETMASK：将信号阻塞集合设为 set 信号集，相当于原来信号阻塞集的内容清空，然后按照set
中的信号重新设置信号阻塞集。

set: 要操作的信号集地址。
若 set 为 NULL，则不改变信号阻塞集合，函数只把当前信号阻塞集合保存到 oldset 中。

oldset: 
保存原先信号阻塞集地址

返回值：
成功：0，
失败：-1，失败时错误代码只可能是 EINVAL，表示参数 how 不合法。
*/

void fun(int signo)
{
    printf("\033[31mcatch signal %d\033[0m\n", signo);
}

int main(void)
{
    int i = 0;
    sigset_t set;
    sigset_t oldset;

    signal(SIGINT, fun);
    signal(SIGQUIT, fun);

    printf("================================\n");
    sigemptyset(&set);
    if(-1 == sigprocmask(SIG_BLOCK, &set, &oldset))
    {
        printf("sigprocmask failed...\n"); 
        return 0;
    }

    sleep(5);
    if(-1 == sigprocmask(SIG_SETMASK, &oldset, NULL))
    {
        printf("sigprocmask failed...\n"); 
        return 0;
    }
    
    return 1;
}
