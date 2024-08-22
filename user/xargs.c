#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>
#include <kernel/param.h>

#define MAXLEN 50

int main(int argc, char *argv[]) {
    char buf;
    char** paramsV;//参数数组
    int cntV, curind, flag;

    while (1) {
        // 为参数指针数组和每个参数分配内存
        paramsV = (char **)malloc(MAXARG * sizeof(char *));
        for(int i = 0; i < MAXARG; i++) {
            paramsV[i] = (char *)malloc(MAXLEN * sizeof(char));
        }
        // 初始化参数数组
        cntV = argc - 1; // 初始化参数数量为命令行参数数目
        curind = 0;      // 当前参数的字符索引
        flag = 0;        // 是否正在读取参数的标志

        // 先将命令行参数复制到 paramsV 数组中
        for (int i = 1; i < argc; i++) {
            strcpy(paramsV[i - 1], argv[i]);
        }

        // 从标准输入读取字符，按空格分割参数
        while (read(0, &buf, 1) > 0 && buf != '\n') {
            if (buf == ' ' && flag == 1) {
                cntV++;
                curind = 0;
                flag = 0;
            } else if (buf != ' ') {
                paramsV[cntV][curind++] = buf;
                flag = 1;
            }
        }

        // 当输入结束（EOF）时，退出循环
        if (flag == 0) {
            break;
        }

        paramsV[MAXARG - 1] = 0; // 确保参数数组最后一个元素为 NULL

        if (fork() == 0) {
            exec(argv[1], paramsV);
            exit(0); // 子进程执行完毕后退出
        } else {
            wait((int *)0); // 父进程等待子进程结束
        }
    }

    exit(0);
}
