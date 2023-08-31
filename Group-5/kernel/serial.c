#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <string.h>
#include <stdlib.h>

enum uart_registers {
	RBR = 0,	// Receive Buffer
	THR = 0,	// Transmitter Holding
	DLL = 0,	// Divisor Latch LSB
	IER = 1,	// Interrupt Enable
	DLM = 1,	// Divisor Latch MSB
	IIR = 2,	// Interrupt Identification
	FCR = 2,	// FIFO Control
	LCR = 3,	// Line Control
	MCR = 4,	// Modem Control
	LSR = 5,	// Line Status
	MSR = 6,	// Modem Status
	SCR = 7,	// Scratch
};

static int initialized[4] = { 0 };

static int serial_devno(device dev)
{
	switch (dev) {
	case COM1: return 0;
	case COM2: return 1;
	case COM3: return 2;
	case COM4: return 3;
	}
	return -1;
}

int serial_init(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	outb(dev + IER, 0x00);	//disable interrupts
	outb(dev + LCR, 0x80);	//set line control register
	outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit
	outb(dev + DLM, 0x00);	//brd most significant bit
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
	outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte threshold
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
	(void)inb(dev);		//read bit to reset port
	initialized[dno] = 1;
	return 0;
}

int serial_out(device dev, const char *buffer, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	for (size_t i = 0; i < len; i++) {
		outb(dev, buffer[i]);
	}
	return (int)len;
}

int isAlphaNumeric(char c) {
    if ((c >= '0' & c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 1;
    return 0;
}

int serial_poll(device dev, char *buffer, size_t len) {
	int index = 0;
	int cursorIndex = index;
	while(index <= (int) len){
		if(inb(dev+LSR)&1){	
			char c = inb(dev);	
			if(c == 0x1B){ // arrows
				if(inb(dev) == 0x5B){ // [
				char arrow = inb(dev);
					if(arrow == 0x44){ // D Left arrow
						if (index > 0){
							cursorIndex--;
							outb(dev, '\b');
						}
					}
					else if(arrow == 0x43){ // C Right arrow
						if (cursorIndex < index){
							cursorIndex++;
							serial_out(dev, "\033[1C", strlen("\033[1C"));
							
						}
					}
				}
			}
			else if(c == 0x0D){	//Carriage Return Hex value
				c = 0X00;//Null
				outb(dev,c);
				buffer[index++] = '\0';
			
				c = 0X0A; //new line
				outb(dev,c);
				
				return 0;
			}

			else if(c == 0x08){//BackSpace Hex Value
				if(cursorIndex > 0){
				c = 0X08;
				outb(dev,c);
				buffer[index--] = '\0';
				
				c = 0x20;//Space Hex Value
				outb(dev,c);
				
				
				c = 0x08;
				outb(dev,c);
				cursorIndex--;
				}
			}
			else if (c == 0x7F){ // delete
				if (cursorIndex > 0){
				int deleteIndex = 0;
					for(int i = cursorIndex; i < index; i++){
						buffer[i] = buffer[i+1];
						c = buffer[i];
						outb(dev,c);
						cursorIndex++;
						deleteIndex++;
					}
					
					c = 0x7F;
					outb(dev,c);
					buffer[index--] = '\0';
					
					c = 0x08;
					outb(dev,c);
					c = 0x20;
					outb(dev,c);
					c = 0x08;
					outb(dev,c);	
					cursorIndex--;
					
					for(int j = 0; j < deleteIndex-1; j++){
						c = 0x08;
						outb(dev,c);
						cursorIndex--;
					}

				}	

			}

				
			else{
			buffer[index++]=c;
			outb(dev,c);
			cursorIndex++;
			}

		}
	}

	// insert your code to gather keyboard input via the technique of polling.
	// You must validate each key and handle special keys such as delete, back space, and
	// arrow keys

	// THIS MUST BE CHANGED TO RETURN THE CORRECT VALUE
	return index;
}
