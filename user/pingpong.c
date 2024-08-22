#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>

int main(){
    int p[2];
    char buf[1];
    pipe(p);
    char* pchar="p";
    char* cchar="c";
  
    if(fork()>0){//parent
        if(write(p[1],pchar,1)!=1){
    	   fprintf(1,"parent can't write to pipe\n");
    	   exit(1);
    	}

    	close(p[1]);
    	
    	wait(0);
    	
    	if(read(p[0],buf,1)!=1){
    	   fprintf(1,"parent can't read from pipe\n");	
    	   exit(1);	
    	}
    	
   	printf("%d: received pong:%c\n",getpid(),buf[0]);
    	printf("%d: received pong\n",getpid());
    	close(p[0]);
    	
    	exit(0);
    }else if(fork()==0){//child
        if(read(p[0],buf,1)!=1){
    	   fprintf(1,"child can't read from pipe\n");	
    	   exit(1);	
    	}
    	printf("%d: received ping:%c\n",getpid(),buf[0]);
    	printf("%d: received ping\n",getpid());
    	close(p[0]);
    	
    	if(write(p[1],cchar,1)!=1){
    	   fprintf(1,"child can't write to pipe\n");
    	   exit(1);
    	}

    	close(p[1]);
    	
    	exit(0);
    }
    exit(0);
}