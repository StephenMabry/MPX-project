#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys_req.h>
#include <mpx/serial.h>
#include <mpx/device.h>
#include <mpx/io.h>

/**
 * @brief Outpus a character pointer to the console
 * 
 * @param input 
 */
void disp(char *input){
    sys_req(WRITE, COM1, input, strlen(input));
    memset(&input,0,sizeof(input));    
}

/**
 * @brief Outpus an integer to the console
 * 
 * @param i 
 */
void disp_int(int i){
    int n = 1;
    int done = 0;
    int check = 0;
    int places = 0;
    char output[100] = {0};
    int a;
    while(done != 1){
	check = i/n;
	if(check >= 1){
	    n = n*10;
	    places++;
	}
	else{
	    if(places == 0){
		n = n*10;
	        places++;
	    }
	    done = 1;
	}

    }
    for(int k = 0;k<places;k++){
	n = n/10;
	a = i/n;
	output[k] = itoa(a);
	i = i-a*n;
    }
    sys_req(WRITE,COM1,output,sizeof(output));
    memset(&output,0,sizeof(output));

}

void disp_ptr(int i){
    int n = 1;
    int done = 0;
    int check = 0;
    int places = 0;
    char output[100] = {0};
    int a;
    while(done != 1){
        check = i/n;
        if(check >= 1){
            n = n*16;
            places++;
        }
        else{
            if(places == 0){
                n = n*16;
                places++;
            }
            done = 1;
        }

    }
    for(int k = 0;k<places;k++){
        n = n/16;
        a = i/n;
	if(a<10)
	{
        	output[k] = itoa(a);
	}else{
		output[k] = (char)(a + 55);
	}
        i = i-a*n;
    }
    sys_req(WRITE,COM1,output,sizeof(output));
    memset(&output,0,sizeof(output));

}
