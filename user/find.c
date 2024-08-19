#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *path,char* filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  //打开路径
  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  //读取文件状态
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
    
  //读取目录信息
  if(read(fd,&de,sizeof(de)) != sizeof(de)){
    exit(1);
  } 

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    if(strcmp(de.name,filename)==0){
        printf("%s/%s\n", fmtname(path), filename);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || strcmp(de.name,".")==0 || strcmp(de.name,"..")==0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      
      if(st.type == T_FILE){
        if(strcmp(de.name,filename) == 0){
            printf("%s\n",buf);
        }
      }else if(st.type == T_DIR){
        find(buf,filename);
      }

    }
    break;
  }
  close(fd);
}

int main(int argc,char *argv[]){
    if(argc<2){
        fprintf(2,"args too less,at least 3 args\n");
        exit(1);
    }else{
        find(argv[1],argv[2]);
        exit(0);
    }
}