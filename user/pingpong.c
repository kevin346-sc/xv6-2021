#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

int
main(int argc, char *argv[])
{
    if(argc >= 2)
    {
        fprintf(2, "usage: no arguments\n");
        exit(1);
    }  
    int p_c[2], c_p[2];
    pipe(p_c);
    pipe(c_p);
    char buf = 'a';
    int pid = fork();
    if(pid < 0)
    {
        fprintf(2, "fork() error!\n");
        close(p_c[WR]);
        close(p_c[RD]);
        close(c_p[WR]);
        close(c_p[RD]);
        exit(-1);
    }
    else if(pid == 0) // child
    {
        close(p_c[WR]); // 关闭父写
        close(c_p[RD]); // 关闭父读
        
        read(p_c[RD], &buf, sizeof(char) );
        fprintf(1, "%d: received ping\n", getpid());
        write(c_p[WR], &buf, 1);

        close(p_c[RD]);
        close(c_p[WR]);
        exit(0);
    }
    else
    {
        close(p_c[RD]);
        close(c_p[WR]);

        write(p_c[WR], &buf, sizeof(char));
        read(c_p[RD], &buf, sizeof(char));
        fprintf(1, "%d: received pong\n", getpid());

        close(p_c[WR]);
        close(c_p[RD]);
        exit(0);
    }
    exit(0);
}
