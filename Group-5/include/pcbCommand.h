#ifndef MPX_PCBCOMMAND_H
#define MPX_PCBCOMMAND_H

#include <stddef.h>
#include <pcb.h>

//Create PCB
//Calls pcb setup() to create a PCB, then inserts it into the appropriate
//queue with pcb insert()
//Parameters: Process Name, Process Class, Process Priority
//Error Checking: Name must be unique and valid,Class must be valid, Priority must be valid
void createPCB();
//Delete PCB
//Finds the requested process, removes it from its queue with pcb remove(),
//and frees all associated memory with pcb free()
//Parameters: Process Name
//Error Checking: Name must be valid, Must not be a System process
void deletePCB();
//Block PCB
//Puts a process in the blocked state, and moves it the appropriate queue
//Parameters: Process Name
//Error Checking: Name must be valid
void blockPCB();
//Unblock PCB
//Puts a process in the unblocked (ready) state, and moves it the appropriate queue
//Parameters: Process Name
//Error Checking: Name must be valid
void unblockPCB();
//Suspend PCB
//Puts a process in the suspended state, and moves it the appropriate queue
//Parameters: Process Name
//Error Checking: Name must be valid, Must not be a System process
void suspendPCB();
//Resume PCB
//Puts a process in the not suspended state, and moves it the appropriate queue
//Parameters: Process Name
//Error Checking: Name must be valid
void resumePCB();
//Set Priority PCB
//Changes a process’s priority, and moves it to the appropriate place in the appropriate queue
//Parameters: Process Name, New Priority
//Error Checking: Name must be valid, Priority must be valid (0-9)
void setPriority();
//Show PCB
//Displays a process’s: Name, Class, State, Suspended Status, Priority
//Parameters: Process Name 
//Error Checking: Name must be valid
void showPCB();
//Show Ready
//For all processes in the Ready state, display the process’s: Name, Class, State, Suspended Status, Priority
//Parameters: None
//Error Checking: None
void showReady();
//Show Blocked
//For all processes in the Blocked state, display the process’s: Name, Class, State, Suspended Status, Priority
//Parameters: None
//Error Checking: None
void showBlocked();
//Show All
//For all processes (in any state), display the process’s: Name, Class, State, Suspended Status, Priority
//Parameters: None
//Error Checking: None
void showAll();

#endif
