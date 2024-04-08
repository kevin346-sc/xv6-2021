#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
ls(char *path, char *file_name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type != T_DIR) // 第一个参数必须是目录
  {
    fprintf(2, "find usage: can't read file in file");
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("ls: path too long\n");
  }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
        continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
    }
    // 递归遍历目录
    if(st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0)
    {
        ls(buf, file_name);
    }
    else if(strcmp(p, file_name) == 0)
    {
        printf("%s\n", buf);
    }
    }
  close(fd);
}


int
main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(2, "usage: find {where} {name}\n");
        exit(1);
    }  
    ls(argv[1], argv[2]);
    exit(0);
}
