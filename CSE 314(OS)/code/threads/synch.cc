// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName)
{
	name=debugName;
	condition=Free;
	waitQueue=new List();
	holderThread=NULL;
}
Lock::~Lock()
{
	delete waitQueue;
}
void Lock::Acquire()
{
	//printf("%s is trying to acquire lock %s\n",currentThread->getName(),name);

	IntStatus oldLevel=interrupt->SetLevel(IntOff);
	for(;1;)
	{

		if(condition==Free)
		{
			//printf("%s acquired lock name %s\n",currentThread->getName(),name);
			condition=Busy;
			holderThread=currentThread;
			interrupt->SetLevel(oldLevel);
			break;
		}
		else if(condition==Busy)
		{
			//printf("%s added in wait queue name %s\n",currentThread->getName(),name);
			waitQueue->Append((void*)currentThread);
			//printf("Here\n");
			currentThread->Sleep();
			interrupt->SetLevel(oldLevel);
		}
	}



}
void Lock::Release()
{
	////printf("release called\n");
	int temp=0;
	IntStatus oldLevel=interrupt->SetLevel(IntOff);
	if(isHeldByCurrentThread())
	{
		//printf("%s Releases lock name %s\n",currentThread->getName(),name);
		if(!waitQueue->IsEmpty())
		{
			Thread * tempThread;
			tempThread=(Thread*)waitQueue->Remove();
			//printf("Selecting Thread %s for ready to run\n",tempThread->getName());

			scheduler->ReadyToRun(tempThread);
			holderThread=tempThread;

		}
		else
		{
			condition=Free;
			holderThread=NULL;
		}
	}
	else
	{
		temp=1;
	}
	interrupt->SetLevel(oldLevel);
	if(temp==1)
	{
		//printf("-----------it is not\n");
	}

}
bool Lock::isHeldByCurrentThread()
{
	if(holderThread==currentThread)
	{
		return true;
	}
	return false;
}
Condition::Condition(char* debugName)
{
		name=debugName;
		waitQueue=new List();
}
Condition::~Condition()
{
	delete waitQueue;
}
void Condition::Wait(Lock* conditionLock)
{
	//ASSERT(FALSE);
	IntStatus oldLevel=interrupt->SetLevel(IntOff);

	if((conditionLock==NULL)||(!conditionLock->isHeldByCurrentThread()))
	{
		interrupt->SetLevel(oldLevel);
		return ;
	}
	 //printf("%s is going to sleep(in condition)\n",currentThread->getName());
	 waitQueue->Append((void*)currentThread);
	 conditionLock->Release();
	 currentThread->Sleep();
	 conditionLock->Acquire();
	 //printf("%s is going to wake(in condition)\n",currentThread->getName());




	interrupt->SetLevel(oldLevel);
	return ;

}
void Condition::Signal(Lock* conditionLock)
{
	IntStatus oldLevel=interrupt->SetLevel(IntOff);


	if(!waitQueue->IsEmpty())
	{
		Thread *tempThread;
		tempThread=(Thread*)waitQueue->Remove();
		scheduler->ReadyToRun(tempThread);


	}

	interrupt->SetLevel(oldLevel);
	return ;
}
void Condition::Broadcast(Lock* conditionLock)
{
	//printf("%s is Broadcasting\n",currentThread->getName());
	IntStatus oldLevel=interrupt->SetLevel(IntOff);


		for(;!waitQueue->IsEmpty();)
		{
			Signal(conditionLock);
		}


	interrupt->SetLevel(oldLevel);
	return ;

}
