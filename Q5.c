#include<stdio.h>
#include<pthread.h>
struct record
{
       int p,arrivaltime,bursttime,waittime,turnaround,runtime;
};


void sortarrivaltime(struct record a[],int processes)   //selection sort
{
       int i,j;
       struct record temp;
       for(i=0;i<processes;i++)
       {
              for(j=i+1;j<processes;j++)
              {
                     if(a[i].arrivaltime > a[j].arrivaltime)
                     {
                           temp = a[i];
                           a[i] = a[j];
                           a[j] = temp;
                     }
              }
       }
       return;
}

int main()
{
	
       int i,j,processes,time,remain,flag=0,ts;
       struct record a[7];
       float avgturnaround=0;
       printf("Round robin algorithm \n");
       printf("Number Of processes");
       scanf("%d",&processes);
       remain=processes;
       for(i=0;i<processes;i++)
       {
              printf("Enter arrival time and Burst time for processes P%d : ",i);
              scanf("%d%d",&a[i].arrivaltime,&a[i].bursttime);
              a[i].p = i;
              a[i].runtime = a[i].bursttime;
       }
       sortarrivaltime(a,processes);
       printf("Enter Time Quantum for each process: ");
       scanf("%d",&ts);
       for(time=0,i=0;remain!=0;)
       {
              if(a[i].runtime<=ts && a[i].runtime>0)
              {
                     time = time + a[i].runtime;
                     a[i].runtime=0;
                     flag=1;
              }
              else if(a[i].runtime > 0)
              {
                     a[i].runtime = a[i].runtime - ts;
                     time = time + ts;
              }
              if(a[i].runtime==0 && flag==1)
              {
                     remain--;
                     a[i].turnaround = time-a[i].arrivaltime;
                     a[i].waittime = time-a[i].arrivaltime-a[i].bursttime;
                     avgturnaround = avgturnaround + time-a[i].arrivaltime;
                     flag=0;
              }
              if(i==processes-1)
                     i=0;
              else if(a[i+1].arrivaltime <= time)
                     i++;
              else
                     i=0;
       }
       printf("\n processes\t	arrivaltime\t	bursttime\t	turnaround\t	waittime\n");
       printf("************************************************************************************************************\n");
       for(i=0;i<processes;i++)
       {
              printf("P%d\t		%d\t		%d\t		%d\t		%d\n",a[i].p,a[i].arrivaltime,a[i].bursttime,a[i].turnaround,a[i].waittime);
       }
       avgturnaround = avgturnaround/processes;
       printf("Average Turnaround Time : %.2f 	\n",avgturnaround);
}
