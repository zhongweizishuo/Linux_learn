#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>

//使用ftok()生成shmget()使用的key

int main(){
    char * pathname = "/home/zhong/Desktop/Linux_process/lesson30/a.txt";
    int proj_id = 'a';

    
    key_t key =  ftok(pathname, proj_id);
    printf("key = %d\n",key);
    return 0;
}