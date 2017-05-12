// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "addrspace.h"
#include "synch.h"
#include "ProcessTable.h"
#include "MemoryManager.h"
#include "SynchConsole.h"
#define E_MAX_SIZE 100
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
extern Lock *exceptionLock;
extern ProcessTable * pTable;
extern MemoryManager* memManager;
extern SynchConsole* synchConsole;
void myFunction(int demo);
void myExit(int status);
void Write(char *buffer, int size, OpenFileId id);
int Read(char *buffer, int size, OpenFileId id);
void IncPC ()
{
    int pc = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, pc);
    pc = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, pc);
    pc += 4;
    machine->WriteRegister(NextPCReg, pc);
}
void ExceptionHandler(ExceptionType which)
{
	//printf("lock from here\n");
	exceptionLock->Acquire();
    int type = machine->ReadRegister(2);
    int arg1 = machine->ReadRegister(4);
    int arg2 = machine->ReadRegister(5);
    int arg3 = machine->ReadRegister(6);
    int arg4 = machine->ReadRegister(7);

    if ((which == SyscallException) && (type == SC_Halt)) {
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    }
    else if ((which == SyscallException) && (type == SC_Exec))
    {
    	char fileName[E_MAX_SIZE];
    	int temp;
    	int i=0;
    	machine->ReadMem(arg1, 1, &temp);
        while ((*(char*)&temp) != '\0')
       {
    		fileName[i] = (char)temp;
    		i++;
    		arg1++;
    		//printf("it is %c\n",temp);
    		machine->ReadMem(arg1, 1, &temp);
        }
        fileName[i]=0;
        printf("%s it is\n",fileName);
        SpaceId id=Exec(fileName);
        int pid=(int)id;
        Thread *th=(Thread*)pTable->Get(id);
    	exceptionLock->Release();
    	//printf("returned thread name %s %d\n",th->getName(),id);
        th->Fork(myFunction,pid);
        machine->WriteRegister(2,pid);
        IncPC();
    }
    else if ((which == SyscallException) && (type == SC_Exit))
    {
    	exceptionLock->Release();
    	myExit(arg1);
    }
    else if ((which == SyscallException) && (type == SC_Read))
    {
    		exceptionLock->Release();
        	int byte=Read((char*)arg1,arg2,arg3);
        	machine->WriteRegister(2,byte);
        	IncPC();

    }
    else if ((which == SyscallException) && (type == SC_Write))
    {
    		exceptionLock->Release();
           	Write((char*)arg1,arg2,arg3);
           	IncPC();


    }

    else {

	printf("Unexpected user mode exception %d %d exception:", which, type);
	if(which==AddressErrorException)
	    	{
	    		printf("Address error exception\n");
	    	}
	    	else if(which==PageFaultException)
	    	{
	    		printf("Page fault exception\n");
	    	}
	    	else if(which==BusErrorException)
	    	{
	    		printf("Bus error exception\n");
	    	}
	    	else if(which==ReadOnlyException)
	    	{
	    		printf("Read only exception\n");
	    	}
	ASSERT(FALSE);
    }

}
SpaceId Exec(char *name)
{
	OpenFile *executableFile=fileSystem->Open(name);
	ASSERT(executableFile!=NULL);
	AddrSpace *fileSpace=new AddrSpace(executableFile);
	Thread *fileThread=new Thread("USER_PROGRAM");
	fileThread->spaceId=pTable->Alloc(fileThread);
	fileThread->space=fileSpace;
	return fileThread->spaceId;

}
void Write(char *buffer, int size, OpenFileId id)
{
	synchConsole->writeConsole(buffer,size);
}
int Read(char *buffer, int size, OpenFileId id)
{
	int temp=synchConsole->readConsole(buffer,size);
	return temp;
}
void myExit(int status)
{

	//printf("lock from there\n");
	exceptionLock->Acquire();

	int ps=machine->pageTableSize;
	for(int i=0;i<ps;i++)
	{
		memManager->freePage(machine->pageTable[i].physicalPage);

	}
	pTable->Release(currentThread->spaceId);
	if(status==0)
	{
		printf("Process executed Successfully!!\n");
	}
	printf("Exit code %d\n",status);
	if(pTable->getNumProcess()==0)
	{
		exceptionLock->Release();
		interrupt->Halt();
	}
	exceptionLock->Release();

	currentThread->Finish();

}

void myFunction(int id)
{
	(currentThread->space)->InitRegisters();		// set the initial register values
	(currentThread->space)->RestoreState();
	machine->Run();
}
