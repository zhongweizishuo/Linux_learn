#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>
#include <exception> //throw()的头文件
#include <semaphore.h>//信号量头文件

// 线程同步机制封装类,将一些库函数进行二次封装

// 1. 互斥锁类
class locker
{
public:
    locker()
    { // 构造函数
        if (pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            throw std::exception();
        }
    }

    ~locker()
    { // 析构函数
        pthread_mutex_destroy(&m_mutex);
    }

    // 上锁
    bool lock()
    {
        return pthread_mutex_lock(&m_mutex) == 0;
    }
    // 解锁
    bool unlock()
    {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }
    // 获得mutex的内容
    pthread_mutex_t *get()
    {
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex; // 成员
};

// 2. 条件变量类(配合互斥锁使用)
class cond
{
public:
    cond()
    {
        if (pthread_cond_init(&m_cond, NULL) != 0)
        {
            throw std::exception();
        }
    }

    ~cond()
    {
        pthread_cond_destroy(&m_cond);
    }

    bool wait(pthread_mutex_t *mutex)
    {
        return pthread_cond_wait(&m_cond, mutex) == 0;
    }

    bool timedwait(pthread_mutex_t *mutex, struct timespec t)
    {
        return pthread_cond_timedwait(&m_cond, mutex, &t) == 0;
    }

    // 唤醒某个线程
    bool singal()
    {
        return pthread_cond_signal(&m_cond) == 0;
    }
    // 唤醒多个线程
    bool broadcast()
    {
        return pthread_cond_broadcast(&m_cond) == 0;
    }

private:
    pthread_cond_t m_cond;
};

// 3. 信号量有关的类
class sem
{
public:
    // 构造与析构函数
    sem()
    {
        if (sem_init(&m_sem, 0, 0) != 0)
        {
            throw std::exception();
        }
    }

    sem(int num)
    {
        if (sem_init(&m_sem, 0, num) != 0)
        {
            throw std::exception();
        }
    }

    ~sem()
    {   
        sem_destroy(&m_sem);
    }

    //等待信号量
    bool wait(){
        return sem_wait(&m_sem)==0;
    }

    //增加信号量
    bool post(){
        return sem_post(&m_sem);
    }

private:
    sem_t m_sem;
};

#endif
