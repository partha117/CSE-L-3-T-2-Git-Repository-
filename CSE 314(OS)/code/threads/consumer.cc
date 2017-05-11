#include "consumer.h"
#include<stdio.h>
consumer::consumer(char* name,PCbuffer *buffer)
{
	threadName=name;
	currentBuffer=buffer;

}
void consumer:: consume()
{
	int n;
	n=currentBuffer->get();
	printf("++++++++++++++++++++++++++++++++++++++Consumer receives %d\n",n);
}
