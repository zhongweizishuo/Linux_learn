/*
    #include <pthread.h>
    int pthread_join(pthread_t thread, void **retval); //**retval是一个二级指针
        - 功能：和一个已经终止的线程进行连接
                子线程结束之后，回收子线程的资源；否则导致产生僵尸线程
                这个函数是阻塞函数，类似wait(),调用一次只能回收一个子线程
                一般在主线程中使用
        - 参数：
            - thread：需要回收的子线程的ID
            - retval: 接收子线程退出时的返回值
        - 返回值：
            0 : 成功
            非0 : 失败，返回的错误号
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int value = 10;//全局变量

void * callback(void * arg) {
    printf("child thread id : %ld\n", pthread_self());
    // sleep(3);
    // return NULL; 
    int value = 10; // 局部变量,随着线程回收会被销毁，main线程无法收到；所以要使用全局变量
    pthread_exit((void *)&value);   // return (void *)&value;  //&value是int * ，强转为需要的void *
} 

int main() {

    // 创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error : %s\n", errstr);
    }

    // 主线程
    printf("main thread startted ...");
    for(int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }

    printf("tid : %ld, main thread id : %ld\n", tid ,pthread_self());

    // 主线程调用pthread_join()回收子线程的资源
    int * thread_retval;//定义线程返回值
    ret = pthread_join(tid, (void **)&thread_retval);//&thread_value是int * ，强转为需要的void *

    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error : %s\n", errstr);
    }

    printf("exit data : %d\n", *thread_retval);

    printf("回收子线程资源成功！\n");

    // 让主线程退出,当主线程退出时，不会影响其他正常运行的线程。
    pthread_exit(NULL);

    return 0; 
}

