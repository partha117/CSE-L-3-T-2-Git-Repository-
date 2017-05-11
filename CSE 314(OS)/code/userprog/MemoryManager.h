#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include "bitmap.h"
# include "synch.h"
class MemoryManager
{
	BitMap *map;
	Lock *lock;
	int numPages;
public:
	MemoryManager(int numPages);
	~MemoryManager();
	int allocPage();
	void freePage(int phyPageNum);


};
#endif
