#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <sys_req.h>
#include <stdbool.h>
#include <mpx/serial.h>
#include <mpx/device.h>
#include <pcb.h>
#include <pcbCommand.h>
void freeProcesses(void);
void freeQueue(queue* q);

/**
 * @brief Shutsdown the terminal after user confirmation.
 * 
 * @return true Shuts down the terminal
 * @return false Returns user to command line input
 */
bool shutdown(){
    char shutdown_msg[100] = "Are you sure you want to shutdown? [y/n]: ";

    for(;;){
	    disp(shutdown_msg);
        char buff[100] = {0};
        int nread = sys_req(READ, COM1, buff, sizeof(buff));
        sys_req(WRITE, COM1, buff, nread);

        if((strcmp(toLowerCase(buff),"y") == 0)){  // user confirmed shutdown
        	freeProcesses();
		char confirmed[100] = "Shutting down...\n";
        	sys_req(WRITE, COM1, confirmed, sizeof(confirmed));
        	return true;
        } else if(strcmp(toLowerCase(buff),"n") == 0){  // user denied shutdown
        	char denied[100] = "Shutdown denied, returning to command line...\n";
        	sys_req(WRITE, COM1, denied, sizeof(denied));
        	return false;
        }
	else{
		disp("Invalid input\n");
		continue;
	}
    }
}
void freeProcesses(){
	freeQueue(ready);
	freeQueue(blocked);	
}
void freeQueue(queue* q){
	pcb *process = q->front;
	while(process != NULL){
		pcb *temp = process->next;
		pcb_remove(process);
		pcb_free(process);
		process = temp;
	}
}
