/*
    使用多线程实现卖票的案例
    有3个窗口，一共100张票
    多线程资源抢夺，资源竞争；解决办法，多线程同步（同步指的是多个线程的先后顺序关系）
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int num = 1000;//全局变量，总数是100张；

void * sellTicket(void *){
    printf("--------------------------sell ticket running...--------------------------\n");

    // int num = 100;//局部变量，每个线程是独立的，所以总数是300张；
    while(num >0){
        usleep(6000);//睡眠多少微秒;更容易导致线程抢占的问题显示出来
        printf("%ld 正在卖第%d张票\n",pthread_self(),num);
        num--;
    }
    return NULL;//不写也可以
}

int main(){
    //创建3个窗口
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1,NULL,sellTicket,NULL);
    pthread_create(&tid2,NULL,sellTicket,NULL);
    pthread_create(&tid3,NULL,sellTicket,NULL);

    //回收子线程资源
    //方式一：阻塞回收
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    //方式二：detach之后，自动回收
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);
    
    
    
    //设置main thread exit,但是不影响子线程
    pthread_exit(NULL);
    
    return 0;
}