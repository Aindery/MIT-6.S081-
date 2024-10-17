#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
	char buf[512] = {0};//滑动窗口
	char *xargv[MAXARG] = {0};
	uint size = 0;//字节大小
	int endIndex = 0;//字段末尾标识符
	
	for (int i = 1; i < argc; i++) {
		xargv[i - 1] = argv[i];
  	}

	while (!(endIndex && size == 0)) {
		if (!endIndex) {
			int readBytes = read(0, buf + size, 512 - size);
			if (readBytes < 0) {
				printf(" read error \n");
				exit(0);
			}
			/*如果读取到 0 字节（即已到达标准输入的末尾），则关闭标准输入并设置为真*/
			if (readBytes == 0) {
				close(0);
				endIndex = 1;
			}
			size += readBytes;
		}
		/*处理读取的行*/
		char* lineEnd = strchr(buf, '\n');//每一行的结束
		while (lineEnd) {
			char xbuf[513] = {0};
			memcpy(xbuf, buf, lineEnd - buf);
			xargv[argc - 1] = xbuf;
			int pid = fork();
			if (pid == 0) {
				/*子进程使用 exec 执行由 argv[1] 指定的程序，并将 xargv 作为参数传递*/
				if (!endIndex) 
					close(0);
				if (exec(argv[1], xargv) < 0) {
					fprintf(2, "xargv: exec fails [file ...]\n");
					exit(1);
				}
			} else if (pid > 0) {
				/*父进程从滑动窗口中移除已处理的行*/
				memmove(buf, lineEnd + 1, size - (lineEnd - buf) - 1);
				size -= lineEnd - buf + 1;
				memset(buf + size, 0, 512 - size);
				
				/*回收僵尸*/
				int status;
				int ret = wait(&status);
				//成功返回0 失败返回-1
				if( ret < 0) {
					printf("wait error");
					exit(1);
				}
				/*继续查找和处理下一行*/
				lineEnd = strchr(buf, '\n');
			} else {
				printf("%d: received ping \n", getpid());
				exit(0);
			}
		}
	}
	exit(0);
}


