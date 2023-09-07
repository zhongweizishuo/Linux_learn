#include <stdio.h>
#include <unistd.h>

int main(){
    while(1){
        printf("hello\n");
        sleep(1);//linux 是1 s

    }
    return 0;
}

//可以使用kill -9 pid 杀死