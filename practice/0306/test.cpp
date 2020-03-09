#include<iostream>
#include<unistd.h>
#include<stdio.h>
using namespace std;

int main()
{
    int ret = fork();
    if(ret < 0)
    {
        perror("fork");
        return 1;
    }
    else if(ret == 0)//子进程
    {
        cout<<"I am child："<<getpid()<<endl;
    }
    else//父进程
    {
        cout<<"I am father："<<getpid()<<endl;
    }
    return 0;
}

