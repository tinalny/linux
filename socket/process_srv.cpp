//使用封装的tcosocket类实现tcp服务端程序
//1、创建套接字
//2、绑定地址信息
//3、开始监听
//4、获取新连接
//5、通过新连接接收数据
//6、通过新连接发送数据
//7、关闭套接字

#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int signo)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("em: ./tcp_srv host_ip host_port\n");
        return -1;
    }
    //对SIGCHLD信号自定义处理，等到子进程退出的时候处理一下就可以了
    //这样避免了父进程一直等待的情况
    signal(SIGCHLD,sigcb);
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket lst_sock;
    //在tcpsocket.hpp中定义宏 #define CHECK_RET(q) if((q) == false) {return -1;}
    //下面这条语句相当于if(lst_sock.Socket() == false) {return -1;}
    CHECK_RET(lst_sock.Socket());//创建套接字
    CHECK_RET(lst_sock.Bind(ip, port));//绑定地址信息
    CHECK_RET(lst_sock.Listen());//开始监听
    while(1)
    {
        TcpSocket newsock;
        std::string cli_ip;
        uint16_t cli_port;
        bool ret = lst_sock.Accept(&newsock, &cli_ip, &cli_port);//获取新连接
        if(ret == false)
        {
            continue;//服务端不会因为一次获取失败而退出程序，而是继续获取下一个
        }
        printf("new conn:[%s:%d]\n",cli_ip.c_str(),cli_port);
        pid_t pid = fork();
        if(pid == 0)
        {//每个子进程负责与客户端进行循环通信
            while(1)
            {
                std::string buf;
                ret = newsock.Recv(&buf);//通过新连接接收数据
                if(ret == false)
                {
                    newsock.Close();
                }
                printf("client say:%s\n",buf.c_str());

                printf("server say:");
                fflush(stdout);
                buf.clear();
                std::cin>>buf;
                ret = newsock.Send(buf);//通过新连接发送数据
                if(ret == false)
                {
                    newsock.Close();
                    continue;
                }
            }
            newsock.Close();
            exit(0);
        }
        //注意：新的套接字创建在创建子进程之前，因此父进程各自独有一份
        //父进程并不与客户端进行通信，因此直接关闭新的套接字，关闭的是自己的，并不影响子进程
        newsock.Close();
        waitpid(-1,NULL,0);
    }
    lst_sock.Close();
    return 0;
}
