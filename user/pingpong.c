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
    if(fork() == 0) // child
    {
        close(p_c[1]); // 关闭父写
        close(c_p[0]); // 关闭父读
        
        read(p_c[0], &buf, sizeof(char) );
        fprintf(1, "%d: received ping\n", getpid());
        write(c_p[1], &buf, 1);

        close(p_c[0]);
        close(c_p[1]);
    }
    else
    {
        close(p_c[0]);
        close(c_p[1]);

        write(p_c[1], &buf, sizeof(char));
        fprintf(1, "%d: received pong\n", getpid());
        read(c_p[0], &buf, sizeof(char));

        close(p_c[1]);
        close(c_p[0]);
    }
    exit(0);
}
