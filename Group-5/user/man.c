#include <mpx/disp.h>

/**
 * @brief Provides information about each command
 * 
 */
void man(){
    //As commands get added, please add descriptions
	char *help = "man - Provides a list of all available commands.\n"
	    "vercom - Provides the current version and the date the version was released.\n"
	    "gtime - Returns the current time.\n"
	    "gdate - Returns the current date.\n"
	    "stime - Allows the user to set the time.\n"
	    "sdate - Allows the user to set the date.\n"
	    "shutdown - Terminates terminal session.\n"
	    "deletePCB - Deletes a PCB from the queue when given a name.\n"
	    "blockPCB - Adds a PCB to a blocked queue that will prevent it from being executed.\n"
	    "unblockPCB - Removes a PCB from the list of blocked PCBs.\n"
	    "suspendPCB - Suspends a process and puts it in a queue based on resources available.\n"
	    "resumePCB - Removes a process from the suspended state.\n"
	    "setPriority - Sets the priority of a PCB already in the queue, moving its position.\n"
	    "showPCB - Displays the details of an individual PCB.\n"
	    "showReady - Displays the details of all of the PCBs in the ready state.\n"
	    "showBlocked - Displays the details of all of the PCBs in the blocked state.\n"
	    "showAll - Displays the details of all PCBs, regardless of current state.\n"
	    "loadR3 - Loads the R3 test processes and saves their context.\n"
	    "alarm - IDLEs a process for a specified time\n"
	    "alloc - allocate space for allocated memory\n"
	    "free - frees space in heap\n"
	    "showAlloc - shows the allocated MCBs\n"
	    "showFree - shows the free MCBs\n";
    		
	disp(help);

}
// deleted for R3 "createPCB - Creates a process control block with a name, priority, and class.\n"
// deleted for R4 "yield - Causes the CPU to yeild all processes\n";
