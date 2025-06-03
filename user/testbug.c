#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"

// 输入ctrl+D结束输入
// 这个程序从标准输入读取数据，并将其写入标准输出，直到遇到EOF（Ctrl+D）为止
// 标准输入是我在命令行中输入的内容，标准输出是我在命令行中看到的内容
int main(int argc, char **argv)
{

    // char* argv[2];
    argv[0] = "cat";
    argv[1] = 0;
    if (fork() == 0) {
        close(0);
        open("input1.txt", O_RDONLY);
        exec("cat", argv);
    }
    exit(0);
}