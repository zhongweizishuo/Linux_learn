/*
    信号量的类型 sem_t
    信号量其实是⼀个整型的计数器，主要⽤于实现进程间的互斥与同步，⽽不是⽤于缓存进程间通信的数据;
    信号量的大小是不是决定了容器的大小
    
    int sem_init(sem_t *sem, int pshared, unsigned int value);
        - 初始化信号量
        - 参数：
            - sem : 信号量变量的地址
            - pshared : 0 用在线程间 ，非0 用在进程间
            - value : 信号量中的值

    int sem_destroy(sem_t *sem);
        - 释放资源

    int sem_wait(sem_t *sem);
        - 对信号量加锁，调用一次对信号量的值-1，如果值为0，就阻塞

    int sem_trywait(sem_t *sem);

    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
    int sem_post(sem_t *sem);
        - 对信号量解锁，调用一次对信号量的值+1

    int sem_getvalue(sem_t *sem, int *sval);

    sem_t psem;
    sem_t csem;
    init(psem, 0, 8);
    init(csem, 0, 0);

    producer() {
        sem_wait(&psem);//对信号量加锁，调用一次对信号量的值-1，如果值为0，就阻塞
        sem_post(&csem);//对信号量解锁，调用一次对信号量的值+1
    }

    customer() {
        sem_wait(&csem);//对信号量加锁，调用一次对信号量的值-1，如果值为0，就阻塞
        sem_post(&psem);//对信号量解锁，调用一次对信号量的值+1
    }

*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

// 创建一个互斥量
pthread_mutex_t mutex;
// 创建两个信号量,全局变量，所有线程都可以访问
sem_t psem;
sem_t csem;

struct Node{
    int num;
    struct Node *next;
};

// 头结点
struct Node * head = NULL;


void * producer(void * arg) {

    // 不断的创建新的节点，添加到链表中
    while(1) {
        sem_wait(&psem);//加锁，-1
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("add node, num : %d, tid : %ld\n", newNode->num, pthread_self());
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);//解锁，+1
    }

    return NULL;
}

void * customer(void * arg) {

    while(1) {
        sem_wait(&csem);//加锁，-1
        pthread_mutex_lock(&mutex);
        // 保存头结点的指针
        struct Node * tmp = head;
        head = head->next;
        printf("del node, num : %d, tid : %ld\n", tmp->num, pthread_self());
        free(tmp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);//解锁，+1
       
    }
    return  NULL;
}

int main() {

    // 互斥锁与信号量的初始化
    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 2);//单线程最多加2个，信号量的大小是不是决定了容器的大小
    sem_init(&csem, 0, 0);

    // 创建5个生产者线程，和5个消费者线程，设置线程分离
    pthread_t ptids[5], ctids[5];

    for(int i = 0; i < 5; i++) {
        pthread_create(&ptids[i], NULL, producer, NULL);
        pthread_create(&ctids[i], NULL, customer, NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }

    //主线程等待，防止互斥锁立即销毁
    while(1) {
        sleep(10);
    }

    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);

    return 0;
}

