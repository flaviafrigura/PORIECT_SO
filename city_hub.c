#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

typedef struct{
    char name[100];
    int totalscore;
    int count;
}inspector;

void start_monitor()
{
    int filedes[2];
    if(pipe(filedes)<0)
    {
        perror("Problema la crearea pipeului");
        return;
    }
    pid_t hub_mon=fork();
    if(hub_mon<0)
    {
        perror("Problema la crearea child process hub_mon");
        close(filedes[0]);
        close(filedes[1]);
        return;
    }
    if(hub_mon==0)
    {
        pid_t monitor=fork();
        if(monitor<0)
        {
            perror("Problema la crearea child process monitor");
            exit(1);
        }
        if(monitor==0)
        {
            close(filedes[0]);
            dup2(filedes[1],STDOUT_FILENO);
            close(filedes[1]);
            execl("./monitor_reports","monitor_reports",NULL);
            perror("Eroare cand incercam sa inlocuim ./monitor_reports");
            exit(1);
        }
        close(filedes[1]);
        char buffer[512];
        int bytesread;
        while((bytesread=read(filedes[0],buffer,sizeof(buffer)-1))>0)
        {
            buffer[bytesread]='\0';
            printf("MONITOR: %s",buffer);
            fflush(stdout);
            if(strstr(buffer,"TERMINAT:")!=NULL)
            {
                printf("HUB: Monitor terminat\n");
                fflush(stdout);
            }
        }
        close(filedes[0]);
        wait(NULL);
        exit(0);
    }
    close(filedes[0]);
    close(filedes[1]);
    printf("Monitor activ\n");
}

void calculate_scores(char *districts[],int count)
{
    inspector persons[200];
    int person_count=0;
    for(int i=0;i<count;i++)
    {
        int filedes[2];
        if(pipe(filedes)<0)
        {
            perror("Problema la crearea pipeului");
            continue;
        }
        pid_t scorer=fork();
        if(scorer<0)
        {
            perror("Problema la creerea procesului");
            close(filedes[0]);
            close(filedes[1]);
            continue;
        }
        if(scorer==0)
        {
            close(filedes[0]);
            dup2(filedes[1],STDOUT_FILENO);
            close(filedes[1]);
            execl("./scorer","scorer",districts[i],NULL);
            perror("Problema la exec scorer");
            exit(1);
        }
        close(filedes[1]);
        char buffer[4096];
        int bytesread;
        char output[32768]="";
        while((bytesread=read(filedes[0],buffer,sizeof(buffer)-1))>0)
        {
            buffer[bytesread]='\0';
            printf("%s",buffer);
            strncat(output,buffer,sizeof(output)-strlen(output)-1);
        }
        close(filedes[0]);
        waitpid(scorer,NULL,0);
        char *line=strtok(output,"\n");
        while(line!=NULL)
        {
            char name[100];
            int score;
            if(sscanf(line,"Inspector: %s | Workload Score %d",name,&score)==2)
            {
                int found=-1;
                for(int j=0;j<person_count;j++)
                {
                    if(strcmp(persons[j].name,name)==0)
                    {
                        found=j;
                        break;
                    }
                }
                if(found==-1)
                {
                    strcpy(persons[person_count].name,name);
                    persons[person_count].totalscore=score;
                    persons[person_count].count=1;
                    person_count++;
                }
                else
                {
                    persons[found].totalscore+=score;
                    persons[found].count++;
                }
            }
            line=strtok(NULL,"\n");
        }
    }
    printf("Total:\n");
    if(person_count == 0)
    {
        printf("Nu au fost gasiti inspectori\n");
        return;
    }
    for(int i=0;i<person_count;i++)
    {
        printf("%10s | %2d | %d\n",persons[i].name,persons[i].totalscore,persons[i].count);
    }
    printf("Total inspectori: %d\n",person_count);
    int total=0;
    for(int i=0;i<person_count;i++)
    {
        total+=persons[i].totalscore;
    }
    printf("Workload-ul combinat pentru toti inspectorii: %d\n",total);
}

int main()
{
    char command[512];
    while(1)
    {
        printf("city_hub> ");
        fflush(stdout);
        if(fgets(command,sizeof(command),stdin)==NULL)
            break;
        command[strcspn(command,"\n")]='\0';
        if(strcmp(command,"exit")==0)
            break;
        else if(strcmp(command,"start_monitor")==0)
            start_monitor();
        else if(strncmp(command,"calculate_scores",16)==0)
        {
            char *districts[20];
            int count=0;
            char *p=strtok(command," ");
            p=strtok(NULL," ");
            while(p!=NULL&&count<20)
            {
                districts[count++]=p;
                p=strtok(NULL," ");
            }
            if(count==0)
                printf("Nu ai denumit niciun district\n");
            else
                calculate_scores(districts,count);
        }
        else
            printf("Comanda necunoscuta\n");
    }
    return 0;
}
