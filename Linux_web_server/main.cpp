#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"

#define MAX_FD 65536   // 最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000  // 监听的最大的事件数量

// 添加文件描述符
extern void addfd( int epollfd, int fd, bool one_shot );
extern void removefd( int epollfd, int fd );

// 信号处理：添加信号
void addsig(int sig, void( handler )(int)){ //函数指针
    struct sigaction sa;//注册信号
    memset( &sa, '\0', sizeof( sa ) );//清空sa,用‘\0’填充
    sa.sa_handler = handler;
    sigfillset( &sa.sa_mask );
    // sigaction(sig, &sa, NULL);//注册信号
    assert( sigaction( sig, &sa, NULL ) != -1 );
}

int main( int argc, char* argv[] ) {
    
    if( argc <= 1 ) {
        printf( "usage: %s port_number\n", basename(argv[0]));
        return 1;
    }

    // 获取端口号，atoi,将string 转为 int
    int port = atoi( argv[1] );// argv[1]存放端口号
    // 对SIGPIE信号进行处理
    addsig( SIGPIPE, SIG_IGN );

    // 创建线程池，初始化线程池
    threadpool< http_conn >* pool = NULL;
    try {
        pool = new threadpool<http_conn>;
    } catch( ... ) {
        return 1;
    }

    // 创建数组users用于保存所有的客户端信息
    http_conn* users = new http_conn[ MAX_FD ];

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );

    // 地址信息
    int ret = 0;
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons( port );

    // 设置端口复用
    int reuse = 1;
    setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    // 绑定
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    // 监听
    ret = listen( listenfd, 5 );

    // 创建epoll对象，和事件数组，添加
    epoll_event events[ MAX_EVENT_NUMBER ];// 最大的监听数量
    int epollfd = epoll_create( 5 );
    // 添加到epoll对象中
    addfd( epollfd, listenfd, false );
    http_conn::m_epollfd = epollfd;

    while(true) {
        
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        
        if ( ( number < 0 ) && ( errno != EINTR ) ) {
            printf( "epoll failure\n" );
            break;
        }
        // 循环遍历事件数组
        for ( int i = 0; i < number; i++ ) {
            
            int sockfd = events[i].data.fd;
            
            if( sockfd == listenfd ) {
                // 有客户端连接进来
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                
                if ( connfd < 0 ) {
                    printf( "errno is: %d\n", errno );
                    continue;
                } 

                if( http_conn::m_user_count >= MAX_FD ) {
                    // 目前连接数满了
                    // 给客户端写一个信息：服务器内部正忙
                    close(connfd);
                    continue;
                }
                // 将新的客户数据初始化，放到数组中
                users[connfd].init( connfd, client_address);

            } else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) ) { //对方异常断开或者错误等事件
                // 对方异常断开，使用自定义函数关闭连接
                users[sockfd].close_conn();

            } else if(events[i].events & EPOLLIN) {
                // 读事件的检测，数据读取
                if(users[sockfd].read()) {
                    // 一次性读完数据，交给工作线程处理
                    pool->append(users + sockfd);
                } else {
                    // 没有读取，就关闭
                    users[sockfd].close_conn();
                }

            }  else if( events[i].events & EPOLLOUT ) {
                // 写事件的检测
                if( !users[sockfd].write() ) {// 一次写完所有数据
                    users[sockfd].close_conn();
                }

            }
        }
    }
    // 关闭与资源释放
    close( epollfd );
    close( listenfd );
    delete [] users;
    delete pool;
    return 0;
}