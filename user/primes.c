#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void primes(int (*pipeline)[2])
{
    close((*pipeline)[1]);
    int first_num = 0;
    read((*pipeline)[0], &first_num, sizeof(int)); // 读取第一个数
    fprintf(1, "prime %d\n", first_num);

    int np_c[2];
    pipe(np_c);
    if(fork() == 0) // child's child
    {
        primes(&np_c);
    }
    else
    {
        close(np_c[0]);
        int num;
        while(read((*pipeline)[0], &num, sizeof(int)))
        {
            if(num % first_num != 0) // 能整除的drop掉
            {
                write(np_c[1], &num, sizeof(int));
            }
        }
        close(np_c[1]);
        wait(0);
    }
}

int
main(int argc, char *argv[])
{
    if(argc >= 2)
    {
        fprintf(2, "usage: no arguments\n");
        exit(1);
    }  
    //2 - 35
    int p_c[2];
    pipe(p_c);
    if(fork() == 0) // child
    {
        primes(&p_c);
    }
    else
    {
        close(p_c[0]);
        int count = 35;
        for (int i = 2; i <= count; i++)
        {
            write(p_c[1], &i, sizeof(int));
        }
        close(p_c[1]);
        wait(0);
    }
    exit(0);
}
