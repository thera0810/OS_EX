// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include <stdio.h>
#include "synch.h"
#include "Table.h"
// testnum is set in main.cc
int testnum = 1;
int threadnum=2;
int N=10;
char threadname[10][5]={{0}};
DLList *ls=new DLList();

int tablesize = 3;
Table* tb = new Table(tablesize);

//---------------------------ThreadTest1---------------------------

//----------------------------------------------------------------------
// SimpleThread
//  Loop 5 times, yielding the CPU to another ready thread 
//  each iteration.
//
//  "which" is simply a number identifying the thread, for debugging
//  purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
        printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
   }
}

//----------------------------------------------------------------------
// ThreadTest1
//  Set up a ping-pong between two threads, by forking a thread 
//  to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);

}

//---------------------------ThreadTest2---------------------------

void SimpleThread20(int which)
{
    ls->SortedInsert(NULL,7);
    ls->Show();
}

void SimpleThread21(int which)
{
    ls->SortedRemove(7);
    ls->Show();
}

void ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");

    dllFunc1(ls, N);
    for (int i=0;i<threadnum;++i)
    {
        sprintf(threadname[i],"%d",i);
        Thread *t = new Thread(threadname[i]);
        if (i==0)
        {
            t->Fork(SimpleThread20, 1);
        }
        else if (i==1)
        {
            t->Fork(SimpleThread21, 1);
        }
    }
}


//---------------------------ThreadTest3---------------------------

void SimpleThreadFunc3(int n)
{
	ls->Append(NULL);
	ls->Show();
}
void ThreadTest3()
{
	DEBUG('t', "Entering ThreadTest3");
	dllFunc1(ls, 3);
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc3, i);
	}
}

//---------------------------ThreadTest4---------------------------

void SimpleThreadFunc4(int n)
{
	ls->SortedInsert(NULL, n);
	ls->Show();
}

void ThreadTest4()
{
	DEBUG('t', "Entering ThreadTest4");
	ls->SortedInsert(NULL, 5);
	ls->SortedInsert(NULL, 15);
	ls->Show();
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc4, i+10);
	}
}

//---------------------------Table ThreadTest---------------------------

void TableThreadFunc0(int n)
{
	int* object = new int();
	*object = n;
	tb->Alloc(object);
	tb->Alloc(object);
	tb->Alloc(object);
	tb->Alloc(object);
	//tb->Alloc(object);
}

void TableThreadFunc1(int n)
{
	int i;
	for (i = 0; i < tablesize; i++) {
		int j = i%3;
		tb->Release(j);
		currentThread->Yield();
	}
}

void TableThreadTest()
{
	DEBUG('t', "Entering TableThreadTest");


	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		if (i == 1) t->Fork(TableThreadFunc1, i);
		else t->Fork(TableThreadFunc0, i);
	}
}

//---------------------------Table ThreadTest0---------------------------

void TableThreadFunc3(int n)
{
	int* object = new int();
	*object = n;
	tb->Alloc(object);
	tb->Alloc(object);
	tb->Alloc(object);
	tb->Alloc(object);
	tb->Alloc(object);
}

void TableThreadFunc4(int n)
{
	int i;
	for (i = 0; i < tablesize; i++) {
		int j = i % 3;
		tb->Release(j);
		currentThread->Yield();
	}
}

void TableThreadTest0()
{
	DEBUG('t', "Entering TableThreadTest0");
	tb->Get(0);
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		if(i==0) t->Fork(TableThreadFunc3, i);
		else t->Fork(TableThreadFunc4, i);
	}
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------
void
ThreadTest()
{
    printf("|%*s|\n",75," ");
    printf("|%*s\033[1;32;40mtestnum    = %-2d\033[m%*s|\n",30," ",testnum,30," ");
    printf("|%*s\033[1;32;40mthreadnum  = %-2d\033[m%*s|\n",30," ",threadnum,30," ");
    printf("|%*s\033[1;32;40melementnum = %-2d\033[m%*s|\n",30," ",N,30," ");
    printf("+---------------------------------------------------------------------------+\n");

    switch (testnum) {
    case 1:
    {
        ThreadTest1();
        break;
    }

    case 2://xht: link break 70
    {
        ThreadTest2();
        break;
    }

	case 3://overwrite(pointer error) lxh 81
	{
		// ./nachos -q 81 -T 2	
		ThreadTest3();
		break;
	}

    case 4:// insert disorder lxh 60
    {
        // ./nachos -q 60 -T 2  
        ThreadTest4();
        break;
    }

	case 5:// Table Threadtest
	{
		// ./nachos -q 5 -T 3  
		TableThreadTest();
		break;
	}

	case 6:// Table Threadtest
	{
		// ./nachos -q 6 -T 2  
		TableThreadTest0();
		break;
	}

    default:
    {
        printf("No test specified.\n");
        break;
    }

    }
}

