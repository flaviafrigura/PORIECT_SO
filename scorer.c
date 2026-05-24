#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>

#define NAME_LEN 32
#define CATEGORY_LEN 32
#define DESC_LEN 128
#define MAX_INSPECTORS 100

typedef struct
{
    int id;
    char inspector[NAME_LEN];
    float latitude;
    float longitude;
    char category[CATEGORY_LEN];
    int severity;
    time_t timestamp;
    char description[DESC_LEN];
}report;

typedef struct
{
    char name[NAME_LEN];
    int totalscore;
}inspector_score;

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        printf("Problema incepere program scorer\n");
        return 1;
    }
    char path[256];
    sprintf(path,"%s/reports.dat",argv[1]);
    int file=open(path,O_RDONLY);
    if(file<0)
    {
        perror("Problema deschidere reports.dat");
        return 1;
    }
    inspector_score inspectors[MAX_INSPECTORS];
    int inspectorcount=0;
    report r;
    while(read(file,&r,sizeof(report))>0)
    {
        int found=0;
        for(int i=0;i<inspectorcount;i++)
        {
            if(strcmp(inspectors[i].name,r.inspector)==0)
            {
                inspectors[i].totalscore+=r.severity;
                found=1;
                break;
            }
        }
        if(!found&&inspectorcount<MAX_INSPECTORS)
        {
            strcpy(inspectors[inspectorcount].name,r.inspector);
            inspectors[inspectorcount].totalscore=r.severity;
            inspectorcount++;
        }
    }
    close(file);
    printf("District: %s\n",argv[1]);
    for(int i=0;i<inspectorcount;i++)
    {
        printf("Inspector: %s | Workload Score %d\n",inspectors[i].name,inspectors[i].totalscore);
    }
    return 0;
}
