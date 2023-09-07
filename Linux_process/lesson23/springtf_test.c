#include<unistd.h>
#include<stdio.h>

int main(){
    for(int i = 0; i < 100; i++) {
        char buf[1024];
        sprintf(buf, "hello, %d\n", i);//给buf写入数据
        printf("write data : %s\n", buf);
        // write(fd, buf, strlen(buf));
        sleep(1);
    }

    return 0;
}