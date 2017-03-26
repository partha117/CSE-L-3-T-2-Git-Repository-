#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
sem_t applicationFull,applicationEmpty;
pthread_mutex_t teacherNotifier,studentNotifier;
pthread_mutex_t queueLock;
int asset=1;
//Queue contents Starts
int applicationQueue[10];
int full=0;
int empty=1;
int currentPos=0;
void initializeQueue()
{
    int i;
    for(i=0;i<10;i++)
    {
        applicationQueue[i]=-1;
    }
}
int  Enqueue(int a)
{
   // pthread_mutex_lock(&teacherNotifier);
    printf(" pos: %d val:%d\n",currentPos,a);
    if(!full)
    {

        applicationQueue[currentPos]=a;
        printf(" pos: %d val:%d\n",currentPos,a);
        currentPos++;
        if(currentPos>=10)
        {
            full=1;
        }
        if(empty)
        {
            //printf("Now here in enq %d\n",empty);
            //
            empty=0;

        }
        sem_post(&applicationEmpty);
        return 1;

    }
    else
    {
        printf("----------------------------FAILED Student %d is failed to submit\n",a);
        return 0;
    }
    //pthread_mutex_unlock(&teacherNotifier);
}
int  Dequeue()
{
    if(!empty)
    {
       // pthread_mutex_lock(&studentNotifier);
        int temp=applicationQueue[0];
        int i;
        for(i=1;i<currentPos;i++)
        {
            applicationQueue[i-1]=applicationQueue[i];

        }
        currentPos--;
        if(currentPos<=0)
        {
            empty=1;
        }

        if(full)
        {

            full=0;
            sem_post(&applicationFull);
        }
       // pthread_mutex_unlock(&studentNotifier);
        return temp;
    }
    else
    {
        return -1;
    }
}
void printQueue()
{

    if(currentPos)
    {
        printf("Current size %d\n",currentPos);
        int i;
        for(i=0;i<currentPos;i++)
        {
            printf("%d  ",applicationQueue[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Queue empty :Size 0\n");
    }
}
//1 enqueue, o dequeue
int queueOperation(int code,int val)
{
    pthread_mutex_lock(&queueLock);
    int temp;
    if(code==1)
    {
        temp=Enqueue(val);

    }
    else if(code==0)
    {
        temp=Dequeue();
    }
    int temp2=temp;
    pthread_mutex_unlock(&queueLock);
    return temp;
}
//Queue contents end
void * studentTask(void * arg)
{
    printf("student %d  starting\n",*((int *)arg));
    int checker=0;
    int c=*((int *)arg);
    for(;checker!=1;)
    {
        if(full)
        {
            sem_wait(&applicationFull);
        }
        printf("---------------------------student %d is Applying\n",c);
        checker=queueOperation(1,c);
        if(!checker)
        {
            printf("%d-------------\n",c);
        }
        else
        {
            sleep(1);
        }

    }
    printf("------------Task Complete  %d\n",c);
}
void * teacherTask(void *arg)
{
    printf("%s starts work\n",(char*)arg);
    for(;1;)
    {
       // if(empty)
       // {
           // printf("Now hehe\n");
            sem_wait(&applicationEmpty);
       // }
       // printQueue();
        printf("%s got %d\n",(char*)arg,queueOperation(0,-1));
        printQueue();
        sleep(1);
    }

}

int main()
{
    void*  result;
    int i,ret=1;
    initializeQueue();
    sem_init(&applicationFull,1,1);
    sem_init(&applicationEmpty,1,0);
    pthread_mutex_init(&teacherNotifier, NULL);
    pthread_mutex_init(&studentNotifier, NULL);
    pthread_mutex_init(&queueLock, NULL);
    pthread_t studentThread[30];
    int studentId[30];
    pthread_t teacherA;
    pthread_t teacherC;
    pthread_t teacherE;
    char * messageA="Teacher A";
    char * messageC="Teacher C";
    char * messageE="Teacher E";
    for(i=0;i<30;i++)
    {
        studentId[i]=i+1;
        pthread_create(&studentThread[i],NULL,studentTask,(void*)(&studentId[i]));
        //pthread_join(studentThread[i],NULL);
    }
    pthread_create(&teacherA,NULL,teacherTask,(void*)(messageA) );
    pthread_create(&teacherC,NULL,teacherTask,(void*)(messageC) );
    pthread_create(&teacherE,NULL,teacherTask,(void*)(messageE) );
    pthread_join(studentThread[1],&result);
    pthread_join(teacherA,&result);
    printQueue();
    while(1);


}
