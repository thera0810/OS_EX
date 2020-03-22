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
char threadname[10][5]={{0}};
DLList *ls=new DLList();

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

//---------------------------ThreadTest1---------------------------

//---------------------------ThreadTest20---------------------------
void SimpleThreadFunc2(int n)
{
    printf("\n/------- thread %s is running -------\\\n",currentThread->getName());
    dllFunc2(ls,n);
    printf("\n\\------- thread %s is end     -------/\n",currentThread->getName());
}

void ThreadTest20()
{
    DEBUG('t', "Entering ThreadTest2");
    dllFunc1(ls,N);
    for (int i=0;i<threadnum;++i)
    {
        sprintf(threadname[i],"%d",i);
        Thread *t = new Thread(threadname[i]);
        t->Fork(SimpleThreadFunc2, 3);
    }
}

//---------------------------ThreadTest20---------------------------

//---------------------------ThreadTest21---------------------------
void ThreadTest21()
{
    DEBUG('t', "Entering ThreadTest2");
    dllFunc1(ls,N);
    for (int i=0;i<threadnum;++i)
    {
        sprintf(threadname[i],"%d",i);
        Thread *t = new Thread(threadname[i]);
        t->Fork(SimpleThreadFunc2, 1);
    }
}
//---------------------------ThreadTest21---------------------------





//---------------------------ThreadTest7---------------------------

void SimpleThread70(int which)
{
    printf("\n/------- thread %s is running -------\\\n",currentThread->getName());
    ls->SortedInsert(NULL,7);
    printf("\n\\------- thread %s is end     -------/\n",currentThread->getName());
    ls->Show();
}

void SimpleThread71(int which)
{
    printf("\n/------- thread %s is running -------\\\n",currentThread->getName());
    ls->SortedRemove(7);
    printf("\n\\------- thread %s is end     -------/\n",currentThread->getName());
}

void ThreadTest70()
{
    DEBUG('t', "Entering ThreadTest70");

    dllFunc1(ls, N);

    for (int i=0;i<threadnum;++i)
    {
        sprintf(threadname[i],"%d",i);
        Thread *t = new Thread(threadname[i]);
        if (i==0)
        {
        	t->Fork(SimpleThread70, 1);
        }
        else if (i==1)
        {
        	t->Fork(SimpleThread71, 1);
        }
    }
}

//---------------------------ThreadTest7---------------------------

//---------------------------ThreadTest30---------------------------
//***********************************************************************
//***********************************************************************
void SimpleThreadFunc31(int n)
{
	printf("\n/------- thread %s is running -------\\\n", currentThread->getName());
	ls->Prepend(NULL);
	ls->Show();
	printf("\n\033[1;31;40m//======= thread %s yield =======\\\\\033[m\n", currentThread->getName());
	currentThread->Yield();
	printf("\n\\\\------- thread %s is running -------//\n", currentThread->getName());
	dllFunc2(ls, 1);
	printf("\n\\------- thread %s is end     -------/\n", currentThread->getName());
}

void SimpleThreadFunc33(int n)
{
	printf("\n/------- thread %s is running -------\\\n", currentThread->getName());
	dllFunc2(ls, 1);
	printf("\n\\------- thread %s is end     -------/\n", currentThread->getName());
}

void ThreadTest30()
{
	DEBUG('t', "Entering ThreadTest2");
	//dllFunc1(ls, N);
	dllFunc1(ls, 0);
	for (int i = 1; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc33, 1);
	}
	SimpleThreadFunc31(0);
}

//---------------------------ThreadTest30---------------------------


//---------------------------ThreadTest40---------------------------

void SimpleThreadFunc44(int n)
{
	printf("\n/------- thread %s is running -------\\\n", currentThread->getName());
	int i = 0;
	printf("\n*** thread %s ready to Insert %d to the list ***\n", currentThread->getName(), i);
	ls->SortedInsert(NULL, i);
	printf("*** thread %s insert %d ***\n", currentThread->getName(), i);
	ls->Show();
	printf("\n\\------- thread %s is end     -------/\n", currentThread->getName());
}

void SimpleThreadFunc45(int n)
{
	printf("\n/------- thread %s is running -------\\\n", currentThread->getName());
	int i = 0;
	printf("\n*** thread %s ready to Removed %d to the list ***\n", currentThread->getName(), i);
	ls->SortedRemove(i);
	ls->Show();
	printf("\n\\------- thread %s is end     -------/\n", currentThread->getName());
}

void ThreadTest40()
{
	DEBUG('t', "Entering ThreadTest2");
	dllFunc1(ls, 1);
	for (int i = 1; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc45, 1);
	}
	SimpleThreadFunc44(0);
}

//---------------------------ThreadTest40---------------------------

//---------------------------ThreadTest41---------------------------

void ThreadTest41()
{
	DEBUG('t', "Entering ThreadTest2");
	dllFunc1(ls, 2);
	//dllFunc1(ls, 3);//???????????Żᱨ?????򲻱???
    for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc45, 1);
	}
}

//---------------------------ThreadTest41---------------------------

//---------------------------ThreadTest8---------------------------
//***********************************************************************
//***********************************************************************

void SimpleThreadFunc8(int n)
{
	printf("\n/------- thread %s is running -------\\\n", currentThread->getName());
	printf("\n*** thread %s ready to Append the last item to the list ***\n", currentThread->getName());
	ls->Append(NULL);
	ls->Show();
	printf("\n\\------- thread %s is end     -------/\n", currentThread->getName());
}

void ThreadTest80()
{
	DEBUG('t', "Entering ThreadTest2");
	dllFunc1(ls, 3);
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc8, 1);
	}
}
void ThreadTest81()
{
	DEBUG('t', "Entering ThreadTest2");
	dllFunc1(ls, 3);
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc8, i);
	}
}
//---------------------------ThreadTest8---------------------------


//---------------------------ThreadTest60---------------------------
void SimpleThreadFunc62(int n)
{
	printf("\n/------- thread %s is running -------\\\n", currentThread->getName());
	printf("\n*** thread %s ready to Insert %d to the list ***\n", currentThread->getName(), n);
	ls->SortedInsert(NULL, n);
	ls->Show();
	printf("\n\\------- thread %s is end     -------/\n", currentThread->getName());
}

void ThreadTest60()
{
	DEBUG('t', "Entering ThreadTest2");
	ls->SortedInsert(NULL, 5);
	ls->SortedInsert(NULL, 15);
	ls->Show();
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc62, i+10);
	}
}
void ThreadTest62()
{
	DEBUG('t', "Entering ThreadTest2");
	ls->SortedInsert(NULL, 4);
	ls->SortedInsert(NULL, 15);
	ls->Show();
	for (int i = 0; i < threadnum; ++i)
	{
		sprintf(threadname[i], "%d", i);
		Thread* t = new Thread(threadname[i]);
		t->Fork(SimpleThreadFunc62, i + 10);
	}
}
//---------------------------ThreadTest60---------------------------
//***********************************************************************
//***********************************************************************

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
	
    case 20://dml: romove in an unexpected order
    {
        //./nachos -q 20 -T 3
        ThreadTest20();
        break;
    }
    case 21://dml remove the same element
    {
        //./nachos -q 21 -T 3
        ThreadTest21();
        break;
    }

	case 30://operate error??remove failure??
		{
		// ./nachos -q 30 -T 2	
		ThreadTest30();
			break;
		}

	case 40://core dumped
	{
		// ./nachos -q 40 -T 2	
		ThreadTest40();
		break;
	}

	case 41://unknown error
	{
		// ./nachos -q 41 -T 2	
		ThreadTest41();
		break;
	}

	case 80://repeated(pointer error)
	{
		// ./nachos -q 80 -T 2	
		ThreadTest80();
		break;
	}

	case 81://overwrite(pointer error)
	{
		// ./nachos -q 81 -T 2	
		ThreadTest81();
		break;
	}
	case 60://disorder
	{
		// ./nachos -q 60 -T 2	
		ThreadTest60();
		break;
	}
	case 62://overwrite
	{
		// ./nachos -q 62 -T 2	
		ThreadTest62();
		break;
	}
	case 70:
    {
    	// .nachos -q 70 -T 2 -N 10
        ThreadTest70();
        break;
    }

    default:
    {
        printf("No test specified.\n");
        break;
    }
	
    }
}

