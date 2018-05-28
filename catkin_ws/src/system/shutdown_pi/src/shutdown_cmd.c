#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t pid;
    if((pid = fork()) < 0)
    {
        printf("fork error!\n");
    }
    else if (pid == 0)
    {
        if(execl("/sbin/shutdown","shutdown","-h","now",(char *)0) < 0)
        {
            printf("excel error!\n");
        }
    }

    if(waitpid(pid,NULL,0) < 0)
    {
        printf("wait error!.\n");
    }

    printf("finish!\n");
    return 0;
}

