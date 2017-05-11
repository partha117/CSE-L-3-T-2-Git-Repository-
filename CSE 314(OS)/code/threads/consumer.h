#ifndef CONSUMER_H
#define CONSUMER_H

#include "PCbuffer.h"

class consumer
{
private:
	char * threadName;
	PCbuffer *currentBuffer;
public:
	consumer(char* name,PCbuffer *currentBuffer);
	void consume();

};
#endif
