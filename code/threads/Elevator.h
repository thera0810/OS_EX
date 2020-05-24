#include "EventBarrier.h"
#include "Alarm.h"
#include <string.h>
#include "synch.h"
#include "thread.h"
#include "system.h"

#define MAXF 100
#include <new>
class Building;
class Elevator;
/*

Here are the method signatures for the Elevator and Building classes.
You should feel free to add to these classes, but do not change the
existing interfaces.

*/

class Elevator {
   public:
     Elevator(char *debugName, int numFloors, int myID);
     ~Elevator();
     char *getName() { return name; }
   
     // elevator control interface: called by Elevator thread
     void OpenDoors();                //   signal exiters and enterers to action
     void CloseDoors();               //   after exiters are out and enterers are in
     void VisitFloor(int floor);      //   go to a particular floor
   
     // elevator rider interface (part 1): called by rider threads.
     bool Enter();                    //   get in
     void Exit();                     //   get out (iff destinationFloor)
     void RequestFloor(int floor);    //   tell the elevator our destinationFloor
   
     // insert your methods here, if needed
     void Operating();                //   elevator operating forever
     void printState();
     int noneedUp(int here);
     int noneedDown(int here);

     int floorCalled[MAXF];       //if the buttom inside elevator was pressed
     
     EventBarrier *exitBar[MAXF];       //exit barrier for floor i

     void SetBuilding(Building *b);
   private:
    void getelevID();  
   private:
     char *name;
     int currentfloor;           // floor where currently stopped
     int occupancy;              // how many riders currently onboard
   
     // insert your data structures here, if needed
     int floorCounts;        //number of floors
     int elevatorID;         //ID of elevator
     int dir;                //current direction: UP 1, DOWN 0; initialvalue 1
     int open;               //the state of the door, initial value=0
     Building *building;
};
   
class Building {
   public:
     Building(char *debugname, int numFloors, int numElevators);
     ~Building();
     char *getName() { return name; }
   
   				
     // elevator rider interface (part 2): called by rider threads
     void CallUp(int fromFloor);      //   signal an elevator we want to go up
     void CallDown(int fromFloor);    //   ... down
     Elevator *AwaitUp(int fromFloor); // wait for elevator arrival & going up
     Elevator *AwaitDown(int fromFloor); // ... down

     Elevator *GetElevator();

     void StartElevator();             // tell elevator to operating forever
     void GetUpID(int floor, int elevatorID); 
     void GetDownID(int floor, int elevatorID);
     Lock *lock;
     Condition *cond; //if noRiders' request, sleep

     int floorCalledUp[MAXF];     //if floor i's up buttom was pressed
     int floorCalledDown[MAXF];   //if floor i's down buttom was pressed
     EventBarrier *enterBarUp[MAXF];    //up enter barrier for floor i
     EventBarrier *enterBarDown[MAXF];  //
     int riderRequest;//state---riders' request num
   
   private:
     char *name;
     Elevator *elevator;         // the elevators in the building (array)
   
     // insert your data structures here, if needed
     int floorCounts;        //number of floors
     int elevatorCounts;     //number of elevators
     int *elevatorUpID;
     int *elevatorDownID;
     Lock *upIDLock;
     Lock *downIDLock;

};

   // here's a sample portion of a rider thread body showing how we
   // expect things to be called.
   //
   // void rider(int id, int srcFloor, int dstFloor) {
   //    Elevator *e;
   //  
   //    if (srcFloor == dstFloor)
   //       return;
   //  
   //    DEBUG('t',"Rider %d travelling from %d to %d\n",id,srcFloor,dstFloor);
   //    do {
   //       if (srcFloor < dstFloor) {
   //          DEBUG('t', "Rider %d CallUp(%d)\n", id, srcFloor);
   //          building->CallUp(srcFloor);
   //          DEBUG('t', "Rider %d AwaitUp(%d)\n", id, srcFloor);
   //          e = building->AwaitUp(srcFloor);
   //       } else {
   //          DEBUG('t', "Rider %d CallDown(%d)\n", id, srcFloor);
   //          building->CallDown(srcFloor);
   //          DEBUG('t', "Rider %d AwaitDown(%d)\n", id, srcFloor);
   //          e = building->AwaitDown(srcFloor);
   //       }
   //       DEBUG('t', "Rider %d Enter()\n", id);
   //    } while (!e->Enter()); // elevator might be full!
   //  
   //    DEBUG('t', "Rider %d RequestFloor(%d)\n", id, dstFloor);
   //    e->RequestFloor(dstFloor); // doesn't return until arrival
   //    DEBUG('t', "Rider %d Exit()\n", id);
   //    e->Exit();
   //    DEBUG('t', "Rider %d finished\n", id);
   // }

