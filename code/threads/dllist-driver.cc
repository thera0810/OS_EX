#include "dllist.h"
#include <time.h>
// #include <stdlib.h>
#include <string.h>
#include "thread.h"

extern Thread *currentThread;
extern int testnum;

//Generate random N elems and insert them to a dllist
void dllFunc1(DLList *list, int N){
    printf("\n*** thread %s is ready to Insert %d elems to the list ***\n",currentThread->getName(),N);
    for(int i=0;i<N;++i){
        list->SortedInsert(NULL,i);
    }
    list->Show();
    printf("\n");
}

//removes N items starting from the head of the list 
//and prints out the removed items to the console
void dllFunc2(DLList *list, int N){
    printf("\n*** thread %s is ready to remove the first %d elems from the list ***\n",currentThread->getName(),N);
    int oldN=N;
	while (N--) {
        int k;
        void *item=list->Remove(&k);
    }
    list->Show();
    printf("\n");
}
