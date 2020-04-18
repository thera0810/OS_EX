#include<stdlib.h>
#include "Table.h"
#include "synch.h"
#include "copyright.h"
#include "system.h"
#include "thread.h"
#include <string.h>

extern Thread* currentThread;
extern int testnum;

// create a table to hold at most 'size' entries.
Table::Table(int size)
{
	ASSERT(size > 0);
	maxsize = size;
	count = 0;
	lock = new Lock("table_lock");
	condition = new Condition("table_full_con");
	entries = (int**)malloc(sizeof(int*) * size);
	for (int i = 0; i < maxsize; i++) entries[i] = NULL;
}

Table::~Table()
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
	ASSERT(maxsize > 0);
	lock->Acquire();
	while (count == maxsize) {
		printf("Thread%s allocated failed, Table is full\n\n", currentThread->getName());
		condition->Wait(lock);
	}
	int i;
	for (i = 0; i < maxsize; ++i)
	{
		if (entries[i] == NULL) break;
	}
	entries[i] = (int*)object;
	printf("Thread%s allocated an object: entries[%d]\n\n", currentThread->getName(), i);
	//DLLElement* element = new DLLElement(object, count);
	//entries->SortedInsert(element, count);
	++count;
	lock->Release();
	return i;
}

// return the object from table index 'index' or NULL on error.
// (assert index is in range).  Leave the table entry allocated
// and the pointer in place.
void* Table::Get(int index)
{
	ASSERT(index >= 0 && index < maxsize);
	lock->Acquire();
	if (entries[index] != NULL)
	{
		printf("Thread%s got entries[%d]: %c\n\n", currentThread->getName(), index, *(char*)entries[index]);
		lock->Release();
		return entries[index];
	}
	else
	{
		printf("Thread%s got failed,entries[%d] is NULL\n\n", currentThread->getName(), index);
		lock->Release();
		return NULL;
	}
}

// free a table slot
void Table::Release(int index)
{
	ASSERT(index >= 0 && index < maxsize);
	lock->Acquire();

	if (entries[index]) {
		--count;
		entries[index] = NULL;
		printf("Thread%s released an object: entries[%d]\n\n", currentThread->getName(), index);
		condition->Signal(lock);                // table is not full nows
		lock->Release();
	}
	else
	{
		printf("Thread%s released an empty object: entries[%d]\n\n", currentThread->getName(), index);
		lock->Release();
	}
}
