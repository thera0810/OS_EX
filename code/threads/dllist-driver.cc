#include "dllist.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"

extern Thread *currentThread;
extern int testnum;


//Generate random N elems and insert them to a dllist
void dllFunc1(DLList *list, int N){
    //srand((unsigned)time(NULL)); 
    printf("\nInsert %d elems in the list:\n",N);
    for(int i=0;i<N;++i){
        //int k=rand()%(10*N);
        printf("%d ",i);
        list->SortedInsert(NULL,i);
    }
    printf("\n");
}
//removes N items starting from the head of the list 
//and prints out the removed items to the console
void dllFunc2(DLList *list, int N){
    printf("\nRemove the first %d elems in the list:\n",N);

    while(!(list->IsEmpty())&&(N--)){
        int k;

        void *item=list->Remove(&k);
        printf("%d ",k);
    }
    printf("\n");
}
