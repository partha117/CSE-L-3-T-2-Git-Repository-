#ifndef PRODUCER_H
#define PRODUCER_H
#include "PCbuffer.h"
class producer
{
private:
	char * threadName;
	PCbuffer *currentBuffer;
	int start;
public:
	producer(char* name,PCbuffer *buffer,int n);
	void produce();

};
#endif
