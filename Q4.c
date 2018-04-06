#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>

struct Process
{
	int time,arrivaltime,bursttime,id,priority,waittime;
	clock_t arrival;
	int flag,completed,p;
	sem_t sema;
	struct Process *nextnode;
};

int i=0,k=0;
typedef struct Process node;
clock_t start;
float A_TAT=0,A_waittime=0;
node *front=NULL,*rear=NULL,*temp;
void *processor(node *A) {
	clock_t count;
	while(1) {
		sem_wait(&A->sema);
		if((A->arrivaltime<=(clock()-start)/CLOCKS_PER_SEC && A->p==1)) 
		{
			A->p=0;
			count=clock();
		}
		if(A->flag==1) {
			printf("\n Process-%d Running ",A->id);
			printf("\n clock value is :%d",(clock()-start)/CLOCKS_PER_SEC);
			A->flag=0;
			A->arrival=clock();
		}
		if((clock()-count)/CLOCKS_PER_SEC==1) {
			count=clock();
			printf("\n Clock value is  : %d",(clock()-start)/CLOCKS_PER_SEC);
			A->time-=1;
			if(A->time==0) {
				A->waittime=((clock()-start)/CLOCKS_PER_SEC)-A->bursttime-A->arrivaltime;
				A_TAT+=(clock()-start)/CLOCKS_PER_SEC-A->arrivaltime;
				A_waittime+=((clock()-start)/CLOCKS_PER_SEC)-A->bursttime-A->arrivaltime;
				sleep(2);
				node *start=rear;
				while(start!=NULL) {
					if(start->nextnode==A) {
						start->nextnode=A->nextnode;
						break;
					}
					if(rear==A) {
						rear=rear->nextnode;
						break;
					}
					start=start->nextnode;
				}
				printf("\nProcess %d Completed ",A->id);
				if(rear!=NULL){
					printf("next Process %d",rear->id);
				}
				A->completed=7;
				if(rear!=NULL){
					sem_post(&rear->sema);
				}
			}
			}
					if(A->completed==7) {
			break;
		}
	sem_post(&A->sema);
	}
} 
void priority(node *temp) {
	int k;
	node *start=rear;
	k=temp->priority;
	k=1+(temp->waittime/temp->time);
	if(rear==NULL) {
		rear=temp;
		rear->nextnode=NULL;
	}
	else{
	int t=temp->time;
	if(start->priority<k)
	{
		temp->nextnode = rear;
        rear=temp;
	}
	else if (start->time > t) {
        	temp->nextnode = rear;
        	rear=temp;
    	}
    else {
        	while (start->nextnode != NULL && start->nextnode->time< t) {
            		start = start->nextnode;
        	}
        temp->nextnode = start->nextnode;
        start->nextnode = temp;
    }
	}
}
void sort() {
	temp=(node *)malloc(sizeof(node));
	printf("\nEnter Arrival Time of Process  %d:",(i+1));
	scanf("%d",&temp->arrivaltime);
	printf("Enter Burst Time :");
	scanf("%d",&temp->time);
	temp->id=i+1;
	temp->p=1;
	temp->flag=1;
	temp->bursttime=temp->time;
	temp->priority=1+(temp->waittime/temp->time);
	sem_init(&temp->sema,0,0);
	int t=temp->arrivaltime;
	node *start=front;
	if (start->arrivaltime > t) {
        	temp->nextnode = front;
        	front=temp;
    	}
    	else {
        	while (start->nextnode != NULL && start->nextnode->arrivaltime < t) {
            		start = start->nextnode;
        	}
        temp->nextnode = start->nextnode;
        start->nextnode = temp;
    }
}

void main() 
{
	int n,l=1;
	pthread_t p[20];
	printf("\nEnter No.of Processes You want to create :");
	scanf("%d",&n);
	while(i<n) {
		if(front==NULL) {
			front=(node *)malloc(sizeof(node));
			printf("Enter Arrival Time of Process  %d  :",(i+1));
			scanf("%d",&front->arrivaltime);
			printf("Enter Burst Time of process:");
			scanf("%d",&front->time);
			front->id=i+1;
			front->flag=1;
			front->p=1;
			front->waittime=0;
			front->bursttime=front->time;
			front->priority=1+(front->waittime/front->time);
			sem_init(&front->sema,0,0);
			front->nextnode=NULL;
		}
		else {
			sort();
		}
		i++;
	}
	i=0;
	
	start=clock();
	while(i<n) {
		temp=front;	
		if(temp->arrivaltime<=0) {
			printf("Process id discarded because arrival time is zero",temp->id);
			front=temp->nextnode;
			temp=front;
			i++;
		}
		if(l==1) {
				l=0;
				sem_post(&temp->sema);
			}
		if((clock()-start)/CLOCKS_PER_SEC==temp->arrivaltime) 
		{
			pthread_create(&p[i],NULL,processor,temp);
			front=front->nextnode;
			priority(temp);
			i++;
		}
	}
	for(i=0;i<n;i++) {
		pthread_join(p[i],NULL);
	}
	printf("\nAverage Turn Around Time :%f",(float)A_TAT/n);
}                                                                                                                                                   
