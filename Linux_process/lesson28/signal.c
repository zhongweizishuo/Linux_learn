/*
    #include <signal.h>
    typedef void (*sighandler_t)(int);
    sighandler_t signal(int signum, sighandler_t handler);
        - 功能：设置某个信号的捕捉行为(信号捕捉+捕捉之后处理)
        - 参数：
            - signum: 要捕捉的信号
            - handler: 捕捉到信号要如何处理
                - SIG_IGN ： ignore 忽略信号
                - SIG_DFL ： default 使用信号默认的行为
                - 回调函数 :  这个函数是内核调用，程序员只负责写捕捉到信号后如何去处理信号。
                回调函数：
                    - 需要程序员实现，提前准备好的，函数的类型根据实际需求，看函数指针的定义
                    - 不是程序员调用，而是当信号产生，由内核调用
                    - 函数指针是实现回调的手段，函数实现之后，将函数名放到函数指针的位置就可以了。

        - 返回值：
            成功，返回上一次注册的信号处理函数的地址。第一次调用返回NULL
            失败，返回SIG_ERR，设置错误号
            
    SIGKILL SIGSTOP不能被捕捉，不能被忽略。
    因为，SIGKILL是强制kill，权限很高，如果捕捉或者忽略，导致最高权限被忽视
*/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void myalarm(int num) {
    printf("捕捉到了信号的编号是：%d\n", num);
    printf("xxxxxxx\n");
}

// 过3秒以后，每隔2秒钟定时一次
int main() {

    // 注册信号捕捉
    
    // signal(SIGALRM, SIG_IGN);//捕捉并忽略
    // signal(SIGALRM, SIG_DFL);//捕捉并按默认处理
    signal(SIGALRM, myalarm);//使用回调函数设置信号捕捉，当捕捉到之后，，执行myalarm
        // void (*sighandler_t)(int); 函数指针，（返回值是void,参数是int）int类型的参数表示捕捉到的信号的值。

    struct itimerval new_value;

    //需求：定时器开始之后，过3秒以后，每隔1秒钟定时一次
    // 设置间隔的时间
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_usec = 0;

    // 设置延迟的时间,3秒之后开始第一次定时
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL); // 非阻塞的
    printf("定时器开始了...\n");

    if(ret == -1) {
        perror("setitimer");
        exit(0);
    }

    getchar();

    return 0;
}