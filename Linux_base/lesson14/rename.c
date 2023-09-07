/*
    #include <stdio.h>
    int rename(const char *oldpath, const char *newpath);

*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

    int res = open("aaa.txt", O_WRONLY | O_CREAT, 0664);
    if(res ==-1){
        perror("open");
        return -1;
    }

    int ret = rename("../lession13/b.txt", "aaa.txt");//将lession13的b.txt重命名为aaa.txt，然后移动到lesson14

    if(ret == -1) {
        perror("rename");
        return -1;
    }

    return 0;

}
