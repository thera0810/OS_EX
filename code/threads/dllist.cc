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
}

DLList::DLList(){
    list = new List();
    lock = new Lock("list lock"); 
    listEmpty = new Condition("list empty cond");
}

DLList::~DLList(){
    delete list; 
    delete lock;
    delete listEmpty;
    while(!IsEmpty()){
        int k;
        Remove(&k);
    }
}

void DLList::Prepend(void *item){
    lock->Acquire();
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
    printf("\n*** thread %s prepend the first item ***\n", currentThread->getName());
    listEmpty->Broadcast(lock);
    lock->Release();
}

void DLList::Append(void *item){
    lock->Acquire();
    if(IsEmpty()){
        first=new DLLElement(item,10);
        last=first;
	}
    else{
        DLLElement *p=new DLLElement(item,last->key+1);
        p->next=NULL;
        p->prev=last; 
        last->next=p;
		
        if (testnum == 3 && strcmp(currentThread->getName(), "0") == 0)
			currentThread->Yield();
		
        last=p;
	}
    printf("\n*** thread %s Appended the last item ***\n", currentThread->getName());
    listEmpty->Broadcast(lock);
    lock->Release();
}

void *DLList::Remove(int *keyPtr){
    lock->Acquire();
    void * ritem=NULL;
    while(IsEmpty())
        listEmpty->Wait(lock);

    if(first==last){
        //only 1 element
        *keyPtr=first->key;
        ritem=first->item;
        first=NULL;
        last=NULL;
        delete first;
    }
    else{
        DLLElement *p=first;
        first=first->next;
        first->prev=NULL;
        *keyPtr=p->key;
        ritem=p->item;
        delete p;
    }
    printf("*** thread %s Removed %d ***\n", currentThread->getName(), *keyPtr);
    lock->Release();
    return ritem;
}

bool DLList::IsEmpty(){
    if(first==NULL){
        return true;
    }
    return false;
}

void DLList::SortedInsert(void *item, int sortKey){
    lock->Acquire();
    if(IsEmpty()){
        
        DLLElement *p= new DLLElement(item,sortKey);
        first=p;
        last=p;

        if (testnum==2 && strcmp(currentThread->getName(),"0")==0)
            currentThread->Yield();
    }
    else{// not empty
        DLLElement *pi=first;
        while(pi!=NULL&&pi->key<sortKey){
            pi=pi->next;
        }

		if (testnum == 4 && strcmp(currentThread->getName(), "0") == 0)
			currentThread->Yield();

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

                if (testnum == 2 && strcmp(currentThread->getName(), "0") == 0)
                    currentThread->Yield();

                p->next=pi;
                pi->prev->next=p;
                pi->prev=p;
            }
        }
    }
    printf("*** thread %s inserted %d ***\n",currentThread->getName(),sortKey);
    listEmpty->Broadcast(lock);
    lock->Release();
}

void *DLList::SortedRemove(int sortKey){
    lock->Acquire();
    while(IsEmpty())
        listEmpty->Wait(lock);

    DLLElement *pi=first;
    while(pi&&pi->key!=sortKey){
        pi=pi->next;
    }
    if(pi==NULL){// do not find
		printf("*** Remove failure: can't find! ***\n");
    }
    else{
        //match
        if(first==last){//only 1 elem
            if(pi==first){
                first=last=NULL;
            }
            delete pi;
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
        printf("*** thread %s Removed %d ***\n", currentThread->getName(), sortKey);
    }
    lock->Release();
}

void DLList::Show(){
    printf("\n***show list***\n");
    if(IsEmpty()){
        printf("*** Show list: List is empty! ***\n");
        return;
    }
    DLLElement * p=first;
    while(p!=NULL){
        printf("%d ",p->key);
        p=p->next;
    }
    printf("\n\n");
}

