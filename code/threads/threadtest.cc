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
#include <stdlib.h>
#include <time.h>
#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include <stdio.h>
#include "synch.h"
#include "Table.h"
#include "BoundedBuffer.h"
#include "EventBarrier.h"
#include <ctime>

// testnum is set in main.cc
int testnum = 1;
int threadnum=2;
int N=10;
char threadname[10][20]={{0}};
DLList *ls=new DLList();

int tablesize = 3;
Table* tb = new Table(tablesize);
BoundedBuffer * bf=new BoundedBuffer(10);

//EventBarrier
EventBarrier* eb=new EventBarrier();

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

//---------------------------ThreadTest2 dllist---------------------------

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


//---------------------------ThreadTest3 dllist---------------------------

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

//---------------------------ThreadTest4 dllist---------------------------

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

//---------------------------ThreadTest 5 Table---------------------------

void TableThreadFunc0(int n) // allocate 5 item continueously
{
	int* object = new int();
	*object = n;
    for(int i=0;i<5;++i)
	   tb->Alloc(object);
}

void TableThreadFunc1(int n) // release from 0 to tablesize one by one
{
	int i;
	for (i = 0; i < tablesize; i++) {
		int j = i%3;
		tb->Release(j);
		currentThread->Yield();
	}
}

void TableThreadTest5()
{
	DEBUG('t', "Entering TableThreadTest5");
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		if (i == 1) t->Fork(TableThreadFunc1, i);
		else t->Fork(TableThreadFunc0, i);
	}
}

//---------------------------ThreadTest 6 Table---------------------------

void TableThreadTest6()
{
	DEBUG('t', "Entering TableThreadTest6");
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		if(i==0) t->Fork(TableThreadFunc0, i);
		else t->Fork(TableThreadFunc1, i);
	}
}


//---------------------------ThreadTest7 Bounded Buffer---------------------------

void Consumer(int n)
{
    //int a[3]={2,0,0};
    void *data=new char[10];
    for(int i=0;i<n;++i){
        int t=rand()%10;
        //int t=a[i];
        bf->Read(data,t);
        printf("%s:%d\n",currentThread->getName(),t);
        bf->ShowState();
    }
}
void Producer(int n)
{
    //int a[3]={3,3,5};
    void *data=new char[10];
    for(int i=0;i<n;++i){
        int t=rand()%10;
        //int t=a[i];
        bf->Write(data,t);
        printf("%s:%d\n",currentThread->getName(),t);
        bf->ShowState();
    }
}
void ThreadTest7()
{
    srand(time(NULL));
    DEBUG('t', "Entering ThreadTest7\n");
    bf->ShowState();
    for (int i = 0; i < threadnum; ++i)
    {
        if(i%2==0){
            sprintf(threadname[i], "Producer%d", i/2);
            Thread* t = new Thread(threadname[i]);
            t->Fork(Producer, 3);
        }
        else{
            sprintf(threadname[i], "Consumer%d", i/2);
            Thread* t = new Thread(threadname[i]);
            t->Fork(Consumer, 3);
        }
    }
}


//--------------------------- ThreadTest 8 EventBarrier---------------------------

void SimpleThreadFunc8_wait(int n)
{
    eb->Wait();
}
void SimpleThreadFunc8_signal(int n)
{
    eb->Signal();
}
void ThreadTest8()
{
    DEBUG('t', "Entering ThreadTest8");
    for (int i = 0; i < threadnum; ++i)
    {
        sprintf(threadname[i], "%d", i);
        Thread* t = new Thread(threadname[i]);
        if(i!=threadnum-1)
            t->Fork(SimpleThreadFunc8_wait,i);
        else
            t->Fork(SimpleThreadFunc8_signal,i);
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
		// ./nachos -d b -q 5 -T 3  
		TableThreadTest5();
		break;
	}

	case 6:// Table Threadtest
	{
		// ./nachos -d b -q 6 -T 2  
		TableThreadTest6();
		break;
	}

    case 7://test boundedbuffer
    {
        ThreadTest7();
        break;
    }

    case 8://test boundedbuffer
    {
        ThreadTest8();
        break;
    }

    default:
    {
        printf("No test specified.\n");
        break;
    }

    }
}

