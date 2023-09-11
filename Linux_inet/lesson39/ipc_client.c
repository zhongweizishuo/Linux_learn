// 本地套接字通信
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main() {

    unlink("client.sock");
        //在程序运行的时候，不管有没有.sock文件，先把它删除，防止报错

    // 1.创建套接字
    int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(cfd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2.绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "client.sock");
    int ret = bind(cfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    //不需要监听

    // 3.连接服务器
    struct sockaddr_un seraddr;//服务器的地址信息
    seraddr.sun_family = AF_LOCAL;
    strcpy(seraddr.sun_path, "server.sock");


    ret = connect(cfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
    if(ret == -1) {
        perror("connect");
        exit(-1);
    }

    // 4.通信
    int num = 0;
    while(1) {

        // 发送数据
        char buf[128];
        sprintf(buf, "hello, i am client %d\n", num++);
        send(cfd, buf, strlen(buf) + 1, 0);
        printf("client say : %s\n", buf);

        // 接收数据
        int len = recv(cfd, buf, sizeof(buf), 0);

        if(len == -1) {//recv 报错
            perror("recv");
            exit(-1);
        } else if(len == 0) { //服务器关闭
            printf("server closed....\n");
            break;
        } else if(len > 0) {// 收到服务端的数据
            printf("server say : %s\n", buf);
        }

        sleep(1);

    }

    close(cfd);
    return 0;
}