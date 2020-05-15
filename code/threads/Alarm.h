#ifndef ALARM_H
#define ALARM_H
#include "list.h"

class Alarm
{
public:
    Alarm();
    ~Alarm();
    //char* getName() { return name; }    // debugging assist

    void Pause(int howLong); // an atomic operation.
                             // let the thread which calls the function
                             // sleep howLong unit times.
                             // If it finds that it's the only thread
                             // waiting for the Alarm,
                             // create a check thread
                             // which check the count of threads
                             // waiting for Alarm
                             // from the beginning to the end.
    void Awaken();           // an atomic operation.
                             // Be called as the time interrupt generates
                             // it has not been added yet.
                             // Traversing the list to find the thread
                             // which should be awoken.
    bool CheckEmpty();

private:
    //char* name;              // for debugging
    List *alarmQueue;        // a queue record the thread which calls the function and sleep
    int waiternum;           // the count of threads already in the alarmQueue
};



#endif // ALARM_H
