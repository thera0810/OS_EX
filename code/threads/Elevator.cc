#include "Elevator.h"

extern Thread* currentThread;
extern Alarm* alarms;
extern int floors;
extern int capacity;

Elevator::Elevator(char* debugName, int numFloors, int myID)
{
	name=debugName;
	currentfloor=0;//to pick up rider on floor1
	occupancy=0;
	floorCounts=numFloors;
	elevatorID=myID;
	dir=1;
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
	currentThread->Yield();//to get all riders' request and do better visit
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
		DEBUG('E',"\033[1;33;40mFULL!! Rider %s can not enter elevator %d on (%d) floor.\033[m\n\n",currentThread->getName(),elevatorID,currentfloor);
		if(dir==1){
			floorCalledDown[currentfloor]=0;
			enterBarUp[currentfloor]->Complete();
		}
		else{
			floorCalledDown[currentfloor]=0;
			enterBarDown[currentfloor]->Complete();
		}
		// currentThread->Yield();
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
	// int flag;
	while (true)
	{
		// flag=0;
		if (dir==1) //UP
		{
			for (int i=currentfloor+1;i<=floorCounts;i++)
			{
				if (floorCalledUp[i]==1||floorCalled[i]==1)
				{
					// flag=1;
					VisitFloor(i);
					OpenDoors();
					CloseDoors();
				}
			}
			currentfloor=floorCounts+1;
			dir=0;// ready to down
			DEBUG('E',"\033[1;32;40mElevator ready to go downstairs.\033[m\n\n");
		}
		else if (dir==0)
		{
			for (int i=currentfloor-1;i>=1;i--)
			{
				if (floorCalledDown[i]==1||floorCalled[i]==1)
				{
					// flag=1;
					VisitFloor(i);
					OpenDoors();
					CloseDoors();
				}
			}
			currentfloor=0;
			dir=1;
			DEBUG('E',"\033[1;32;40mElevator ready to go upstairs.\033[m\n\n");
		}

		// if (flag==0)
		// 	currentThread->Yield();

		//no rider in or request elevator, sheep
		static int dml=0;

		dml++;
		if(dml==10)
			printState();
		lock->Acquire();
		while(riderRequest==0&&occupancy==0){
			DEBUG('E',"\033[1;32;40mNo task or request, elevator sleep\033[m\n\n");
			cond->Wait(lock);
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
	return elevator;
}

Elevator* Building::AwaitDown(int fromFloor)// wait for elevator arrival & going down
{
	elevator->enterBarDown[fromFloor]->Wait();
	return elevator;
}

void Building::StartElevator()// tell elevator to operating forever
{
	DEBUG('E',"\n==========Elevator Start Operating==========\n\n");
	elevator->Operating();
}

