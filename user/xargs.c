// #include "kernel/param.h"
// #include "kernel/types.h"
// #include "user/user.h"

// // max exec argument  MAXARG 

// int
// main(int argc, char *argv[])
// {
//     char *args[MAXARG];
//     int i;
//     char buf[512];

//     for(i = 1; i < argc; i++)
//     {
//         args[i - 1] = argv[i];
//     }
//     int n = 0;
//     while(1)
//     {
//         char *p = buf;
//         while((n == read(0, p, 1)) && (*p != '\n'))
//         {
//             p++;
//         }
//         *p = '\0';
//         if(p != buf)
//         {
//             if(fork() == 0)
//             {
//                 args[argc - 1] = buf;
//                 args[argc] = 0;
//                 exec(argv[1], args);
//                 printf("no printed\n");
//             }
//             wait(0);
//         }
//         if(n <= 0)
//             break;
//     }
//     exit(0);
// }


#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  char inputBuf[32];
  char charBuf[320];
  char *charBufPointer = charBuf;
  int charBufSize = 0;

  char *commandToken[32];
  int tokenSize = argc - 1;
  int inputSize = -1;

  for (int tokenIdx = 0; tokenIdx < tokenSize; tokenIdx++)
    commandToken[tokenIdx] = argv[tokenIdx + 1];

  while ((inputSize = read(0, inputBuf, sizeof(inputBuf))) > 0)
  {
    for (int i = 0; i < inputSize; i++)
    {
      char curChar = inputBuf[i];
      if (curChar == '\n')
      {
        charBuf[charBufSize] = 0;
        commandToken[tokenSize++] = charBufPointer;
        commandToken[tokenSize] = 0;

        if (fork() == 0)
        {
          exec(argv[1], commandToken);
        }
        wait(0);
        tokenSize = argc - 1;
        charBufSize = 0;
        charBufPointer = charBuf;
      }
      else if (curChar == ' ')
      {
        charBuf[charBufSize++] = 0;
        commandToken[tokenSize++] = charBufPointer;
        charBufPointer = charBuf + charBufSize;
      }
      else
      {
        charBuf[charBufSize++] = curChar;
      }
    }
  }
  exit(0);
}
