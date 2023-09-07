#include <stdio.h>
#include <unistd.h>

//父子进程的gdb调试
//    1. gdb默认调试只跟踪父进程，比如断点在父进程等待，子进程会全部执行完
//    2. show/set follow-fork-mode child 可以显示/修改为跟踪子进程
//    3. set detach-on-fork on/off 非gdb跟踪的进程是不是暂时挂起
//       通过info inferior 查看和设置 inferiors id来切换调试的进程，\
         detach inferiors id (进程gdb 脱离调试)


int main() {

    printf("begin\n");

    if(fork() > 0) {

        printf("我是父进程：pid = %d, ppid = %d\n", getpid(), getppid());

        int i;
        for(i = 0; i < 10; i++) {
            printf("i = %d\n", i);
            sleep(1);
        }

    } else {

        printf("我是子进程：pid = %d, ppid = %d\n", getpid(), getppid());
        
        int j;
        for(j = 0; j < 10; j++) {
            printf("j = %d\n", j);
            sleep(1);
        }

    }

    return 0;
}