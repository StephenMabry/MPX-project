#include <mcb.h>
#include <mpx/vm.h>


mcb* mcbList;

void initialize_heap(size_t size){
	mcb* empty = kmalloc(size+sizeof(mcb),0,NULL);
	empty->address = empty + 1;
	empty->size = size;
	empty->status = 0;
	empty->next = NULL;
	empty->prev = NULL;
	mcbList = empty;
}

void *allocate_memory(size_t size){
	mcb* current = mcbList;
	mcb* newAlloc;
	mcb* newFree;
	size_t rem = 0;

	while(current != NULL){
		if((size <= current->size)&&(current->status == 0)){
			rem = current->size - size;
				
			newAlloc = current;
			newAlloc->size = size;
			newAlloc->status = 1;
			
			newFree = (mcb*)(newAlloc->size + (uint32_t) newAlloc->address);// pointer math with void *
			newFree->address = newFree + 1;
			newFree->size = rem - sizeof(struct mcb);
			newFree->status = 0;

			if(newAlloc->next != NULL){
				newFree->next = newAlloc->next;
			}
			
			newAlloc->next = newFree;
			newFree->prev = newAlloc;

			return newAlloc->address; //return actuall= add
		}
		else if(current->next ==  NULL){
			break;
		}
		current = current->next;
	}
	return NULL;

}

int free_memory(void* address){
	mcb* current = mcbList;

	while(current->address != address){
	//gets mcb that will be freed
		current = current->next;
		if(current == NULL){
			return -1;
		}

	}
	
	current->status = 0;

	if(current->next != NULL && current->next->status == 0){
	// merge right	
		current->size = current->size + current->next->size + sizeof(struct mcb);
		if(current->next->next != NULL){
			current->next->next->prev = current;
		}
		current->next = current->next->next;

	}

	if(current->prev != NULL && current->prev->status == 0){
	// merge left
		current->prev->size = current->prev->size + current->size + sizeof(struct mcb);
		if(current->next != NULL){
			current->next->prev = current->prev;	
		}
		
		current->prev->next = current->next;
	}
	
	return 0;
}

