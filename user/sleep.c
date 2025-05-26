#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/// @brief Sleep for a specified number of ticks.  1代表0.1s
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char **argv) 
{
	if(argc < 2)	
	{
		printf("usage: sleep <ticks>\n");
		exit(1);
	}
	int ticks = atoi(argv[1]); //这里atoi函数将字符串转换成正整数，不能取负数
	if(ticks < 0) 
	{
		printf("sleep: negative ticks %d\n", ticks);
		exit(1);
	}
	printf("slept for %d ticks\n", ticks);
	sleep(ticks);
	exit(0);
}