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

// testnum is set in main.cc
int testnum = 1;
DLList *ls=new DLList();

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

void listAdd(int which){
    int i;
    printf("Thread num:%d, list add.\n",which);
    for(i=101;i<=105;++i){
        ls->SortedInsert(NULL,i);
        //currentThread->Yield();
    }
    /*for(i=0;i<5;++i){
        ls->Prepend(NULL);
    }*/
    ls->Show();
}
void listRemove(int which){
    int i,k;
    printf("Thread num:%d, list remove\n",which);
    for(i=0;i<15;++i){
        ls->Remove(&k);
        //currentThread->Yield();
    }
    //dllFunc2(ls,5);
    /*for(i=0;i<5;++i){
        ls->Append(NULL);
    }*/
    
    for(i=102;i<104;++i){
        ls->SortedRemove(i);
    }
    ls->Show();
}

void
ThreadTest2(){
    Thread *t = new Thread("test dllist");
    //create a list    
    dllFunc1(ls,10);
    ls->Show();
    //t->Fork(listAdd,1);
    t->Fork(listRemove,2);
    //listRemove(2);
    listAdd(1);
}
//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    case 2:{
               ThreadTest2();
               break;
           }
    default:
	printf("No test specified.\n");
	break;
    }
}

