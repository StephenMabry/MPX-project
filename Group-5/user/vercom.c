#include <mpx/disp.h>

void vercom(void){
	char* version = strcat("\t\e[38;2;255;255;51mV.5.2\n\tCompiled on: ", __DATE__);
	version = strcat(version, "\n");
	disp(version);
}
