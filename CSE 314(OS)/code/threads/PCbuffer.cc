#include "PCbuffer.h"
#include<stdio.h>

PCbuffer::PCbuffer()
{
	for(int i=0;i<bufferCapacity;i++)
	{
		storage[i]=-999;
	}
	pointer=0;

}
int PCbuffer:: get()
{
	//printf("now pointer Consumer %d\n",pointer);
	if(pointer>0)
	{
		pointer--;
		int temp=storage[pointer];
		return temp;
	}
	else
	{
		return -999;
	}
}
void PCbuffer::store(int storedValue)
{
	//printf("now pointer Producer %d\n",pointer);
	if(pointer<bufferCapacity)
	{
		storage[pointer]=storedValue;
		pointer++;
	}
	return ;
}
bool PCbuffer::full()
{
	if(pointer<bufferCapacity)
	{
		return false;
	}
	return true;
}
bool PCbuffer::empty()
{
	if(pointer>0)
	{
		return false;
	}
	return true;
}
void PCbuffer::printBuffer()
{
	int i;
	printf("Buffer Content: ");
	for(i=0;i<pointer;i++)
	{
		printf("%d   ,",storage[i]);
	}
	printf("\n");
}

