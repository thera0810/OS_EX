#include "synch.h"

class EventBarrier{
public:
    EventBarrier();
    ~EventBarrier();
    // Wait until the event is signaled. Return immediately if already in the signaled state.
    void Wait();
    // Signal the event and block until all threads that wait for this event have responded. 
    // The EventBarrier reverts to the unsignaled state when Signal() returns.
    void Signal();
    // Indicate that the calling thread has finished responding to a signaled event, and block
    // until all other threads that wait for this event have also responded.
    void Complete();
    // Return a count of threads that are waiting for the event or that have not yet responded to it.
    int Waiters();
private:
    Lock * lock;
    Condition *condsig,*condcom;
    int state,cnt;
};
