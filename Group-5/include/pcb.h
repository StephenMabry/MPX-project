#ifndef MPX_PCB_H
#define MPX_PCB_H

#include <stddef.h>
#include <priorityQ.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/disp.h>
#include <stdbool.h>
#include <context.h>

typedef struct pcb{
	char pid[100];
	int state; // 0 is ready 1 is blocked
	int pClass;
	int priority;
	int status; // 0 is not suspended 1 is suspended
	unsigned char *stackPtr;
	unsigned char stack[1024];
	struct pcb* next;
}pcb;

typedef struct LinkedList{
	pcb* front;
	pcb* rear;
}queue;

extern queue *blocked;
extern queue *ready;

void createReady();
void createBlocked();

//Uses sys alloc mem() to allocate memory for a new PCB, including the stack, and perform reasonable initialization
//Parameters: None
//Returns: A pointer to a newly allocated PCB on success, NULL on error during allocation or initializat
pcb* pcb_allocate(void);

//Uses sys free mem() to free all memory associated with a PCB, including the stack
//Parameters: A pointer to the PCB to free
//Returns: A code indicating success or error
int pcb_free(pcb* pcb_to_free);

//Allocates (via allocate pcb()) a new PCB, initializes it with data provided, and sets the state to Ready, Not-suspended
//Parameters: The process name, the process class (user application or system process), the process priority (0-9)
//Returns: A pointer to the initialized PCB on success, NULL on error allocating, initializing, or invalid parameter
pcb* pcb_setup(char *pname, int pclass, int pprio);

//Searches all process queues for a process with the provided name
//Parameters: The name of the process to find
//Returns: A pointer to the found PCB on success, NULL if the provided name was not found
pcb* pcb_find(char *pname);

//Inserts a PCB into the appropriate queue, based on state and (if Ready) priority
//Parameters: A pointer to the PCB to enqueue
//Returns: None
void push(queue *qPtr, pcb* in_pcb);

void pcb_insert(pcb* in_pcb);
//Removes a PCB from its current queue, but does not free any associated memory or data structures
//Parameters: A pointer to the PCB to unqueue
//Returns: A success or error code
int pcb_remove(pcb* rm_pcb);

int removeQ(queue *qPtr, pcb *rm_pcb);

bool isEmpty(pcb* head);

int test();

#endif
