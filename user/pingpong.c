#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc,char *agrv[]){
   int p1[2];
   int p2[2];
   pipe(p1);
   pipe(p2);
   int pid = fork();
   if(pid==0){
     close(p1[1]);
     close(p2[0]);
     char son[2];
     read(p1[0],son,1);
     close(p1[0]);
     printf("%d: received ping\n",getpid());
     write(p2[1],"a",1);
     close(p2[1]);
   }else if(pid>0){
     close(p1[0]);
     close(p2[1]);
     write(p1[1],"a",1);
     close(p1[1]);
     char father[2];
     read(p2[0],father,1);
     printf("%d: receive pong\n",getpid());
     close(p2[0]);
   }
   exit(0);
}
