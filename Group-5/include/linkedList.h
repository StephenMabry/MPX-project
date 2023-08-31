struct* pcb head = NULL;
struct* pcb current = NULL;
struct* pcb prev = NULL;

bool isEmpty(){
	return head == NULL;
}

int length(){
	int length = 0;
	struct* pcb current;

	for(current = head; current != NULL; current = current -> next){
		length++;
	}
}

struct* pcb deleteFirst(){
	struct* pcb temp = head;
	head = head -> next;

	return temp;
}

struct* pcb deleteKey(const char* name){
	struct pcb current = head;
	struct pcb prev = NULL;

	if(head == NULL){
		return NULL;
	}

	while(current -> pid != name){
		if(current -> next == NULL){
			return NULL;
		}
		else{
			prev = current;
			current = current -> next;
		}
	}

	if(current == head){
		head = head -> next;
	}
	else{
		prev -> next = current -> next;
	}
}

void insert(const char* name, struct pcb newPCB){
	struct* pcb temp;

	temp -> pid = name;
	temp -> state = newPCB -> state;
	temp -> class = newPCB -> class;
	temp -> priority = newPCB -> priority;
	temp -> next = head;
	temp -> prev = NULL;
	head -> prev = temp;
	head = temp;
}
