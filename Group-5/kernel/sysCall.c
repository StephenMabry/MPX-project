#include <sys_req.h>
#include <sysCall.h>
#include <pcb.h>
#include <sys_req.h>
#include <mpx/com_handler.h>

pcb* executing = NULL;
context* first = NULL;

struct context* sys_call(struct context* cur){
	
	if((cur->eax != IDLE) && (cur->eax != EXIT)){
		cur->eax = -1;
                return cur; // register not set to idle or exit                
        }
        else if(cur->eax == IDLE){
		if(first == NULL){
			first = cur;
		}
		pcb* nextProcess = availablePCB();
		if(nextProcess == NULL){
			return cur;
		}
		
		if(executing != NULL){
			executing->stackPtr = (unsigned char*) cur;
		        pcb_insert(executing);	
		}
                executing = nextProcess;
		return (context*) nextProcess->stackPtr;
		
	}
        else {
        //EXIT process
		if(executing != NULL){
			pcb_free(executing);
		}
                pcb* nextProcess = availablePCB();
                if(nextProcess != NULL){
			executing = nextProcess;
                        return (context*) nextProcess->stackPtr;
                }else{
                        return first;
                }

        }

}


pcb* availablePCB(){
	pcb* nextProcess = ready->front;
	if(nextProcess == NULL){
		return NULL;
	}
	while(nextProcess->status == 1){
		nextProcess = nextProcess->next;
	}
		
	pcb_remove(nextProcess);
	return nextProcess;
}

