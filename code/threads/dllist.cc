#include "dllist.h"
#include "copyright.h"
#include "thread.h"
#include <string.h>

extern Thread *currentThread;
extern int testnum;

DLLElement::DLLElement( void *itemPtr, int sortKey){
    prev=next=NULL;
    key=sortKey;
    item=itemPtr;
    // std::cout<<"create first elem\n";
}

DLList::DLList(){
    // printf("create list!!!\n");
    // first=NULL;
    // last=NULL;
}

DLList::~DLList(){
    while(!IsEmpty()){
        int k;
        Remove(&k);
    }
}

void DLList::Prepend(void *item){
    if(IsEmpty()){
        first= new DLLElement(item,10);
        last=first;
    }
    else{
        DLLElement *p=new DLLElement(item,first->key-1);
        p->prev=NULL;
        p->next=first;
        first->prev=p;
        first=p;
    }
}

void DLList::Append(void *item){
    if(IsEmpty()){
        first=new DLLElement(item,10);
        last=first;
    }
    else{
        DLLElement *p=new DLLElement(item,last->key+1);
        p->next=NULL;
        p->prev=last;
        last->next=p;
        last=p;
    }
}

void *DLList::Remove(int *keyPtr){
    if(IsEmpty()){
        printf("Remove failure: List is empty!\n");
        keyPtr=NULL;
        return NULL;
    }
    if(first==last){
        //only 1 element
        *keyPtr=first->key;
        void * ritem=first->item;
        first=NULL;
        last=NULL;
        delete first;
        return ritem;
    }
    else{
        DLLElement *p=first;
        first=first->next;
        first->prev=NULL;
        *keyPtr=p->key;
        void * ritem=p->item;
        delete p;
        return ritem;
    }
}

bool DLList::IsEmpty(){
    if(first==NULL){
        return true;
    }
    return false;
}

void DLList::SortedInsert(void *item, int sortKey){
    if(IsEmpty()){
        DLLElement *p= new DLLElement(item,sortKey);
        first=p;
        last=p;
        if (testnum==70 && strcmp(currentThread->getName(),"main")==0)
        {
            printf("\n======= thread %s yield =======\n",currentThread->getName());
            currentThread->Yield();
            printf("\n------- thread %s is running -------\n",currentThread->getName());
        }
    }
    else{// not empty
        DLLElement *pi=first;
        while(pi!=NULL&&pi->key<sortKey){
            pi=pi->next;
        }
        if(pi==NULL){//put in the last
            DLLElement *p=new DLLElement(item,sortKey);
            p->next=NULL;
            p->prev=last;
            last->next=p;
            last=p;
        }
        else{
            if(pi->prev==NULL){//put in the first
                DLLElement *p=new DLLElement(item,sortKey);
                p->prev=NULL;
                p->next=first;
                first->prev=p;
                first=p;
            }
            else{//put in the middle
                DLLElement *p=new DLLElement(item,sortKey);
                p->prev=pi->prev;
                p->next=pi;
                pi->prev->next=p;
                pi->prev=p;
            }
        }
    }
}

void *DLList::SortedRemove(int sortKey){

    if(IsEmpty()){
        printf("Remove failure: List is empty!\n");
        return NULL;
    }

    DLLElement *pi=first;
    while(pi&&pi->key!=sortKey){
        pi=pi->next;
    }
    if(pi==NULL){// do not find
        return NULL;
    }
    //match
    if(first==last){//only 1 elem
        if(pi==first){
            first=last=NULL;
        }
    }
    else{
        if(pi==first){//delete first
            first=pi->next;
            first->prev=NULL;
            delete pi;
        }
        else if(pi==last){//delete last
            last=pi->prev;
            last->next=NULL;
            delete pi;
        }
        else{//pi in the middle
            pi->next->prev=pi->prev;
            pi->prev->next=pi->next;
            delete pi;
        }
    }
}

void DLList::Show(){
    printf("\n***show list***\n");
    if(IsEmpty()){
        printf("Show failure: List is empty!\n");
        return;
    }
    DLLElement * p=first;
    while(p!=NULL){
        printf("%d ",p->key);
        p=p->next;
    }
    printf("\n\n");
}

