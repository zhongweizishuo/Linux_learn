// 多进程TCP通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <error.h>
#include <errno.h>

void recyleChild(int arg){
    while(1)
    {
        int num = waitpid(-1, NULL ,WNOHANG);
        if(num == -1){
            //所有子进程全部回收
            break;
        }else if(num == 0){
            //还有子进程活着
            break;
        }else if(num >0){
            //回收
            printf("子进程%d被回收了\n",num);
        }
    }
}

int main()
{
    //0. 注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recyleChild;
    sigaction(SIGCHLD, &act, NULL);

    // 1. 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
            exit(-1);
    }

    // 2.绑定
    struct sockaddr_in saddr; // server addr
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

    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
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

    // 不断循环等待客户端连接
    while (1)
    {

        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        // 接受连接
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len); // cfd = communicate fd
        if (cfd == -1)
        {
            if(errno == EINTR){
                continue;
            }
            
            perror("accept");
            exit(-1);
        }

        // 每一个连接进来，创建一个子进程跟客户端进行通信
        pid_t pid = fork();
        if (pid == 0)
        {
            // 子进程

            // 获取客户端的信息
            char cliIP[16];
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cliIP, sizeof(cliIP));
            unsigned short cliPort = ntohs(cliaddr.sin_port);
            printf("client ip is %s, port is %d.\n", cliIP, cliPort);

            // 接受客户端发来的数据
            char recvBuf[1024] = {0};
            while (1)
            {

                int num = read(cfd, recvBuf, sizeof(recvBuf));
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

                write(cfd, recvBuf, strlen(recvBuf));
            }

            // 关闭文件描述符
            close(cfd);
            exit(0);
        }
    }
    close(lfd);
    return 0;
}