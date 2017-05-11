#include<stdio.h>
#include "producer.h"


producer::producer(char* name,PCbuffer *buffer,int n)
{
	threadName=name;
	currentBuffer=buffer;
	start=n;

}
void producer:: produce()
{

	printf("+++++++++++++++++++++++++++++++++++++Producer produces %d\n",start);
	currentBuffer->store(start);
	start+=10;
}
