//这个demo用于体会udp服务端通信的编程流程以及接口的使用
//1、创建套接字
//2、为套接字绑定地址信息
//3、接收数据
//4、发送数据
//5、关闭套接字

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>  //struct sockaddr_in 结构体的定义以及协议的宏
#include <arpa/inet.h> //一些字节序转换的接口
#include <sys/socket.h> //套接字接口

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("em: ./udp_srv 192.168.122.132 9000\n");
        return -1;
    }
    //argv[0] = ./udp_srv  argv[1] = 192.168.122.132  argv[2] = 9000
    const char *ip_addr = argv[1];//通过参数获取IP地址
    uint16_t port_addr = atoi(argv[2]);//通过参数获取端口数据

    //创建套接字
    //int socket(int domain, int type, int protocol);
    //其中domain表示地址域，AF_INET表示ipv4的网络地址结构，AF_INET6表示ipv6的网络地址结构
    //type表示套接字类型，SOCK_DGRAM表示数据报套接字，SOCK_STREAM表示流式套接字
    //protocol表示协议类型，IPPROTO_UDP表示UDP协议类型，IPPROTO_TCP 表示TCP协议类型
    //返回值：返回一个文件描述符的操作句柄，失败返回-1
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // 
    if(sockfd < 0)
    {
        perror("socket error");
        return -1;
    }

    //定义IPv4地址结构
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_addr);//网络字节序端口
    //字节序转换接口
    // uint32_t htonl(uint32_t hostlong); 将4个字节的整数转换为网络字节序整数
    //uint16_t htons(uint16_t hostshort); 将2个字节的整数转换为网络字节序整数
    //uint32_t ntohl(uint32_t netlong); 将4个字节的网络字节序整数转换为主机字节序的整数
    //uint16_t ntohs(uint16_t netshort); 将2个字节的网络字节序整数转换为主机字节序的整数
    //两个字节的整数，不能使用htonl/ntoh
    inet_pton(AF_INET, ip_addr, &addr.sin_addr.s_addr);
    int ret;
    socklen_t len = sizeof(struct sockaddr_in);
    ret = bind(sockfd, (struct sockaddr *) &addr, len);
    if(ret < 0)
    {
        perror("bind error");
        return -1;
    }

    //循环接收/发送数据
    while(1)
    {
        //作为服务端应该先接收数据
        //recvfrom(描述符，缓冲区，长度，参数，对端地址缓冲区，地址长度缓冲区)
        char buf[1024] = {0};
        struct sockaddr_in cliaddr;
        ret = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*) &cliaddr, &len);
        if(ret < 0)
        {
            perror("recvfrom error");
            close(sockfd);
            return -1;
        }
        char cli_ip[32] = {0};
        uint16_t cli_port = ntohs(cliaddr.sin_port);
        inet_ntop(AF_INET, &cliaddr.sin_addr, cli_ip, 32);
        printf("client[%s:%d] say:%s\n",cli_ip,cli_port,buf);

        printf("server say:");
        fflush(stdout);
        memset(buf, 0x00, 1024);//初始化buf缓冲区为0
        scanf("%s",buf);
        ret = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, len);
        if(ret < 0)
        {
            perror("sendto error");
            close(sockfd);
            return -1;
        }

    }
    close(sockfd);
    return 0;
}
