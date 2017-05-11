#ifndef PCBUFFER_H
#define PCBUFFER_H
#define bufferCapacity 10
class PCbuffer
{
private:
	int storage[bufferCapacity];
	int pointer;
public:
	PCbuffer();
	void store(int storedValue);
	bool full();
	bool empty();
	int get();
	void printBuffer();


};
#endif
