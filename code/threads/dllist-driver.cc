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
    printf("\n*** thread %s ready to Insert %d elems to the list ***\n",currentThread->getName(),N);
    // printf("\nInsert %d elems in the list:\n",N);
    for(int i=0;i<N;++i){
        //int k=rand()%(10*N);
        list->SortedInsert(NULL,i);
        printf("*** thread %s insert %d ***\n",currentThread->getName(),i);
    }
    list->Show();
    printf("\n");
}
//removes N items starting from the head of the list 
//and prints out the removed items to the console
void dllFunc2(DLList *list, int N){
    printf("\n*** thread %s ready to remove the first %d elems from the list ***\n",currentThread->getName(),N);
    int oldN=N;
    while(!(list->IsEmpty())&&(N--)){
        int k;
        void *item=list->Remove(&k);

        if (testnum==20&&N==oldN-1)//when removed the first elem, yield
        {
            printf("\n\033[1;31;40m//======= thread %s yield =======\\\\\033[m\n",currentThread->getName());
            currentThread->Yield();
            printf("\n\\\\------- thread %s is running -------//\n",currentThread->getName());
        }

        printf("*** thread %s removed %d ***\n",currentThread->getName(),k);
    }
    list->Show();
    printf("\n");
}
