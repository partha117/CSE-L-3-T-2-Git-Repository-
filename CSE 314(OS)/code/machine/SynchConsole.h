/*
 * SynchConsole.h
 *
 *  Created on: May 11, 2017
 *      Author: partha
 */
#ifndef MACHINE_SYNCHCONSOLE_H_
#define MACHINE_SYNCHCONSOLE_H_
#include "synch.h"
#include "console.h"
class SynchConsole
{
public:
	SynchConsole();
	~SynchConsole();
	int readConsole(char *buffer, int size);
	void writeConsole(char *buffer, int size);
private:
	Lock* consoleLock;
	Console* console;
};


#endif /* MACHINE_SYNCHCONSOLE_H_ */
