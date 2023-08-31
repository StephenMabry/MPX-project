#include <mpx/io.h>
#include <mpx/clock.h>
#include <mpx/serial.h>
#include <man.h>
#include <sys_req.h>
#include <string.h>
#include <shutdown.h>
#include <mpx/vercom.h>
#include <mpx/disp.h>
#include <pcb.h>
#include <pcbCommand.h>
#include <r3Command.h>
#include <alarm.h>
#include <mcbCommand.h>

typedef struct { char *key; int val; } t_symstruct;
int quit = 0;
static t_symstruct lookuptable[] = {
    { "shutdown", 0xA0 },
	{ "vercom", 0xA1 },
	{ "gtime", 0xA2 },
	{ "gdate", 0xA3 },
	{ "stime", 0xA4 },
	{ "sdate", 0xA5 },
	{ "man", 0xA6},
//	{ "test", 0xD0},
	{ "deletePCB", 0xA8 },
	{ "blockPCB", 0xA9 },
	{ "unblockPCB", 0xAA },
	{ "suspendPCB", 0xAB },
	{ "resumePCB", 0xAC },
	{ "setPriority", 0xAD },
	{ "showPCB", 0xAE },
	{ "showReady", 0xAF },
	{ "showBlocked", 0xB0 },
	{ "showAll", 0xB1 },
	{ "loadR3", 0xB3 },
    	{ "alarm", 0xB4},
	{ "alloc", 0xC0},
	{ "free", 0xC1},
	{ "showAlloc", 0xC2},
	{ "showFree", 0xC3}
};
//Deleted for R3
	//{ "createPCB", 0xA7 },
//Deleted for R4
	//{ "yield", 0xB2 },

#define NKEYS (int)(sizeof(lookuptable)/sizeof(t_symstruct))

int keyfromstring(char *key){
    int i;
    for (i=0; i < NKEYS; i++) {
        t_symstruct *sym = &lookuptable[i];
        if (strcmp(sym->key, key) == 0)
            return sym->val;
    }
    return 0xF0;
}

void comhand(void){
	char welcome[] = "\n\t\e[38;2;255;0;0m@>\e[38;2;50;205;50m~~ WELCOME TO G5OS ~~\e[38;2;255;0;0m<@\n";
	
	disp(welcome);
	for(;;){
		char *cur = "\e[38;2;255;105;180m@\e[38;2;164;219;232m~\e[38;2;0;255;127m> \e[38;2;255;255;255m";
	    	disp(cur);	
		char *na = "\t\e[38;2;255;0;0mNot an available command\e[38;2;255;255;255m\n";
		char buf[64] = {0};
		
		int nread = sys_req(READ, COM1, buf, sizeof(buf));
		sys_req(WRITE, COM1, buf, nread);
		
		sys_req(IDLE);	

		switch (keyfromstring(buf)){
			case 0xA0:
				if(shutdown() == true){
					quit = 1;
					sys_req(EXIT);
					return;
				} else{
					break;
				}
			case 0xA1: vercom(); break;
			case 0xA2: getTime(); break;
			case 0xA3: getDate(); break;
			case 0xA4: setTime(); break;
			case 0xA5: setDate(); break;
			case 0xA6: man(); break;
			case 0xA7: createPCB(); break;
			//case 0xD0: test(); break;
			case 0xA8: deletePCB(); break;
			case 0xA9: blockPCB(); break;
			case 0xAA: unblockPCB(); break;
			case 0xAB: suspendPCB(); break;
			case 0xAC: resumePCB(); break;
			case 0xAD: setPriority(); break;
			case 0xAE: showPCB(); break;
			case 0xAF: showReady(); break;
			case 0xB0: showBlocked(); break;
			case 0xB1: showAll(); break;
			case 0xB2: yield(); break;
			case 0xB3: loadR3(); break;
			case 0xB4: loadAlarm(); break;
			case 0xC0: allocateMem(); break;
			case 0xC1: freeMem(); break;
			case 0xC2: showAllocated(); break;
			case 0xC3: showFree(); break;
			case 0xF0: 
				   if(buf[0] != '\0'){
					   disp(na);
				   }else{
					   break;
				   }
		}

		//Clears buffer
		memset(&buf,0,sizeof(buf));
	}
}
