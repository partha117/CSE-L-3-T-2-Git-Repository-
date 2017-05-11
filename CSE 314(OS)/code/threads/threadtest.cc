// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "producer.h"
#include "consumer.h"
#include "synch.h"
#include "thread.h"
// testnum is set in main.cc
int testnum = 2;
PCbuffer *buffer;
Lock *lock;
Condition *condition;
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------


void producerThreadTask(int arg)
{
	int temp;
	producer p("producer",buffer,arg);
	bool prev;
	for(int i=0;i<30;i++)
	{
		//printf("Producer It is %d\n",arg);
		lock->Acquire();

		for(;1;)
		{
			if(buffer->full())
			{
				condition->Wait(lock);
			}
			else
			{
				break;
			}
		}
		prev=buffer->empty();
		temp=123;
		for(int j=0;j<temp;j++)
		{

		}

		//buffer->printBuffer();
		p.produce();
		//buffer->printBuffer();

		if(prev)
		{
			printf("<<<<<<<<producer broadcast>>>>>>>>>>\n");
			condition->Broadcast(lock);
		}
		lock->Release();
		for(int k=0;k<temp;k++)
		{

		}
	}


}
void consumerThreadTask(int arg)
{
	int temp;
	consumer c("consumer",buffer);
	bool prev;
	for(int i=0;i<30;i++)
	{
		//printf("Consumer It is %d\n",arg);
		lock->Acquire();
		//printf("Consumer here\n");
		for(;1;)
		{
			if(buffer->empty())
			{
				condition->Wait(lock);
			}
			else
			{
				break;
			}
		}
		prev=buffer->full();
		temp=123;
		for(int l=0;l<temp;l++)
		{

		}
		c.consume();
		if(prev)
		{
			printf("<<<<<<<<consumer broadcast>>>>>>>>>>\n");
			condition->Broadcast(lock);
		}
		lock->Release();
		for(int m=0;m<temp;m++)
		{

		}


	}
}
void myTest()
{
	lock=new Lock("myLock");
	condition=new Condition("myCondition");
	buffer=new PCbuffer();
	Thread *t1 = new Thread("Consumer1");
	Thread *t2 = new Thread("Consumer2");
	Thread *t3 = new Thread("Consumer3");
	Thread *t4 = new Thread("Consumer4");
	Thread *t5 = new Thread("Producer1");
	Thread *t6 = new Thread("Producer2");
	Thread *t7 = new Thread("Producer3");
	Thread *t8 = new Thread("Producer4");
	t1->Fork(&consumerThreadTask,1);
	t2->Fork(&consumerThreadTask,2);
	t3->Fork(&consumerThreadTask,3);
	t4->Fork(&consumerThreadTask,4);

	t5->Fork(&producerThreadTask,5);
	t6->Fork(&producerThreadTask,6);
	t7->Fork(&producerThreadTask,7);
	t8->Fork(&producerThreadTask,8);


}

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    case 2:
    myTest();
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}

