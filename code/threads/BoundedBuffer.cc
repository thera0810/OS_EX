#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int maxsize){
    lock= new Lock("BF lock");
    Empty=new Condition("Empty Condition");
    Full=new Condition("Full Condition");
    maxsize+=1;//one more byte to distinguish empty and full
    bottom=first=last=new char[maxsize];
    top=bottom+maxsize-1;
    len=maxsize;
}
BoundedBuffer::~BoundedBuffer(){
    delete lock;
    delete Empty;
    delete Full;
    delete first;
}

// read 'size' bytes from the bounded buffer, storing into 'data'.
// ('size' may be greater than 'maxsize')
void BoundedBuffer::Read(void *data, int size){
    lock->Acquire();
    while((first-last+len)%len<size)//not full enough
        Full->Wait(lock);
    //copy last:last+size to data
    if(top-last+1>size){ //last to top is enough
        memcpy((char*)data,last,size);
        last+=size;
    }
    else{ //last to top is not enough
        int t=top-last+1;
        memcpy((char*)data,last,t);
        size-=t;
        if(size)
            memcpy((char*)data+t,bottom,size);
        last=bottom+size;
        ASSERT(first-last>=0);
    }
    Empty->Broadcast(lock);
    lock->Release();
}

// write 'size' bytes from 'data' into the bounded buffer.
// ('size' may be greater than 'maxsize')
void BoundedBuffer::Write(void *data, int size){
    lock->Acquire();
    while(len-1-(first-last+len)%len<size)
        Empty->Wait(lock);
    //copy data to first:first+size
    if(top-first+1>size){
        memcpy((char*)data,first,size);
        first+=size;
    }
    else{
        int t=top-first+1;
        memcpy((char*)data,first,t);
        size-=t;
        if(size)
            memcpy((char*)data+t,bottom,size);
        first=bottom+size;
        ASSERT(last-first>=1);
    }
    Full->Broadcast(lock);
    lock->Release();
}

void BoundedBuffer::ShowState(){
    printf("0=========================%d\n",len-1);
    printf("first at %d\n",first-bottom);
    printf("last  at %d\n\n",last-bottom);
}