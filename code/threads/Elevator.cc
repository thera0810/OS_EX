#include "Elevator.h"


extern Thread* currentThread;
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
	elevatorAlarm = new Alarm();
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
    delete elevatorAlarm;
    int i;
    for (i=1;i<=floorCounts;i++)
    {
		delete enterBar[i];
		delete exitBar[i]; 
    }
}

void Elevator::OpenDoors()//   signal exiters and enterers to action
{

}

void Elevator::CloseDoors()//   after exiters are out and enterers are in
{

}

void Elevator::VisitFloor(int floor)//   go to a particular floor
{

}

bool Elevator::Enter()//   get in
{
	return true;
}

void Elevator::Exit()//   get out (iff destinationFloor)
{

}

void Elevator::RequestFloor(int floor)//   tell the elevator our destinationFloor
{

}

void Elevator::Operating()//   elevator operating forever
{

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

}

void Building::CallDown(int fromFloor)//   signal an elevator we want to go down
{
	
}

Elevator* Building::AwaitUp(int fromFloor)// wait for elevator arrival & going up
{
	return elevator;
}

Elevator* Building::AwaitDown(int fromFloor)// wait for elevator arrival & going down
{
	return elevator;
}

void Building::StartElevator()// tell elevator to operating forever
{
	elevator->Operating();
}
