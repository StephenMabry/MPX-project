#include <processes.h>
#include <mpx/com_handler.h>
#include <stddef.h>
#include <pcb.h>
#include <stdlib.h>
#include <mpx/interrupts.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/disp.h>
#include <alarm.h>
#define RTC_INDEX_REG 0x70
#define RTC_DATA_REG 0x71
#define SECONDS_INDEX 0x00
#define MINUTES_INDEX 0x02
#define HOURS_INDEX 0x04



void displayAlarm(time* atm, int hour, int minute, int second);
void setAlarm(time *atm,char* mess){

        int hours;
        int minutes;
        int seconds;
        char temp = inb(0x71);
	outb(RTC_INDEX_REG, HOURS_INDEX);
	temp = inb(0x71);
	int setHour = ((temp >> 4) & 0x0f) * 10 + (temp &0x0f);
	outb(RTC_INDEX_REG, MINUTES_INDEX);
	temp = inb(0x71);
	int setMinute = ((temp >> 4) & 0x0f) * 10 + (temp &0x0f);
	outb(RTC_INDEX_REG, SECONDS_INDEX);
	temp = inb(0x71);
	int setSecond = ((temp >> 4) & 0x0f) * 10 + (temp &0x0f);

	displayAlarm(atm, setHour, setMinute, setSecond);
	
	int day = 0;
	if(setHour > atm->hh || (setHour == atm->hh && setMinute > atm->mm) || (setHour == atm->hh && setMinute == atm->mm && setSecond >= atm->ss)){
		day = 1;
	}

        for(;;){
		int flag = 0;
		
                outb(RTC_INDEX_REG, SECONDS_INDEX);
		temp = inb(0x71);
                seconds = ((temp >> 4) & 0x0f ) *10 + (temp &0x0f);

                outb(RTC_INDEX_REG, MINUTES_INDEX);
                temp = inb(0x71);
                minutes = ((temp >>4) & 0x0f ) *10 + (temp &0x0f);

                outb(RTC_INDEX_REG, HOURS_INDEX);
                temp = inb(0x71);
                hours = ((temp >>4) & 0x0f) *10 + (temp &0x0f);

		if(hours < atm->hh || (hours == atm->hh && minutes < atm->mm) || (hours == atm->hh && minutes == atm->mm && seconds <= atm->ss)){
			day = 0;
		}

		if(hours > atm->hh){
			flag = 1;	
		}
		else if(hours == atm->hh && minutes > atm->mm){
			flag = 1;
		}
		else if ((hours == atm->hh)&&(minutes == atm->mm)&&(seconds >= atm->ss)){
			flag = 1;
                }
		if (flag == 1 && day == 0){ 
			disp(mess);
			disp("\n");
			break;
		}
		if (quit == 1){
			break;
		}
                sys_req(IDLE);
        }
        sys_req(EXIT);
}

void displayAlarm(time* atm, int hour, int minute, int second){
	disp("\nAlarm set for ");
	if(atm->hh<10){
		disp_int(0);
	}
	disp_int(atm->hh);
	disp(":");
	if(atm->mm<10){
                disp_int(0);
        }
	disp_int(atm->mm);
	disp(":");
	if(atm->ss<10){
                disp_int(0);
        }
	disp_int(atm->ss);

	disp("\nSet at ");
	if(hour<10){
                disp_int(0);
        }
	disp_int(hour);
	disp(":");
	if(minute<10){
                disp_int(0);
        }
	disp_int(minute);
	disp(":");
	if(second<10){
                disp_int(0);
        }
	disp_int(second);
	disp("\n\n");
}

void loadAlarm(){
	int count = 0;
	char name[] = "alarm_X";
	for(;;){
		name[6] = count + '0';
		if(pcb_find(name) != NULL){
			count++;
			continue;
		}
		break;
	}
        
	pcb* alarmPCB = pcb_setup(name,0,0);
        context* ctx = (context*) alarmPCB->stackPtr;

        ctx->eax = 0x00;
        ctx->ebx = 0x00;
        ctx->ecx = 0x00;
        ctx->edx = 0x00;
        ctx->esi = 0x00;
        ctx->edi = 0x00;
        ctx->ebp = (uint32_t)alarmPCB->stackPtr;
        ctx->esp = (uint32_t)alarmPCB->stackPtr;
        ctx->eip = (uint32_t)alarm;
        ctx->cs = 0x08;
        ctx->ds = 0x10;
        ctx->es = 0x10;
        ctx->fs = 0x10;
        ctx->gs = 0x10;
        ctx->ss = 0x10;
        ctx->eflags = 0x0202;
        pcb_insert(alarmPCB);
	sys_req(IDLE);
}

void alarm(){
	
	char *prompt = "\tEnter alarm time following format \"hh:mm:ss\"\n\t";
	char *error = "";
	int seconds;
	int minutes;
	int hours;
	int errorFlag = 0;

	disp(prompt);
	char atm[100] = {0};
	int nread = sys_req(READ, COM1, atm, sizeof(atm));
	sys_req(WRITE, COM1, atm, nread);


	//Verifies proper format
	if((atm[2]!=':')||(atm[5]!=':')){
		error = "\e[38;2;255;0;0m    Input error: Separators (':') missing or not in right place\n";
		disp(error);
		errorFlag = 1;
	}

	hours = atoi(&atm[0]);
	minutes = atoi(&atm[3]);
	seconds = atoi(&atm[6]);

	if((seconds<0)||(seconds>59)){
		error = "\e[38;2;255;0;0m    Input error: Seconds out of range 0-59\n";
		disp(error);
		errorFlag = 1;
	}
	if((minutes<0)||(minutes>59)){
		error = "\e[38;2;255;0;0m    Input error: Minutes out of range 0-59\n";
		disp(error);
		errorFlag = 1;
	}
	if((hours<0)||(hours>23)){
		error = "\e[38;2;255;0;0m    Input error: Hours out of range 0-23\n";
		disp(error);
		errorFlag = 1;
	}
	if(errorFlag == 0){	
		prompt = "\tEnter a message to display\n\t";
		disp(prompt);
		char msg[100] = {0};
	        nread = sys_req(READ, COM1, msg, sizeof(msg));
        	sys_req(WRITE, COM1, msg, nread);

		time alarm_time;
		alarm_time.hh = hours;
		alarm_time.mm = minutes;
		alarm_time.ss = seconds;

		setAlarm(&alarm_time,msg);
	}else{
		sys_req(EXIT);
	}
}

