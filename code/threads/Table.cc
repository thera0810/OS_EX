#include <stdlib.h>
#include "Table.h"
#include "synch.h"
#include <string.h>

extern Thread* currentThread;
extern int testnum;

// create a table to hold at most 'size' entries.
Table::Table(int size)
{
	ASSERT(size > 0);
	maxsize = size;
	count = 0;
	lock = new Lock("table_lock");//used for an exclusive lock. 
	condition = new Condition("table_full_con");//a semaphore to process blockage problem.
	entries = (int**)malloc(sizeof(int*) * size);//a list to save the addr of content in each slot.
	for (int i = 0; i < maxsize; i++) entries[i] = NULL;//initialize each element NULL.
}

Table::~Table()//deallocate Table
{
	delete entries;
	delete lock;
	delete condition;
	maxsize = 0;
}

// allocate a table slot for 'object'.
// return the table index for the slot or -1 on error.
int Table::Alloc(void* object)
{
	lock->Acquire();//acquired an exclusive lock.
	ASSERT(maxsize > 0);//make sure the Table is available.
	while (count == maxsize) {//Table is full.
		DEBUG('b',"Thread%s allocated failed, Table is full\n\n", currentThread->getName());
		condition->Wait(lock);//the threads will go to Sleep to wait for resource.
	}
	int i;
	for (i = 0; i < maxsize; ++i)
	{
		if (entries[i] == NULL) break;//look for the first emtpy entry in the Table.
	}
	entries[i] = (int*)object;//save the addr of object pointer.
	DEBUG('b',"++ Thread%s allocated an object: entries[%d]\n\n", currentThread->getName(), i);
	++count;//count total number of used slots in current Table.
	lock->Release();//released an exclusive lock
	return i;
}

// return the object from table index 'index' or NULL on error.
// (assert index is in range).  Leave the table entry allocated
// and the pointer in place.
void* Table::Get(int index)
{
	lock->Acquire();//acquired an exclusive lock.
	ASSERT(index >= 0 && index < maxsize);//make sure the index is valid.
	if (entries[index] != NULL)//the entry is not empty.
	{
		DEBUG('b',"Thread%s got entries[%d]: %c\n\n", currentThread->getName(), index, *(char*)entries[index]);
		lock->Release();//released an exclusive lock.
		return entries[index];//the content of the entry is returned.
	}
	else
	{
		DEBUG('b',"Thread%s got failed,entries[%d] is NULL\n\n", currentThread->getName(), index);
		lock->Release();//released an exclusive lock.
		return NULL;//If the entry is empty, NULL is returned to the caller. 
	}
}

// free a table slot
void Table::Release(int index)
{
	lock->Acquire();//acquired an exclusive lock. 
	ASSERT(index >= 0 && index < maxsize);//make sure the index is valid.

	if (entries[index]) {//one slot is released in the Table.
		--count;//update total number of used slots in current Table.
		entries[index] = NULL;//released the entry.
		DEBUG('b',"-- Thread%s released an object: entries[%d]\n\n", currentThread->getName(), index);
		condition->Broadcast(lock);// table is not full now.
		lock->Release();//released an exclusive lock.
	}
	else//no space has been released yet.
	{
		DEBUG('b',"Thread%s released an empty object: entries[%d]\n\n", currentThread->getName(), index);
		lock->Release();//released an exclusive lock.
	}
}
