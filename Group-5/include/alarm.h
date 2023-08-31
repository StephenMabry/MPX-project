#ifndef MPX_ALARM_H
#define MPX_ALARM_H
#include <pcb.h>
#include <processes.h>
#include <mpx/disp.h>
#define RTC_INDEX_REG 0x70
#define RTC_DATA_REG 0x71
#define SECONDS_INDEX 0x00
#define MINUTES_INDEX 0x02
#define HOURS_INDEX 0x04


typedef struct time{
	int hh;
	int mm;
	int ss;
}time;

void setAlarm(time *atm,char* mess);
void loadAlarm();
void alarm();
#endif
