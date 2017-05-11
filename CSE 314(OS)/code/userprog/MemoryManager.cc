#include "MemoryManager.h"
#include "synch.h"
MemoryManager::MemoryManager(int totalPages)
{
	map=new BitMap(totalPages);
	lock=new Lock("MemoryManager");
	numPages=totalPages;
}
int MemoryManager::allocPage()
{
	int temp;
	lock->Acquire();
	temp=map->Find();
	//printf("it is map value %d",temp);
	lock->Release();
	return temp;
}
void MemoryManager::freePage(int phyPageNum)
{
	lock->Acquire();
	map->Clear(phyPageNum);
	lock->Release();

}
MemoryManager::~MemoryManager()
{
	delete map;
	delete lock;
}


