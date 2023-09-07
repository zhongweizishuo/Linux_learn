/*
    #include <unistd.h>
    int access(const char *pathname, int mode);
        作用：判断某个文件是否有某个权限，或者判断文件是否存在
        参数：
            - pathname: 判断的文件路径
            - mode:
                R_OK: 判断是否有读权限
                W_OK: 判断是否有写权限
                X_OK: 判断是否有执行权限
                F_OK: 判断文件是否存在
        返回值：成功返回0， 失败返回-1
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main() {

#if 0
    int ret = access("a.txt", F_OK);
    if(ret == -1) {
        perror("access");
    }
    else{
        printf("文件存在！！!\n");
    }


    // chang mod =chmod() 
    int result = chmod("a.txt",0777);//修改权限，从0665改为0775
    if (result== -1){
        perror("chmod");
        return -1;
    }
#endif

    //chang own 修改用户 =chown()
    int res = chown("access.c",1000,1001);
    if (res== -1){
        perror("chown");
        return -1;
    }

    return 0;
}
