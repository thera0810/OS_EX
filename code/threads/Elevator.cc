#include "Elevator.h"

extern Thread* currentThread;
extern Alarm* alarms;
extern int floors;
extern int capacity;

Elevator::Elevator(char* debugName, int numFloors, int myID)
{
	name=debugName;
	currentfloor=1;//to pick up rider on floor1
	occupancy=0;
	floorCounts=numFloors;
	elevatorID=myID;
	dir=0;
	open=0;
	lock = new Lock("Elevator lock");
    int i;
    for (i=1;i<=numFloors;i++)
    {
    	floorCalled[i] = 0;
    	floorCalledUp[i]=0;
    	floorCalledDown[i]=0;

    	enterBarUp[i] = new EventBarrier();
    	enterBarDown[i] = new EventBarrier();
    	exitBar[i] = new EventBarrier();
    }

    riderRequest=0;
    cond = new Condition("Elevator condition");
}

Elevator::~Elevator()
{
    delete lock;
    delete cond;
    int i;
    for (i=1;i<=floorCounts;i++)
    {
		delete enterBarUp[i];
		delete enterBarDown[i];
		delete exitBar[i]; 
    }
}

void Elevator::OpenDoors()//   signal exiters and enterers to action
{
	open=1;
	DEBUG('E',"\033[1;33;40mElevator %d OpenDoors on %d floor.\033[m\n\n", elevatorID, currentfloor);
	if (exitBar[currentfloor]->Waiters()>0)
		exitBar[currentfloor]->Signal();

	if (enterBarUp[currentfloor]->Waiters()>0&&dir==1)// move up and rider request up
		enterBarUp[currentfloor]->Signal();

	if(enterBarDown[currentfloor]->Waiters()>0&&dir==0)
		enterBarDown[currentfloor]->Signal();

}

void Elevator::CloseDoors()//   after exiters are out and enterers are in
{
	open=0;
	currentThread->Yield();//to get all riders' request and do better visit
	//currentThread->Yield();//to get all riders' request and do better visit
	// if(currentfloor==floorCounts&&dir==1)
	// 	dir=0;
	// else if(currentfloor==1&&dir==0)
	// 	dir=1;
	DEBUG('E',"\033[1;33;40mElevator %d CloseDoors on %d floor.\033[m\n\n", elevatorID, currentfloor);
}

void Elevator::VisitFloor(int floor)//   go to a particular floor
{
	if (floor>currentfloor)
		alarms->Pause(10*(floor-currentfloor));
	else
		alarms->Pause(10*(currentfloor-floor));

	currentfloor=floor;
	floorCalled[floor]=0;
	DEBUG('E',"\033[1;33;40mElevator %d arrived %d floor.\033[m\n", elevatorID, currentfloor);
}

bool Elevator::Enter()//   get in
{
	riderRequest--;
	// printf("*** rr:%d\n",riderRequest);
	ASSERT(riderRequest>=0);
	if (occupancy<capacity)
	{
		occupancy++;
		DEBUG('E',"\033[1;33;40mRider %s enter elevator %d on (%d) floor.\033[m\n",currentThread->getName(),elevatorID,currentfloor);
		if(dir==1){
			floorCalledUp[currentfloor]=0;
			enterBarUp[currentfloor]->Complete();
		}
		else{
			floorCalledDown[currentfloor]=0;
			enterBarDown[currentfloor]->Complete();
		}
		return true;
	}
	else
	{
		DEBUG('E',"\033[1;31;40mFULL!! Rider %s can not enter elevator %d on (%d) floor.\033[m\n\n",currentThread->getName(),elevatorID,currentfloor);
		if(dir==1){
			floorCalledDown[currentfloor]=0;
			enterBarUp[currentfloor]->Complete();
		}
		else{
			floorCalledDown[currentfloor]=0;
			enterBarDown[currentfloor]->Complete();
		}
		currentThread->Yield();//let elevator change state of EnterBar
		return false;
	}
}

void Elevator::Exit()//   get out (iff destinationFloor)
{
	occupancy--;
	DEBUG('E',"\033[1;33;40mRider %s exit elevator %d on (%d) floor.\033[m\n",currentThread->getName(),elevatorID,currentfloor);
	exitBar[currentfloor]->Complete();
}

void Elevator::RequestFloor(int floor)//   tell the elevator our destinationFloor
{
	floorCalled[floor]=1;//press the buttom inside the elevator
	DEBUG('E',"\033[1;33;40mRider %s RequestFloor(%d)\033[m\n\n",currentThread->getName(), floor);
	exitBar[floor]->Wait();
}

void Elevator::Operating()//   elevator operating forever
{
	int flag=0; //there is no UP/DOWN task
	while (true)
	{
		if (dir==1) //UP
		{
			for (int i=currentfloor+1;i<=floorCounts;i++)
			{
				flag = 0;
				if (floorCalledUp[i]==1||floorCalled[i]==1)
				{
					VisitFloor(i);
					OpenDoors();
					for (int j=i+1;j<=floorCounts;j++)
					{
						if (floorCalledUp[j]==1||floorCalled[j]==1)
						{
							flag=1; //there are still UP tasks
							break;
						}
					}
					if (flag==1) //there are still UP tasks, close the door and go UP
					{
						CloseDoors();
					}
				}
			}
			for(int i=floorCounts;i>=1;--i)//消除转向缺陷
			{
				if (floorCalledDown[i]==1&&enterBarDown[i]->Waiters()>0)//really called and waiting
				{
					if (i==currentfloor&&open==1) //the same floor and the door is open
					{
						dir = 0;
						floorCalledDown[currentfloor] = 0;
						if(enterBarDown[currentfloor]->Waiters()>0)
							enterBarDown[currentfloor]->Signal();
						CloseDoors();
					}
					else
					{
						VisitFloor(i);
						dir=0;
						OpenDoors();
						CloseDoors();
					}
					break;
				}
			}
			dir=0;// to pick fail-enterd rider
			// DEBUG('E',"\033[1;32;40mElevator ready to go downstairs.\033[m\n\n");
		}
		else if (dir==0)
		{
			for (int i=currentfloor-1;i>=1;i--)
			{
				flag=0;
				if (floorCalledDown[i]==1||floorCalled[i]==1)
				{
					VisitFloor(i);
					OpenDoors();
					for (int j=i-1;j>=1;j--)
					{
						if (floorCalledDown[j]==1||floorCalled[j]==1)
						{
							flag=1;//there are still DOWN tasks
							break;
						}
					}
					if (flag==1)//there are still DOWN tasks, close the door and go DOWN
					{
						CloseDoors();
					}
				}
			}
			for(int i=1;i<=floorCounts;i++)//消除转向缺陷
			{
				if (floorCalledUp[i]==1&&enterBarUp[i]->Waiters()>0)  //really called and waiting
				{
					if (i==currentfloor&&open==1)//the same floor and the door is open
					{
						dir = 1;
						floorCalledUp[currentfloor] = 0;
						if(enterBarUp[currentfloor]->Waiters()>0)
							enterBarUp[currentfloor]->Signal();
						CloseDoors();
					}
					else
					{
						VisitFloor(i);
						dir=1;
						OpenDoors();
						CloseDoors();
					}
					break;
				}
			}
			dir=1;
			// DEBUG('E',"\033[1;32;40mElevator ready to go upstairs.\033[m\n\n");
		}

		//no rider in or request elevator, sheep
		// static int dml=0;
		// dml++;
		// if(dml==10)
		// 	printState();

		lock->Acquire();
		while(riderRequest==0&&occupancy==0){
			DEBUG('E',"\033[1;32;40mNo task or request, elevator sleep\033[m\n\n");
			cond->Wait(lock);
			DEBUG('E',"\033[1;32;40melevator start working\033[m\n\n");
		}
		lock->Release();
	}
}

void Elevator::printState(){//print current state of the elevator
	printf("** riderRequest:%d\n", riderRequest);
	printf("** occupancy:%d\n", occupancy);
	printf("** currentfloor:%d\n",currentfloor);
	printf("** floor called: ");
	for(int i=1;i<=floorCounts;++i)
		if(floorCalled[i])
			printf("%d ",i);
	printf("\n** floor calledUp: ");
	for(int i=1;i<=floorCounts;++i)
		if(floorCalledUp[i])
			printf("%d ",i);
	printf("\n** floor calledDown: ");
	for(int i=1;i<=floorCounts;++i)
		if(floorCalledDown[i])
			printf("%d ",i);
	printf("\n");
}
//******************************building ****************************************

Building::Building(char *debugname, int numFloors, int numElevators)
{
	name=debugname;
	floorCounts=numFloors;
	elevatorCounts=numElevators;
	elevator = new Elevator("E1",numFloors,1);
}

Building::~Building()
{
	delete elevator;
}

void Building::CallUp(int fromFloor)//   signal an elevator we want to go up
{
	elevator->riderRequest++;
	elevator->floorCalledUp[fromFloor]=1;//press floor's up buttom
	
	elevator->lock->Acquire();
	elevator->cond->Broadcast(elevator->lock);
	elevator->lock->Release();
	
}

void Building::CallDown(int fromFloor)//   signal an elevator we want to go down
{
	elevator->riderRequest++;
	elevator->floorCalledDown[fromFloor]=1;
	
	elevator->lock->Acquire();
	elevator->cond->Broadcast(elevator->lock);
	elevator->lock->Release();
	
}

Elevator* Building::AwaitUp(int fromFloor)// wait for elevator arrival & going up
{
	elevator->enterBarUp[fromFloor]->Wait();
	/*
	elevator->lock->Acquire();
	elevator->cond->Broadcast(elevator->lock);
	elevator->lock->Release();
	*/
	return elevator;
}

Elevator* Building::AwaitDown(int fromFloor)// wait for elevator arrival & going down
{
	elevator->enterBarDown[fromFloor]->Wait();
	/*
	elevator->lock->Acquire();
	elevator->cond->Broadcast(elevator->lock);
	elevator->lock->Release();
	*/
	return elevator;
}

void Building::StartElevator()// tell elevator to operating forever
{
	DEBUG('E',"\n==========Elevator Start Operating==========\n\n");
	elevator->Operating();
}

