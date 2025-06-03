#include   "kernel/types.h"
#include   "kernel/stat.h"
#include   "user/user.h"


int main(int argc, char **argv) 
{
    //创建两个管道
    int p2c[2]; //0是读端；1是写端
    int c2p[2];
    pipe(p2c);
    pipe(c2p);

    int pid = fork(); //创建子进程0
    if (pid == 0) 
    {
        // 子进程
        close(p2c[1]); // 关闭父->子管道的写端（子进程不需要）
        close(c2p[0]); // 关闭子->父管道的读端（子进程不需要）

        char buf[1];
        while (read(p2c[0], buf, 1) > 0) 
        {
            printf("%d: received %d\n", getpid(), buf[0]); // 打印接收到的数据
            write(c2p[1], buf, 1); // 回复父进程
        }
        // 关闭管道并退出
        close(p2c[0]);
        close(c2p[1]);
        exit(0);
    } 
    else 
    {
        // 父进程
        close(p2c[0]); // 关闭父->子管道的读端（父进程不需要）
        close(c2p[1]); // 关闭子->父管道的写端（父进程不需要）

        for (int i = 0; i < 10; i++) 
        {
            write(p2c[1], &i, 1); // 向子进程发送数据
            char buf[1];
            read(c2p[0], buf, 1); // 等待子进程回复
            printf("%d: sent %d\n", getpid(), i);
        }
        // 关闭管道并回收子进程
        close(p2c[1]);
        close(c2p[0]);
        wait(0); // 等待子进程结束
    }
    exit(0); // 退出父进程
}