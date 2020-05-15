#include <stdlib.h>
#include <string.h>
#include "Alarm.h"
#include "thread.h"
#include "system.h"

extern Thread* currentThread;
extern int testnum;
extern Alarm *alarms;

//To avoid System shut down when there is no thread runnable.
//Setup an additional thread when there is only one item in alarmQueue.
void sentinel(int when)
{
    while(!(alarms->CheckEmpty()))
    {
    	//DEBUG('a', "\033[1;33;40m%s Yield.\033[m\n\n",currentThread->getName());
        currentThread->Yield();
    }
    DEBUG('a',"\033[1;33;40m%s Finished.\033[m\n\n", currentThread->getName());
    currentThread->Finish();
}

Alarm::Alarm()
{
	//name=debugName;
    alarmQueue = new List();
    waiternum = 0;
}

Alarm::~Alarm()
{
    delete alarmQueue;
}

bool Alarm::CheckEmpty()
{
	if(waiternum==0)
		return TRUE;
	else
		return FALSE;
}

void Alarm::Pause(int howLong)
{
	if(howLong < 0 )
    {
    	DEBUG('a',"\033[1;33;40mThread%s alarm failed, howLong is negative.\033[m\n\n", currentThread->getName());
        return;
    }

	waiternum++;
	int wakeTime=-1;
	printf("Thread%s set an alarm for %d Ticks.\n\n", currentThread->getName(), howLong);
	if(waiternum==1)	//only one thread waiting for the alarm
    {
        Thread *t = new Thread("CheckThread");  //create a check thread
        t->Fork(sentinel, 0);
    	DEBUG('a',"\033[1;33;40mCheckThread has been created.\033[m\n\n");
    }

    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // disable interrupts

    wakeTime = stats->totalTicks + howLong;		//set the Alarm
	DEBUG('a',"\033[1;33;40mThread%s SLEEP, it will wake up at %d totalTicks.\033[m\n\n", currentThread->getName(), wakeTime);
    alarmQueue->SortedInsert((void *)currentThread, wakeTime);  //insert into queue
    currentThread->Sleep();

    (void) interrupt->SetLevel(oldLevel);   // re-enable interrupts
}


void Alarm::Awaken()
{
    int when = -1;
    Thread *thread = NULL;
    //DEBUG('a',"\033[1;33;40mTimer interrupt handler is going to wake up threads at %d totalTicks.\033[m\n\n", stats->totalTicks);

    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // disable interrupts

    thread = (Thread*)alarmQueue->SortedRemove(&when);	//capture the first thread and  
    													//its wakeTime in the alarmQueue.
    while(thread != NULL)
    {
        if( when <= stats->totalTicks)   //If there is any thread can be waked up this time.
        {
            scheduler->ReadyToRun(thread);  //set runnable status
            DEBUG('a',"\033[1;33;40mTimer interrupt handler wake up a thread%s successfully at %d totalTicks.\033[m\n\n", thread->getName(),stats->totalTicks);
            thread = (Thread *)alarmQueue->SortedRemove(&when);	//capture next thread
            waiternum--;
        }
        else
        {
            alarmQueue->SortedInsert(thread, when);  //insert back into the alarmQueue
            break;
        }
    }

    (void) interrupt->SetLevel(oldLevel);   // re-enable interrupts
}

