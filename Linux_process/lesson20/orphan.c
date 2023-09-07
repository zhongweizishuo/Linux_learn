#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // 创建子进程
    pid_t pid = fork();

    // 判断是父进程还是子进程
    if(pid > 0) {

        // 如果大于0，返回的是创建的子进程的进程号，当前是父进程
        printf("i am parent process, pid : %d, ppid : %d\n", getpid(), getppid());

    } else if(pid == 0) {
        // 当前是子进程
        sleep(2);
        printf("i am child process, pid : %d, ppid : %d\n", getpid(),getppid());
        printf("child process is over.\n");
    }

    // for循环;父子进程都可以执行的代码,抢占式交替执行
    for(int i = 0; i < 3; i++) {
        printf("for loop; i = %d , pid = %d, ppid : %d\n", i , getpid(),getppid());
        sleep(1);
    }
    return 0;
}


