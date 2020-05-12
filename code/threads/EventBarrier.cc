#include "EventBarrier.h"
#include "system.h"

EventBarrier::EventBarrier(){
    lock = new Lock("EventBarrier lock");
    condsig = new Condition("signal");
    condcom = new Condition("complete");
    state=0;//unsignaled
    cnt=0;
}

EventBarrier::~EventBarrier(){
    delete lock;
    delete condsig;
    delete condcom;
}

// Wait until the event is signaled. Return immediately if already in the signaled state.
void EventBarrier::Wait(){
    lock->Acquire();
    while(state==0){//unsignaled
        cnt++;
        DEBUG('e',"thread %s wait. condsig cnt = %d\n",currentThread->getName(),cnt);
        condsig->Wait(lock);
        cnt--;
    }
    Complete();
    lock->Release();
}

// Signal the event and block until all threads that wait for this event have responded. 
// The EventBarrier reverts to the unsignaled state when Signal() returns.
void EventBarrier::Signal(){
    lock->Acquire();
    state++;//signaled, can accumulate
    condsig->Broadcast(lock);
    DEBUG('e',"thread %s signaled\n",currentThread->getName());
    DEBUG('e',"thread %s wait to complete. condcom cnt = %d\n",currentThread->getName(),cnt);
    condcom->Wait(lock);
    state--;//recover to unsignaled
    lock->Release();
}

// Indicate that the calling thread has finished responding to a signaled event, and block
// until all other threads that wait for this event have also responded.
void EventBarrier::Complete(){
    ASSERT(cnt>=0);
    if(cnt>0){//last one to call complete
        DEBUG('e',"thread %s wait to complete. condcom cnt = %d\n",currentThread->getName(),cnt);
        condcom->Wait(lock);
    }
    else{
        condcom->Broadcast(lock);
        DEBUG('e',"thread %s complete.\n",currentThread->getName());
    }
}

// Return a count of threads that are waiting for the event or that have not yet responded to it.
int EventBarrier::Waiters(){
    return cnt;
}