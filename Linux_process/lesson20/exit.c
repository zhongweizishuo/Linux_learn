/*
    #include <stdlib.h>
    void exit(int status);//标准C库的函数会多做一点事情，会多刷新缓存

    #include <unistd.h>
    void _exit(int status);

    status参数：是进程退出时的一个状态信息。父进程回收子进程资源的时候可以获取到。
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    printf("hello\n");
    printf("world");//换行符会自动刷新printf的缓存,因为标准C库的函数都带有缓存

    exit(0);//hello world ；会刷新缓存区
    // _exit(0);//hello；不会刷新缓存区域，所以不打印world
    
    return 0;//相当于exit(0),都是进程结束
}
