#include "dllist.h"
#include <time.h>
#include <stdlib.h>

//Generate random N elems and insert them to a dllist
void dllFunc1(DLList *list, int N){
    srand((unsigned)time(NULL)); 
    for(int i=0;i<N;++i){
        int k=rand()%(10*N);
        printf("%d\n",k);
        list->SortedInsert(NULL,k);
    }
}
//removes N items starting from the head of the list 
//and prints out the removed items to the console
void dllFunc2(DLList *list, int N){
    printf("Remove the first %d elems in the list:\n",N);
    while(!(list->IsEmpty())&&(N--)){
        int k;
        void *item=list->Remove(&k);
        printf("%d ",k);
    }
    printf("\n");
}

