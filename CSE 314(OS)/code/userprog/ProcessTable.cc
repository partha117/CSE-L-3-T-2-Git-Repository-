/*
 * ProcessTable.cc
 *
 *  Created on: May 9, 2017
 *      Author: partha
 */
#include "ProcessTable.h"


ProcessTable::ProcessTable(int tableSize)
{
	map=new BitMap(tableSize);
	processTableLock=new Lock("ProcessTable");
	entrySet=new Entry[tableSize];
	numProcess=0;
	size=tableSize;
}
ProcessTable::~ProcessTable()
{
	delete entrySet;
}
int ProcessTable::Alloc(void *object)
{
	processTableLock->Acquire();
	int temp=map->Find();
	if(temp!=-1)
	{
		entrySet[temp].space=object;
		entrySet[temp].processId=temp;
		numProcess++;
		//printf("pTable alloc %d name %s\n",temp,((Thread*)(object))->getName());
	}
	processTableLock->Release();
	return temp;
}
void* ProcessTable::Get(int index)
{
	processTableLock->Acquire();
	if((index>=0)&&(index<size)&&(map->Test(index)))
	{
		void *temp=entrySet[index].space;
		processTableLock->Release();
		return temp;
	}
	processTableLock->Release();
	return 0;
}
void  ProcessTable::Release(int index)
{
	processTableLock->Acquire();
	if(map->Test(index))
	{
		map->Clear(index);
		numProcess--;
	}
	processTableLock->Release();

}
int ProcessTable::getNumProcess()
{
	return numProcess;
}



