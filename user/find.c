#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* filename)
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if((fd = open(path, 0)) < 0){
	fprintf(2, "find: cannot open %s\n", path);
	return;
	}

	//fstat() 用来将参数filedes 所指向的文件状态复制到参数buf 所指向的结构
	if(fstat(fd, &st) < 0)	{
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
    		return;
  	}

	//参数错误，find的第一个参数必须是目录
	if (st.type != T_DIR) {
		fprintf(2, "usage: find <DIRECTORY> <filename>\n");
		return;
	}
	
	//检查路径长度是否适合存储在buf中
	if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
		printf("find: path too long\n");
		return;
	}
	strcpy(buf, path);
	p = buf+strlen(buf);
	*p++ = '/';
	while(read(fd, &de, sizeof(de)) == sizeof(de)) {
		if(de.inum == 0)
			continue;
	memmove(p, de.name, DIRSIZ);
	p[DIRSIZ] = 0;
	if(stat(buf, &st) < 0) {
		printf("find: cannot stat %s\n", buf);
		continue;
      }
	//不要在“.”和“..”目录中递归
	if (st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0) {
		find(buf, filename);
	} else if (strcmp(filename, p) == 0)
		printf("%s\n", buf);
	}
	close(fd);
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Parameter missing\n");
		exit(0);
	}
	
	find(argv[1], argv[2]);
	return 0;
}  
