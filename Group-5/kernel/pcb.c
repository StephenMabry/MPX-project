#include <stddef.h>
#include <memory.h>
#include <priorityQ.h>
#include <string.h>
#include <stdlib.h>
#include <pcb.h>
#include <stdbool.h>
#include <processes.h>
#include <context.h>

queue *ready;
queue *blocked;

void createReady(){
	ready = sys_alloc_mem(sizeof(queue));
	if (ready == NULL){
		return;
	}
	ready->front = NULL;
	ready->rear = NULL;	
}

void createBlocked(){
	blocked = sys_alloc_mem(sizeof(queue));
	if (blocked == NULL){
		return;
	}
	blocked->front = NULL;
	blocked->rear = NULL;
}

/**
 * @brief Uses sys alloc mem() to allocate memory for a new PCB, including the stack, and perform reasonable initialization
 * 
 * @return pcb* A pointer to a newly allocated PCB on success, NULL on error during allocation or initialization
 */
pcb* pcb_allocate(void){
	pcb *process = sys_alloc_mem(sizeof(pcb));
	return process;
}

/**
 * @brief Uses sys free mem() to free all memory associated with a PCB, including the stack
 * 
 * @param pcb_to_free A pointer to the PCB to free
 * @return int A code indicating success or error
 */
int pcb_free(pcb* pcb_to_free){
	return sys_free_mem(pcb_to_free);
}


/**
 * @brief Allocates (via allocate pcb()) a new PCB, initializes it with data provided, and sets the state to Ready, Not-suspended
 * 
 * @param pname The process name
 * @param pclass The process class (user application or system process)
 * @param pprio The process priority (0-9)
 * @return pcb* A pointer to the initialized PCB on success, NULL on error allocating, initializing, or invalid parameter
 */
pcb* pcb_setup(char *pname, int pclass, int pprio){
        pcb *process = pcb_allocate();;
	strcpy(process->pid, pname);
	process->state = 0;
	process->status = 0;
	process->pClass = pclass;
	process->priority = pprio;
	process->next = NULL;
	process->stackPtr = process->stack + 1024 - sizeof(struct context);
	return process;
}

/**
 * @brief Searches all process queues for a process with the provided name
 * 
 * @param pname The name of the process to find
 * @return pcb* A pointer to the found PCB on success, NULL if the provided name was not found
 */
pcb* pcb_find(char *pname){
	pcb *temp = ready->front;
	
	for(;;){
		if(temp == NULL){  // checks if ready queue is empty
			break;
		} else if(strcmp(temp->pid, pname)==0){
			return temp;
		}
		else if(temp->next == NULL){
			break;
		}
		else{
			temp = temp->next;
		}
	}
	
	temp = blocked->front;
	for(;;){
		if(temp == NULL){  // checks if blocked queue is empty
			break;
		} else if(strcmp(temp->pid, pname)==0){
			return temp;
		}
		else if (temp->next == NULL){
			break;
		}
		else{
			temp = temp->next;
		}
	}
	return NULL;
}

/**
 * @brief Inserts a PCB into the appropriate queue, based on state and (if Ready) priority
 * 
 * @param in_pcb A pointer to the PCB to enqueue
 */
void pcb_insert(pcb* in_pcb){
	int state = in_pcb->state;
	if(state == 1){ // blocked
		push(blocked, in_pcb);
	}
	else if(state == 0){ //ready
		push(ready, in_pcb);
	}
	return;
}

/**
 * @brief Pushes a PCB to a selected queue
 * 
 * @param qPtr A pointer to the queue
 * @param in_pcb A pointer to the PCB to enqueue
 */
void push(queue* qPtr, pcb *in_pcb){
	pcb *curr = qPtr->front;
	int p = in_pcb->priority;
	if(in_pcb->state == 0){
		if(isEmpty(curr)){
        	qPtr->front = in_pcb;
        	qPtr->rear = in_pcb;
        }
		// Special Case: The head of list has lesser
		// priority than new node. So insert new
		// node before head node and change head node.
		else if (curr->priority > p){
			//Insert New Node before head
			in_pcb->next = qPtr->front;
			qPtr->front = in_pcb;
		}
		else{
        	// Traverse the list and find a position to insert new node
			while ((curr->next != NULL) && (curr->next->priority <= p)){
				curr = curr->next;
			}
			if(curr->next == NULL){
				curr->next = in_pcb;
				qPtr->rear = in_pcb;
			}else{
				in_pcb->next = curr->next;
				curr->next = in_pcb;
			}
		// Either at the ends of the list or at required position
		}
	}
	//blocked
	else{
		if(isEmpty(curr)){
			qPtr->front = in_pcb;
			qPtr->rear = in_pcb;
		}	
		qPtr->rear->next = in_pcb;
		qPtr->rear = in_pcb;
		qPtr->rear->next = NULL;
	}
}

/**
 * @brief Removes a PCB from its current queue, but does not free any associated memory or data structures
 * 
 * @param rm_pcb A pointer to the PCB to unqueue
 * @return int A success or error code
 */
int pcb_remove(pcb* rm_pcb){
	int state = rm_pcb->state;
	if(state == 1){ //blocked
		return removeQ(blocked, rm_pcb);
	}
	else if(state == 0){ //ready
		return removeQ(ready, rm_pcb);
	}
	return 0;
}

/**
 * @brief Removes PCB from defined queue
 * 
 * @param qPtr A pointer to the queue
 * @param rm_pcb The PCB to remove
 * @return int Returns 1 on success, 0 on failure
 */
int removeQ(queue *qPtr, pcb *rm_pcb){
	pcb *curr = qPtr->front;
	if(curr->next == NULL){ //only one element
		qPtr->front = NULL;
		qPtr->rear = NULL;
		rm_pcb->next = NULL;
		return 1;
	}
	if(qPtr->front == rm_pcb){ // remove the head
		qPtr->front = qPtr->front->next;
		rm_pcb->next = NULL;
		return 1; // success
	}
	while(curr->next != NULL){ 
		curr = curr->next;
	}
	qPtr->rear = curr; // set tail
	curr = qPtr->front;// reset head
	while(curr->next != NULL){
		if(curr->next == rm_pcb){
			if(curr->next == qPtr->rear){
				curr->next = NULL;
				qPtr->rear = curr;
				rm_pcb->next = NULL;
				return 1; // success
			}
			curr->next = curr->next->next;
		}
		curr = curr->next;
	}
	rm_pcb->next = NULL;
	return 0;
}

 /**
  * @brief Function to check if list is empty
  * 
  * @param head Head PCB of a queue
  * @return true List is empty
  * @return false List is not empty
  */
bool isEmpty(pcb* head){
	return head == NULL;
}
