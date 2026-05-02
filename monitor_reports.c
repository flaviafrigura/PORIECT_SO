#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>

void sigint_handler(int sig)
{
    printf("SIGINT recieved. Terminating.\n");
    unlink(".monitor_pid");
    exit(0);
}

void sigusr_handler(int sig)
{
    printf("SIGUSR1 recieved. Adding new report.\n");
}

int main()
{
    int file=open(",monitor_pid",O_CREAT|O_WRONLY|O_TRUNC,0644);
    if(file<0)
    {
        perror("Error with the PID file");
        return 1;
    }
    char buffer[32];
    sprintf(buffer,"%d\n",getpid());
    write(file,buffer,strlen(buffer));
    close(file);
    struct sigaction signalint,signalusr1;
    memset(&signalint,0,sizeof(signalint));
    signalint.sa_handler=sigint_handler;
    sigaction(SIGINT,&signalint,NULL);
    memset(&signalusr1,0,sizeof(signalusr1));
    signalusr1.sa_handler=sigusr_handler;
    sigaction(SIGUSR1,&signalusr1,NULL);
    while(1)
    {
        pause();
    }
    return 0;
}
