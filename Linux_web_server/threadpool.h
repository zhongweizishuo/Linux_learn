#ifndef THREADPOOL_H
#define THREADPOLL_H

#include <pthread.h>
#include <list>
#include "locker.h"
#include <cstdio>

// 线程池类，定义为template，是为了代码复用
template <typename T>
class threadpool
{
public:
	// 函数声明：类内声明
	threadpool(int thread_number = 8, int max_requests = 10000);
	~threadpool();
	bool append(T *request);

private:
    /*工作线程运行的函数，它不断从工作队列中取出任务并执行之*/
    static void* worker(void* arg);
    void run();

private:
	// 线程的数量
	int m_thread_number;

	// 线程池的容器，大小为m_thread_number
	pthread_t *m_threads;

	// 请求队列中最多允许的，等待处理请求数量
	int m_max_requests;

	// 请求队列
	std::list<T *> m_workqueue;

	// 互斥锁
	locker m_queuelocker;

	// 信号量用来判断是否有任务需要处理
	sem m_queuestat;

	// 是否结束线程
	bool m_stop;
};

//构造函数的实现
template <typename T>
threadpool<T>::threadpool(int thread_number, int max_requests) : // 使用：进行初始化参数
																 m_thread_number(thread_number),
																 m_max_requests(max_requests),
																 m_stop(flase),
																 m_thread(NULL)
{

	// 传入的两个数据检测，不合适就抛出异常
	if ((thread_number <= 0) || (max_requests <= 0))
	{
		throw std::exception();
	}

	// 动态创建数组，申请内存
	m_threads = new pthread_t[m_thread_number];
	if (!m_threads)
	{
		throw std::exception();
	}
	// 创建thread_number个线程，并设置线程分离
	for (int i = 0; i < thread_number, i++)
	{
		printf("create the %dth thread\n",i);
		if (pthread_create(m_threads+i, NULL, worker, this) !=0){
			//创建线程失败的话，释放线程内存，并throw错误；
			delete [] m_thread;
			throw std::exception();
		}

		if(pthread_detach(m_thread[i])){
			delete [] m_threads;
			throw std::exception();
		}
	}
}

//析构函数
template<typename T>
threadpool<T>::~threadpool(){
	delete[] m_threads;
	m_stop = true;
}

//成员函数append()
template<typename T>
bool threadpool<T>::append(T* request){
    // 操作工作队列时一定要加锁，因为它被所有线程共享。
    m_queuelocker.lock();
    if ( m_workqueue.size() > m_max_requests ) {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

// 静态函数worker()
template< typename T >
void* threadpool< T >::worker( void* arg )
{
    threadpool* pool = ( threadpool* )arg;
    pool->run();
    return pool;
}

// 成员函数run()
template< typename T >
void threadpool< T >::run() {

    while (!m_stop) {
        m_queuestat.wait();
        m_queuelocker.lock();
        if ( m_workqueue.empty() ) {
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if ( !request ) {
            continue;
        }
        request->process();
    }
}

#endif /*threadpool.h*/
