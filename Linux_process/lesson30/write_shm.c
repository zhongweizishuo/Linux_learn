#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() {    

    // 1.创建一个共享内存
    int shmid = shmget(100, 4096, IPC_CREAT|0664);
    printf("shmid : %d\n", shmid);
    
    // 2.和当前进程进行关联
    void * ptr = shmat(shmid, NULL, 0);

    //2.1 准备数据
    char * str = "helloworld";

    // 3.写数据
    memcpy(ptr, str, strlen(str) + 1);//字符串结束符号也要拷贝

    printf("按任意键继续\n");
    getchar();//程序暂停，在此处使用read_shm.c来读取share memory

    // 4.解除关联
    shmdt(ptr);

    // 5.删除共享内存
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
