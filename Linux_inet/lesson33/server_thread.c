// 多线程并发实现TCP通信

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct sockInfo
{
    /* 结构体给working传多个参数 */
    int fd;                  // 通信的文件描述符
    pthread_t tid;           // 线程号
    struct sockaddr_in addr; // 传入地址结构体
};
struct sockInfo sockinfos[128];

void * working(void * arg)
{
    // 子线程与客户端通信    cfd、客户端的信息、线程号之类的传参数使用结构体
    // 获取客户端的信息
    struct sockInfo* pinfo = (struct sockInfo *)arg;

    char cliIP[16];
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, cliIP, sizeof(cliIP));
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    printf("client ip is %s, port is %d.\n", cliIP, cliPort);

    // 接受客户端发来的数据
    char recvBuf[1024];
    while (1)
    {
        int num = read(pinfo->fd, &recvBuf, sizeof(recvBuf));
        if (num == -1)
        {
            perror("read");
            exit(-1);
        }
        else if (num > 0)
        {
            printf("recv client %s\n", recvBuf);
        }
        else if (num == 0)
        {
            // 表示客户端断开连接
            printf("client closed...");
            break;//客户端结束时，应该退出循环
        }
        // 5.2 给客户端发送数据

        write(pinfo->tid, recvBuf, strlen(recvBuf)+1);
    }

    // 关闭文件描述符
    close(pinfo->tid);
  
}

int main()
{

    // 1. 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    /*
int lfd：这部分代码声明了一个整数变量 lfd，它将用于存储套接字的文件描述符。
文件描述符是一个非负整数，用于标识打开文件或套接字等文件系统资源。
socket(AF_INET, SOCK_STREAM, 0)：这是创建套接字的系统调用，它接受三个参数：

AF_INET：这是地址族（Address Family）的常量，表示使用IPv4地址族。它告诉操作系统使用IPv4协议来进行通信。
SOCK_STREAM：这是套接字类型（Socket Type）的常量，表示创建一个流式套接字（stream socket），也就是TCP套接字。
流式套接字提供了可靠的、面向连接的、基于流的数据传输。
0：这是套接字的协议类型，通常为0表示使用默认协议，根据前面的 AF_INET 参数来选择合适的协议（在IPv4的情况下通常是TCP协议）。
socket 函数返回一个整数值，即文件描述符，该文件描述符用于后续的套接字操作，例如绑定、监听、接受连接等。
    */
    if (lfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 2.绑定
    struct sockaddr_in saddr; // server addr: 使用结构体表示服务器的地址信息
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    /*
    这段代码用于设置一个 `struct sockaddr_in` 结构体，该结构体用于表示服务器的地址信息。以下是对代码的解释：

    1. `struct sockaddr_in saddr;`：这一行定义了一个名为 `saddr` 的 `struct sockaddr_in` 结构体变量，
    用于存储服务器的地址信息。`sockaddr_in` 结构体通常用于IPv4网络通信。

    2. `saddr.sin_family = AF_INET;`：这行代码将 `sin_family` 字段设置为 `AF_INET`，表示该地址族为IPv4。
    `AF_INET` 是IPv4地址族的常量。

    3. `saddr.sin_port = htons(9999);`：这行代码将 `sin_port` 字段设置为服务器的端口号。
    `htons` 函数用于将主机字节序（可能是大端或小端）转换为网络字节序（大端字节序），以确保在网络上正确传输端口号。
    在这里，端口号被设置为9999。

    4. `saddr.sin_addr.s_addr = INADDR_ANY;
    `：这行代码将 `sin_addr.s_addr` 字段设置为 `INADDR_ANY`，表示服务器将接受来自任何可用网络接口的连接请求。
    这通常用于服务器希望监听所有可用网络接口的情况。

    综合起来，这段代码设置了一个 `struct sockaddr_in` 结构体，
    其中包括了服务器的地址族（IPv4）、端口号（9999），以及接受来自任何可用网络接口的连接请求。
    这是在网络编程中设置服务器地址信息的常见方式，通常在创建服务器套接字并绑定到特定地址后使用。
    */

    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr)); // 地址结构体的大小，通常是 struct sockaddr_in 的大小
    if (ret == -1)                                                 // 如果出现错误，则返回一个负数，错误码存储在 errno 变量中
    {
        perror("bind");
        exit(-1);
    }

    // 3. 监听
    ret = listen(lfd, 8); // 8是监听队列里面的个数
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }

    // 初始化数据为0
    int max = sizeof(sockinfos) / sizeof(sockinfos[0]);
    for (int i = 0; i < max; i++)
    {
        bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }

    // 循环等待客户端连接，一旦一个客户端连接进来，就创建一个子线程
    while (1)
    {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        // 接受连接
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len); // cfd = communicate fd
        // if (cfd == -1)
        // {
        //     if (errno == EINTR)
        //     {
        //         continue;
        //     }
        //     perror("accept");
        //     exit(-1);
        // }

        // 每一个连接进来，创建一个子线程跟客户端进行通信
        // pthread_t tid;

        // pthread_create 给回调函数working传参数使用的结构体进行实例化
        struct sockInfo *p_info;
        for (int i = 0; i < max; i++)
        {
            // 从这个数组中找到一个可以用的sockInfo元素
            if (sockinfos[i].fd == -1)
            {
                p_info = &sockinfos[i];
                break;
            }
            if (i == max - 1)
            {
                sleep(1);
                i--;
            }
        }
        p_info->fd= cfd;
        memcpy(&p_info->addr, &cliaddr, len);

        pthread_create(&p_info->tid, NULL, working, p_info);
        pthread_detach(p_info->tid);
    }
    close(lfd);

    return 0;
}
