// TCP通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main()
{

    // 1. 创建套接字(用于监听的套接字)
    int lfd = socket(AF_INET, SOCK_STREAM, 0); //lfd = listen fd
    if (lfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 2. 绑定 bind()

    struct sockaddr_in saddr; // 地址结构体
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "192.168.111.131", saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = 0; // 指定任意地址，此处的零代表0.0.0.0，或者使用INADDR_ANY代表0
    saddr.sin_port = htons(9999);

    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    // 3. 监听
    ret = listen(lfd, 8);
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }

    // 4. 接受客户端连接
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);

    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len); //cfd = communicate fd
    if (cfd == -1)
    {
        perror("accept");
        exit(-1);
    }
    // 输出客户端的信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s, port is %d.\n", clientIP, clientPort);

    // 5. 通信
    // 5.1 获取客户端的数据
    char recvBuf[1024] = {0};
    while(1){
        
        int num = read(cfd, recvBuf, sizeof(recvBuf));
        if (num == -1)
        {
            perror("read");
            exit(-1);
        }
        else if (num > 0)
        {
            printf("recv client data: %s\n", recvBuf);
        }
        else if (num == 0)
        {
            // 表示客户端断开连接
            printf("client closed...");
            break;
        }
        // 5.2 给客户端发送数据
        char * data = "hello i am server";
        write(cfd, data, strlen(data));
    }



    //关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}