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
#include "../dllist/dllist.h"
#include <stdio.h>

// testnum is set in main.cc
int testnum = 1;
int threadnum=2;
int N=10;
DLList *ls=new DLList();


//---------------------------ThreadTest1---------------------------


//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
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
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//---------------------------ThreadTest1---------------------------


//---------------------------ThreadTest7---------------------------

void SimpleThread7(int which)
{
    printf("\n------- thread %s is running -------\n",currentThread->getName());
    dllFunc1(ls,N);
    dllFunc2(ls,N);
}

void ThreadTest7()
{
    DEBUG('t', "Entering ThreadTest2");

    for (int i=1;i<threadnum;i++)
    {
        char threadname[10]={0};
        sprintf(threadname,"%d",i);

        Thread *t = new Thread(threadname);
        
        t->Fork(SimpleThread7, i);
    }

    SimpleThread7(0);
}

//---------------------------ThreadTest7---------------------------


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------
void
ThreadTest()
{
    switch (testnum) {
    case 1:
    {
        ThreadTest1();
        break;
    }
	
    case 70:
    {
        printf("====================This is the main test! testnum=%d====================\n",testnum);
        ThreadTest7();
        break;
    }


    default:
    {
        printf("No test specified.\n");
        break;
    }
	
    }
}

