/*
 * ProcessTable.h
 *
 *  Created on: May 9, 2017
 *      Author: partha
 */

#ifndef USERPROG_PROCESSTABLE_H_
#define USERPROG_PROCESSTABLE_H_
#include "bitmap.h"
#include "synch.h"

class Entry
{
public:
	int processId;
	void * space;
	Entry()
	{

	}
};
class ProcessTable
{

public:
	ProcessTable(int size);
	int Alloc(void *object);
	void *Get(int index) ;
	void Release(int index) ;
	~ProcessTable();
	int getNumProcess();
private:
	Lock* processTableLock;
	BitMap* map;
	Entry *entrySet;
	int numProcess;
	int size;

};


#endif /* USERPROG_PROCESSTABLE_H_ */
