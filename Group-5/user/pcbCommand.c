#include <stddef.h>
#include <pcb.h>
#include <sys_req.h>
#include <mpx/disp.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Calls pcb_setup() to create a PCB, then inserts it into the appropriate queue with pcb_insert()
 * 
 */
void createPCB(){
	char name[100] = {0};
	int class = 0;
	int pprio = 0;
	for(;;){
		char* msg = "\tEnter the PCB name (Max 10 characters): ";
		disp(msg);

		int nread = sys_req(READ, COM1, name, sizeof(name));
		sys_req(WRITE, COM1, name, nread);
		if(strlen(name) > 10){
			msg = "\t\e[38;2;255;0;0mThe name you entered is greater than 10 characters\e[38;2;255;255;255m\n";
			disp(msg);
			continue;	
		}
		if(pcb_find(toLowerCase(name)) != NULL){
			disp("\t\e[38;2;255;0;0mThis name is already in use, each PCB must be unique\e[38;2;255;255;255m\n");
			continue;
		}
		break;
	}
	
	for(;;){
		char *msg = "\tEnter to specify class USER[0] / SYSTEM[1]: "; 
		disp(msg);
		char classInput[100] = {0};
		char nread = sys_req(READ, COM1, classInput, sizeof(classInput));
		sys_req(WRITE, COM1, classInput, nread);	
		if(strcmp(classInput, "1") == 0 || strcmp(classInput, "0") == 0){
			class = atoi(classInput);
			break;
		}
		//accepts Characters BAD
		else{ 
			disp("\t\e[38;2;255;0;0mInvalid Input\e[38;2;255;255;255m\n");
			continue;
		}
	}

	for(;;){
		char *msg = "\tEnter a priority level 0-9: ";
		disp(msg);
		char input[100] = {0};
		char nread = sys_req(READ, COM1, input, sizeof(input));
		sys_req(WRITE, COM1, input, nread);
		pprio = atoi(input);
		if(pprio == 0){
			if(strcmp(input, "0") == 0){
				break;
			}
		}
		else if(pprio >= 1 && pprio <= 9){
			break;
		}
		msg = "\t\e[38;2;255;0;0mInput is not valid\e[38;2;255;255;255m\n";
		disp(msg);
	}
	pcb* in_pcb = pcb_setup(toLowerCase(name), class, pprio);
	pcb_insert(in_pcb);
}

/**
 * @brief Finds the requested process, removes it from its queue with pcb remove(),
          and frees all associated memory with pcb free()
 * 
 */
void deletePCB(){
	char name[100] = {0};
	for(;;){
		char *msg = "\tEnter the name of the process you want to delete: ";
		disp(msg);
		int nread = sys_req(READ, COM1, name, sizeof(name));
		sys_req(WRITE, COM1, name, nread);
		if(pcb_find(toLowerCase(name)) == NULL){
			msg = "\t\e[38;2;255;0;0mDelete PCB not found\e[38;2;255;255;255m\n";
			disp(msg);
			continue;
		}
		if(pcb_find(toLowerCase(name))->pClass == 1){
			disp("\t\e[38;2;255;0;0mCannot delete a system process\e[38;2;255;255;255m\n");
			continue;
		}
		break;
	}
	pcb_remove(pcb_find(toLowerCase(name)));
	disp("\tPCB deleted\n");
}

/**
 * @brief Puts a process in the blocked state, and moves it the appropriate queue
 * 
 */
void blockPCB(){
	char name[100] = {0};

	if(isEmpty(blocked->front) && isEmpty(ready->front)){
		disp("\t\e[38;2;255;0;0mAll queues are empty, nothing can be blocked\e[38;2;255;255;255m\n");
		return;
	}

	disp("Enter the name of the process you want to block: ");
	int nread = sys_req(READ, COM1, name, sizeof(name));
	sys_req(WRITE, COM1, name, nread);

	if(pcb_find(toLowerCase(name)) == NULL){
		disp("\t\e[38;2;255;0;0mPCB not found\e[38;2;255;255;255m\n");
	} else{
		pcb *block = pcb_find(toLowerCase(name));
		pcb_remove(block);
		block->state = 1;
		pcb_insert(block);

		disp("\tThe PCB was moved into the Blocked queue\n");
	}
}

/**
 * @brief Puts a process in the unblocked (ready) state, and moves it the appropriate queue
 * 
 */
void unblockPCB(){
	char name[100] = {0};

	if(isEmpty(blocked->front) && isEmpty(ready->front)){
		disp("\t\e[38;2;255;0;0mAll queues are empty, nothing can be unblocked\e[38;2;255;255;255m\n");
		return;
	}

	disp("\tEnter the name of the process you want to unblock: ");
	int nread = sys_req(READ, COM1, name, sizeof(name));
	sys_req(WRITE, COM1, name, nread);
	
	

	if(pcb_find(toLowerCase(name)) == NULL){
		disp("\t\e[38;2;255;0;0mPCB not found\e[38;2;255;255;255m\n");
	} else{
		//put process into unblocked queue
		pcb *unblock = pcb_find(toLowerCase(name));
		pcb_remove(unblock);
		unblock->state = 0;
		pcb_insert(unblock);

		disp("\tThe PCB was moved into the Ready queue\n");
	}
}

/**
 * @brief Puts a process in the suspended state, and moves it the appropriate queue
 * 
 */
void suspendPCB(){
	char name[100] = {0};

	disp("\tEnter the name of the process you want to suspend: ");
	int nread = sys_req(READ, COM1, name, sizeof(name));
	sys_req(WRITE, COM1, name, nread);

	if(pcb_find(toLowerCase(name)) == NULL){
		disp("\t\e[38;2;255;0;0mPCB not found\e[38;2;255;255;255m\n");
	} else if(pcb_find(toLowerCase(name))->pClass == 1){
		disp("\t\e[38;2;255;0;0mCannot suspend a system process\t\e[38;2;255;255;255m\n");
	} else{
		//put process into suspended state
		pcb *suspend = pcb_find(toLowerCase(name));
		pcb_remove(suspend);
		suspend->status = 1;
		pcb_insert(suspend);

		disp("\tThe PCB was suspended\n");
	}
}

/**
 * @brief Puts a process in the not suspended state, and moves it the appropriate queue
 * 
 */
void resumePCB(){
	char name[100] = {0};

	disp("\tEnter the name of the process you want to resume: ");
	int nread = sys_req(READ, COM1, name, sizeof(name));
	sys_req(WRITE, COM1, name, nread);

	if(pcb_find(toLowerCase(name)) == NULL){
		disp("\t\e[38;2;255;0;0mPCB not found\e[38;2;255;255;255m\n");
	} else{
		//put process into not suspended state
		pcb *resume = pcb_find(toLowerCase(name));
		pcb_remove(resume);
		resume->status = 0;
		pcb_insert(resume);
		disp("\tThe PCB was resumed\n");
	}
}

/**
 * @brief Changes a process’s priority, and moves it to the appropriate place in the appropriate queue
 * 
 */
void setPriority(){
	int pprio = 0;
	char name[100] = {0};
	char input[100] = {0};

	disp("Enter the name of the process you want to change: ");
	int nread = sys_req(READ, COM1, name, sizeof(name));
	sys_req(WRITE, COM1, name, nread);

	if(pcb_find(toLowerCase(name)) == NULL){
		disp("\t\e[38;2;255;0;0mPCB not found\e[38;2;255;255;255m\n");
		return;
	}

	disp("\tEnter a priority level 0-9: ");
	nread = sys_req(READ, COM1, input, sizeof(input));
	sys_req(WRITE, COM1, input, nread);

	pprio = atoi(input);
	if(pprio <= 0 || pprio >= 9){
		disp("\t\e[38;2;255;0;0mInput is not valid\e[38;2;255;255;255m\n");
		return;
	}

	//change the priority level of the processe
	pcb *process = pcb_find(toLowerCase(name));
	pcb_remove(process);
	process->priority = pprio;
	pcb_insert(process);
	disp("\tThe PCB was changed\n");
}

/**
 * @brief Displays a process’s: Name, Class, State, Suspended Status, Priority
 * 
 */
void showPCB(){
	pcb* found;
	char name[100] = {0};
	for(;;){
		char *msg = "\tEnter the name of the process you want to see: ";
		disp(msg);
		int nread = sys_req(READ, COM1, name, sizeof(name));
		sys_req(WRITE, COM1, name, nread);
		break;
	}
	if(pcb_find(toLowerCase(name)) == NULL){
		disp("\t\e[38;2;255;0;0mPCB not found\e[38;2;255;255;255m\n");
		return;
	}
	found = pcb_find(toLowerCase(name));
	
	disp("\n\tName: ");
	disp(found -> pid);
    disp("\n\tClass: ");
	
	if(found -> pClass == 0){
		disp("User");
	} else{
		disp("System");
	}
	disp("\n\tPriority: ");
	disp_int(found -> priority);
	disp("\n\tStatus: ");
	if(found -> status == 1){
		disp("Suspended");
	} else{
        disp("Not Suspended");
	}
	disp("\n\tState: ");
	if(found->state == 0){
		disp("Ready\n");
	} else{
		disp("Blocked\n");
	}
}

/**
 * @brief For all processes in the Ready state, display the process’s: Name, Class, State, Suspended Status, Priority
 * 
 */
void showReady(){
	pcb* current = ready->front;
	
	if(current == NULL){
		disp("\n\tReady queue is empty.\n");
	} else{
		disp("\n\t\e[38;2;0;255;0mReady PCBs:\e[38;2;255;255;255m\n");

		while(current != NULL){
			disp("\n\tName: ");
			disp(current -> pid);

			disp("\n\tClass: ");
			if(current -> pClass == 0){
				disp("User");
			} else{
				disp("System");
			}

			disp("\n\tPriority: ");
			disp_int(current -> priority);

			disp("\n\tStatus: ");
			if(current -> status == 1){
				disp("Suspended");
			} else{
				disp("Not Suspended");
			}

			disp("\n\tState: Ready\n");
			current = current -> next;
		}
	}
}

/**
 * @brief For all processes in the Blocked state, display the process’s:
          Name, Class, State, Suspended Status, Priority
 * 
 */
void showBlocked(){
	pcb* current = blocked->front;
	if (current == NULL){
		disp("\n\tBlocked queue is empty. \n");
	}
	else{
		disp("\n\t\e[38;2;255;0;0mBlocked PCBs:\e[38;2;255;255;255m\n");
		while(current != NULL){
			disp("\n\tName: ");
			disp(current -> pid);
			disp("\n\tClass: ");
			
			if(current -> pClass == 0){
				disp("User");
			} else{
				disp("System");
			}
			
			disp("\n\tPriority: ");
			disp_int(current -> priority);
			disp("\n\tStatus: ");
			
			if(current -> status == 1){
				disp("Suspended");
			} else{
				disp("Not Suspended");
			}
			
			disp("\n\tState: Blocked\n");
			current = current -> next;
		}
	}
}

/**
 * @brief For all processes (in any state), display the process’s:
 		  Name, Class, State, Suspended Status, Priority
 * 
 */
void showAll(){
	showReady();
	showBlocked();
}

