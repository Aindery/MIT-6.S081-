#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int readp,int writep){
     char jug[36];
     int target = 0;
     read(readp,jug,sizeof jug);
     for(int i = 0 ; i < 35; i++){
     	if(jug[i] == '0'){
     	    target = i;
     	    break;
     	}
     }
     if(target == 0) exit(0);
     printf("prime: %d\n",target);
     jug[target] = '1';
     
     for(int i = 0 ; i<35;i++){
     	if(i%target == 0){
     	   jug[i] = '1';
     	}	
     }
     
     if(fork() >0){
 	write(writep,jug,sizeof jug);
 	wait(0);    
     }
     else{
     	prime(readp,writep);
     	wait(0);
     }
}

int main(int argc,int *argv[]){
   char jug[36];
   memset(jug,'0',sizeof jug);
   int p[2];
   pipe(p);
   
   if(fork()>0){
   	jug[0] = jug[1] = '1';
   	write(p[1],jug,sizeof jug);
   	wait(0);	
   }
   else{
        prime(p[0],p[1]);
        wait(0);
   }
   exit(0);
}

