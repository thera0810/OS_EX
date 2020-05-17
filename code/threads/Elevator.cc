#include "Elevator.h"


extern Thread* currentThread;
extern Alarm* alarms;
extern int floors;
extern int capacity;

Elevator::Elevator(char* debugName, int numFloors, int myID)
{
	name=debugName;
	currentfloor=1;
	occupancy=0;
	floorCounts=numFloors;
	elevatorID=myID;
	dir=1;
	lock = new Lock("Elevator lock");
    int i;
    for (i=1;i<=numFloors;i++)
    {
    	floorCalled[i] = 0;
    	enterBar[i] = new EventBarrier();
    	exitBar[i] = new EventBarrier();
    }
}

Elevator::~Elevator()
{
    delete lock;
    int i;
    for (i=1;i<=floorCounts;i++)
    {
		delete enterBar[i];
		delete exitBar[i]; 
    }
}

void Elevator::OpenDoors()//   signal exiters and enterers to action
{
	DEBUG('E',"\033[1;33;40mElevator %d OpenDoors on %d floor.\033[m\n\n", elevatorID, currentfloor);
	if (exitBar[currentfloor]->Waiters()>0)
	{
		exitBar[currentfloor]->Signal();
	}
	if (enterBar[currentfloor]->Waiters()>0)
	{
		enterBar[currentfloor]->Signal();
	}
}

void Elevator::CloseDoors()//   after exiters are out and enterers are in
{
	//currentThread->Yield();
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
	DEBUG('E',"\033[1;33;40mElevator %d arrived %d floor.\033[m\n\n", elevatorID, currentfloor);
}

bool Elevator::Enter()//   get in
{	
	if (occupancy<capacity)
	{
		occupancy++;
		DEBUG('E',"\033[1;33;40mRider %s enter elevator %d on %d floor.\033[m\n\n",currentThread->getName(),elevatorID,currentfloor);
		enterBar[currentfloor]->Complete();
		return true;
	}
	else
	{
		DEBUG('E',"\033[1;33;40mFULL!! Rider %s can not enter elevator %d on %d floor.\033[m\n\n",currentThread->getName(),elevatorID,currentfloor);
		enterBar[currentfloor]->Complete();
		currentThread->Yield();
		return false;
	}
}

void Elevator::Exit()//   get out (iff destinationFloor)
{
	occupancy--;
	exitBar[currentfloor]->Complete();
	DEBUG('E',"\033[1;33;40mRider %s exit elevator %d on %d floor.\033[m\n\n",currentThread->getName(),elevatorID,currentfloor);
}

void Elevator::RequestFloor(int floor)//   tell the elevator our destinationFloor
{
	floorCalled[floor]=1;
	exitBar[floor]->Wait();
	//DEBUG('E',"\033[1;33;40mRider %s request floor %d in elevator %d.\033[m\n\n",currentThread->getName(),floor,elevatorID);
}

void Elevator::Operating()//   elevator operating forever
{
	int flag;
	while (true)
	{
		flag=0;
		if (dir==1) //UP
		{
			for (int i=currentfloor+1;i<=floorCounts;i++)
			{
				if (floorCalled[i]==1)
				{
					flag=1;
					VisitFloor(i);
					OpenDoors();
					CloseDoors();
				}
			}
			dir=0;
		}
		else if (dir==0)
		{
			for (int i=currentfloor-1;i>=1;i--)
			{
				if (floorCalled[i]==1)
				{
					flag=1;
					VisitFloor(i);
					OpenDoors();
					CloseDoors();
				}
			}
			dir=1;
		}

		if (flag==0)
			currentThread->Yield();
	}
}

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
	elevator->floorCalled[fromFloor]=1;
}

void Building::CallDown(int fromFloor)//   signal an elevator we want to go down
{
	elevator->floorCalled[fromFloor]=1;
}

Elevator* Building::AwaitUp(int fromFloor)// wait for elevator arrival & going up
{
	elevator->enterBar[fromFloor]->Wait();
	return elevator;
}

Elevator* Building::AwaitDown(int fromFloor)// wait for elevator arrival & going down
{
	elevator->enterBar[fromFloor]->Wait();
	return elevator;
}

void Building::StartElevator()// tell elevator to operating forever
{
	DEBUG('E',"==========Elevator Start Operating==========\n");
	elevator->Operating();
}
