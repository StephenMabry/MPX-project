#include <mcbCommand.h>
#include <mcb.h>
#include <sys_req.h>
#include <mpx/disp.h>
#include <stdlib.h>
#include <integer.h>

void allocateMem(){
	char input[100] = {0};
	size_t size = 0;

	disp("Enter the size of memory you want to allocate in bytes: ");
	int nread = sys_req(READ, COM1, input, sizeof(input));
	sys_req(WRITE, COM1, input, nread);
	
	// if input is not a number continue
	if(!isnumber(input)){
		disp("\t\e[38;2;255;0;0mInvalid input.\e[38;2;255;255;255m\n");
		return;
	}

	//Available memory

	mcb* current = mcbList;
	size = (size_t) atoi(input);
	int flag = 0;

	while(current != NULL){

		if(current->status == 0 && current->size >= (size + sizeof(struct mcb))){
			
			flag = 1;
			break;
		
		}

		current = current->next;

	}

	if(flag == 0){
		
		disp("\t\e[38;2;255;0;0mNot enough memory available.\e[38;2;255;255;255m\n");
		return;

	}

	allocate_memory(size);

	disp("Allocated ");
	disp_int(size);
	disp(" bytes of memory.\n");
	
	return;
}

void freeMem(){
	char input[100] = {0};
	void* address;
	for(;;){
		disp("Enter the memory address of the MCB that you want to free: ");
		int nread = sys_req(READ, COM1, input, sizeof(input));
		sys_req(WRITE, COM1, input, nread);
		// valiade use input
		address =  (void*)hex2int(input);
		//disp_ptr((int)address);
		break;
	}
	

	int flag = 0;
	mcb* current = mcbList;
	while(current != NULL){

		if(current->address == address){

			flag = 1;
			break;

		}

		current = current->next;

	}

	if(flag == 0){

		disp("\t\e[38;2;255;0;0mAddress not found.\e[38;2;255;255;255m\n");
		return;

	}

	free_memory(address);

}

void showAllocated(){
	disp("\n\tAllocated List:\n");		
	mcb* current = mcbList;
	int blocks = 0;

	if(current != NULL){
		while(1){
			if(current->status == 1){
				blocks++;
				disp("\n");
                        	disp("\n\tAddress: ");
                        	disp_ptr((int)current->address);
                        	disp("\n\tSize: ");
                        	disp_int(current->size);
                        	disp("\n");
			}
                        if(current->next == NULL){
            			break;
                        }
			current = current->next;
		}
	}

	if(blocks<1){
		disp("\n\t\tAllocated list is empty\n");
	}
}

void showFree(){
        disp("\n\tFree List:\n");
        mcb* current = mcbList;
        int blocks = 0;

        if(current != NULL){
                while(1){
                        if(current->status == 0 && current->size > 0){
                                blocks++;
                                disp("\n");
                                disp("\n\tAddress: ");
                                disp_ptr((int)current->address);
                                disp("\n\tSize: ");
                                disp_int(current->size);
                                disp("\n");
                        }
                        if(current->next == NULL){
                                break;
                        }
                        current = current->next;
                }
        }

        if(blocks<1){
                disp("\n\t\tFree list is empty\n");
        }
}
