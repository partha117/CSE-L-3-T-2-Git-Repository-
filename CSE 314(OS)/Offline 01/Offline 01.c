#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include"ArrayList.c"
typedef struct abc password;
struct abc
{   int id;
    int pass;
};

sem_t applicationFull,applicationEmpty,teacherBQueueStudent,teacherBQueueTeacher,teacherD,checkDone,onAvailable;
pthread_mutex_t teacherInsertLock,fullLock;
pthread_mutex_t queueLock,checkingPassword;
int asset=1;
//Queue contents Starts
int applicationQueue[10];
int full=0;
int empty=1;
int currentPos=0;
int teacherBQueue;
password approvedPassword[50];
int currnetPassword=0;
int fillId;
password GivenPassword;
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
   // printf(" pos: %d val:%d\n",currentPos,a);
    if(!full)
    {

        applicationQueue[currentPos]=a;
        //printf(" pos: %d val:%d\n",currentPos,a);
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
        applicationQueue[currentPos-1]=-1;
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
    //printf("student %d  starting\n",*((int *)arg));
    int checker=0;
    int c=*((int *)arg);
    for(;checker!=1;)
    {
        pthread_mutex_lock(&fullLock);
        if(full)
        {
           // printf("student %d  in Lock\n",c);
            sem_wait(&applicationFull);
           // printf("student %d  in unLock\n",c);
        }
        pthread_mutex_unlock(&fullLock);
        //printf("++++++++student %d is Applying++++++++++\n",c);
        checker=queueOperation(1,c);
        if(!checker)
        {
           // printf("%d-------------\n",c);
        }
        else
        {
            sleep(1);
           // printf("<<<<<<<<<<<<Successfully applied %d\n",c);
        }

    }
    sem_wait(&teacherBQueueStudent);
    teacherBQueue=c;
    sem_post(&teacherBQueueTeacher);
    for(;1;)
    {
        pthread_mutex_lock(&checkingPassword) ;
        //printf("checking now %d\n",c);
        fillId=c;
        sem_post(&teacherD);
        sem_wait(&checkDone);
        if(GivenPassword.id==c)
        {
            printf("->->->-Task Complete  Student %d<-<-<-\n",c);
            pthread_mutex_unlock(&checkingPassword) ;
            break;
        }
        pthread_mutex_unlock(&checkingPassword) ;
        sleep(1);
    }

}
void * teacherTask(void *arg)
{
    //printf("%s starts work\n",(char*)arg);
    for(;1;)
    {
       // if(empty)
       // {
           // printf("Now hehe\n");
            sem_wait(&applicationEmpty);
       // }
       // printQueue();
        int temp=queueOperation(0,-1);
        if(temp!=-1)
        {
            pthread_mutex_lock(&teacherInsertLock) ;
            insertItem(temp);
           // printf("%s got %d\n",(char*)arg,temp);
            sem_post(&onAvailable);
           // printQueue();
            pthread_mutex_unlock(&teacherInsertLock);
            sleep(1);
        }

    }

}

void *teacherBTask(void *arg)
{
    for(;1;)
    {
        sem_wait(&teacherBQueueTeacher);
        int temp=teacherBQueue;
        int val=0;
       // printf("Teacher B got %d >>>>>>>>>>\n",temp);
        for(;1;)
        {
            pthread_mutex_lock(&teacherInsertLock);
            val=hasDuplicate(temp);
            if(val==0)
            {
                pthread_mutex_unlock(&teacherInsertLock);
                //printf("Its %d \n",val);
               // printList();
                sem_wait(&onAvailable);
            }
            else if(val==1)
            {
                int temp2;
                approvedPassword[currnetPassword].id=temp;
                approvedPassword[currnetPassword].pass=temp2;
               // approvedPassword[currnetPassword].password=(char)(temp);
                currnetPassword++;
                deleteItem(temp);
                //printf("Its %d \n",temp);
                printList();
                pthread_mutex_unlock(&teacherInsertLock);
                break;
            }
            else if(val==2)
            {
                printf("<+<+<+Student %d password rejected+>+>+>+>\n",temp);

                //printList();
                pthread_mutex_unlock(&teacherInsertLock);
                break;
            }

        }

        sem_post(&teacherBQueueStudent);
    }

}
void *teacherDTask(void *arg)
{
    int i;
    for(;1;)
    {
        sem_wait(&teacherD);
        GivenPassword.id=-1;
        //printList();
        printf("Pinged by %d\n",fillId);
        for(i=0;i<currnetPassword;i++)
        {
            if(approvedPassword[i].id==fillId)
            {
                GivenPassword.id=fillId;
                GivenPassword.pass=approvedPassword[i].pass;
            }
           // printf("request: %d got:%d",fillId,approvedPassword[i]);
        }
        //printf("\n");
        sem_post(&checkDone);
    }
}
int main()
{
    void*  result;
    int i,ret=1;
    initializeQueue();
    initializeList();
    sem_init(&applicationFull,1,1);
    sem_init(&applicationEmpty,1,0);
    sem_init(&teacherBQueueStudent,1,1);
    sem_init(&teacherBQueueTeacher,1,0);
    sem_init(&teacherD,1,0);
    sem_init(&checkDone,1,0);
    sem_init(&onAvailable,1,0);

    pthread_mutex_init(&teacherInsertLock, NULL);
    pthread_mutex_init(&checkingPassword, NULL);
    pthread_mutex_init(&queueLock, NULL);
    pthread_mutex_init(&fullLock, NULL);
    pthread_t studentThread[40];
    int studentId[40];
    pthread_t teacherA;
    pthread_t teacherC;
    pthread_t teacherE;
    pthread_t teacherB;
    pthread_t teacherD;
    char * messageA="Teacher A";
    char * messageC="Teacher C";
    char * messageE="Teacher E";
    char * messageB="Teacher B";
    char * messageD="Teacher D";
    for(i=0;i<40;i++)
    {
        if(i==11)//not 12
        {
            studentId[i]=i;
        }
        else if(i==13)//not 14
        {
            studentId[i]=i;
        }
        else if(i==15)//not 16
        {
           studentId[i]=i;
        }
        else
        {
            studentId[i]=i+1;
        }
       // studentId[i]=(i);

        pthread_create(&studentThread[i],NULL,studentTask,(void*)(&studentId[i]));

        //pthread_join(studentThread[i],NULL);
    }

    pthread_create(&teacherA,NULL,teacherTask,(void*)(messageA) );
    pthread_create(&teacherC,NULL,teacherTask,(void*)(messageC) );
    pthread_create(&teacherE,NULL,teacherTask,(void*)(messageE) );
    pthread_create(&teacherB,NULL,teacherBTask,(void*)(messageB) );
    pthread_create(&teacherD,NULL,teacherDTask,(void*)(messageD) );
    //pthread_join(studentThread[1],&result);
    pthread_join(teacherA,&result);
    printQueue();
    while(1);


}
