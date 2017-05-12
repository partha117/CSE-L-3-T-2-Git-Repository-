/*
 * SynchConsole.cc
 *
 *  Created on: May 11, 2017
 *      Author: partha
 */
# include "SynchConsole.h"
#include "console.h"
#include "machine.h"
extern Machine *machine;
extern Semaphore* readConsoleSemaphore;
extern Semaphore* writeConsoleSemaphore;
static void ReadConsoleAvail(int arg) { readConsoleSemaphore->V();  }
static void WriteConsoleDone(int arg) { writeConsoleSemaphore->V(); }
SynchConsole::SynchConsole()
{
	consoleLock=new Lock("ConsoleLock");
	console=new Console(NULL,NULL,ReadConsoleAvail,WriteConsoleDone,0);
	writeConsoleSemaphore=new Semaphore("WriteConsole",0);
	readConsoleSemaphore=new Semaphore("ReadConsole",0);

}
SynchConsole::~SynchConsole()
{
	delete consoleLock;
}
int SynchConsole::readConsole(char *buffer,int size)
{
	//printf("console buffer %d\n",buffer);
	consoleLock->Acquire();
	char ch;
	int temp;
	int i;
	for( i=0;i<size;i++)
	{
		readConsoleSemaphore->P();
		ch=console->GetChar();
		if(ch!='\n')
		{
			temp=(int)(buffer+i);
			machine->WriteMem(temp,1,ch);
		}
		else
		{

			temp=(int)(buffer+i);
			machine->WriteMem(temp,1,'\n');
			temp=(int)(buffer+i+1);
			machine->WriteMem(temp,1,NULL);
			consoleLock->Release();
			return i+1;
		}

	}
	consoleLock->Release();
	return i+1;

}
void SynchConsole::writeConsole(char *buffer,int size)
{
	//printf("before\n");
	consoleLock->Acquire();
	char ch;
	int temp;
	int tempch;
	//printf("before loop size %d\n",size);
	for(int i=0;i<size;i++)
	{
		temp=(int)(buffer+i);
		//printf("virtual addr %d",temp);
		//printf("before read mem\n");
		machine->ReadMem(temp,1,&tempch);
		ch=(char)tempch;
		//printf("putting value %c\n",ch);
		console->PutChar(ch);
		writeConsoleSemaphore->P();
	}
	consoleLock->Release();
}

