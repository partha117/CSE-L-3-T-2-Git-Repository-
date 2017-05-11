/*
 * ProcessTable.cc
 *
 *  Created on: May 9, 2017
 *      Author: partha
 */
#include "ProcessTable.h"


ProcessTable::ProcessTable(int size)
{
	map=new BitMap(size);
	processTableLock=new Lock("ProcessTable");
	entrySet=new Entry[size];
	numProcess=0;
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
	}
	processTableLock->Release();
	return temp;
}
void* ProcessTable::Get(int index)
{
	processTableLock->Acquire();
	if(map->Test(index))
	{
		void *temp=entrySet->space;
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



