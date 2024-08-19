#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>

int primes(int *fd){
    //first receive
    int receive;
    //later receive
    int childrec;
    //childproc不需要写入
    close(fd[1]);
    if(read(fd[0],(void*)&receive,sizeof(receive))!=sizeof(receive)){
        fprintf(2,"Read Error!!");
        exit(1);
    }
    printf("prime %d\n",receive);

    if(read(fd[0],(void*)&childrec,sizeof(childrec))){
        int fd1[2];
        pipe(fd1);
        if(fork()==0){
            primes(fd1);
        }else{
            close(fd1[0]);
            do{
                if(childrec % receive != 0){
                    write(fd1[1],(void*)&childrec,sizeof(childrec));
                }
            }while(read(fd[0],(void*)&childrec,sizeof(childrec)));
            close(fd[0]);
            close(fd1[1]);
            wait(0);
        }
    } 

    exit(0);
}

int main(int argc,char *argv[]){
    //get primes to 35
    int p[2];
    pipe(p);
    if(fork()==0){
        //main func
        primes(p);
    }else{
        for(int i=2;i<=35;i++){
            //parent proc不需要读取
            close(p[0]);
            //put all num in p
            if(write(p[1],(void *)&i,sizeof(i)) != 4){
                fprintf(2,"Write Error!");
                exit(1);
            }
  
        }
        //wirte finish
        close(p[1]);
        //wait child to exit
        wait(0);
    }
    exit(0);
}