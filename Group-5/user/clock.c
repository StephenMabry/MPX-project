#include <mpx/interrupts.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/disp.h>
#define RTC_INDEX_REG 0x70
#define RTC_DATA_REG 0x71
#define SECONDS_INDEX 0x00
#define MINUTES_INDEX 0x02
#define HOURS_INDEX 0x04
#define DAY_OF_MONTH_INDEX 0x07
#define MONTHS_INDEX 0x08
#define YEAR_INDEX 0x09

/**
 * @brief Gets the current time
 * 
 */
void getTime(){
    int seconds;
    outb(RTC_INDEX_REG, SECONDS_INDEX);
    char temp = inb(0x71);
    seconds = ((temp >> 4) & 0x0f ) *10 + (temp &0x0f);

    char *msg = "\e[38;2;0;255;0m\t";
    disp(msg);
    char *f = "0";

    int minutes;
    outb(RTC_INDEX_REG, MINUTES_INDEX);
    temp = inb(0x71);
    minutes = ((temp >>4) & 0x0f ) *10 + (temp &0x0f);

    int hours;
    outb(RTC_INDEX_REG, HOURS_INDEX);
    temp = inb(0x71);
    hours = ((temp >>4) & 0x0f) *10 + (temp &0x0f);
    
    if(hours < 10){
        disp(f);
    }
    disp_int(hours);
    msg = ":";
    disp(msg);

    if(minutes < 10){
        disp(f);
    }
    disp_int(minutes);
    msg = ":";
    disp(msg);

    if(seconds < 10){
        disp(f);
    }
    disp_int(seconds);
    msg = " UTC\n";
    disp(msg);
}

/**
 * @brief Gets the currentdate
 * 
 */
void getDate(){
    char *msg = "\e[38;2;0;255;0m\t";
    disp(msg);
	
    char *f = "0";
    int day;
    outb(RTC_INDEX_REG, DAY_OF_MONTH_INDEX);
    char temp = inb(0x71);
    day = ((temp >> 4) &0x0f ) *10 + (temp &0x0f);
    if(day < 10){
        disp(f);
    }
    disp_int(day);
    msg = "/";
    disp(msg);

    int month;
    outb(RTC_INDEX_REG, MONTHS_INDEX);
    temp = inb(0x71);
    month = ((temp >> 4) &0x0f ) *10 + (temp &0x0f);
    if(month < 10){
        disp(f);
    }
    disp_int(month);
    msg = "/";
    disp(msg);

    int year;
    outb(RTC_INDEX_REG, YEAR_INDEX);
    temp = inb(0x71);
    year = ((temp >> 4) &0x0f ) *10 + (temp &0x0f);
    if(year < 10){
        disp(f);
    }
    disp_int(year);
    msg = "\n";
    disp(msg);
}

/**
 * @brief Sets the time
 * 
 */
void setTime(){
    char *msg = "    Enter new time following format \"hh:mm:ss\"\n    ";
    disp(msg);
    int seconds;
    int minutes;
    int hours;
	
    char input[100] = {0};
    int nread = sys_req(READ, COM1, input, sizeof(input));
    sys_req(WRITE, COM1, input, nread);

    //Verifies proper format
    if((input[2]!=':')||(input[5]!=':')){
		msg = "\e[38;2;255;0;0m    Input error: Separators (':') missing or not in right place\n";
		disp(msg);
		return;
    }

    hours = atoi(&input[0]);
    minutes = atoi(&input[3]);
    seconds = atoi(&input[6]);

    if((seconds<0)||(seconds>59)){
	msg = "\e[38;2;255;0;0m    Input error: Seconds out of range 0-59\n";
        disp(msg);
        return;
    }
    if((minutes<0)||(minutes>59)){
        msg = "\e[38;2;255;0;0m    Input error: Minutes out of range 0-59\n";
        disp(msg);
        return;
    }
    if((hours<0)||(hours>23)){
        msg = "\e[38;2;255;0;0m    Input error: Hours out of range 0-23\n";
        disp(msg);
        return;
    }

    sti();

    //Setting up for writing to clock and converting to BCD
    outb(RTC_INDEX_REG  , SECONDS_INDEX);
    outb(RTC_DATA_REG, ((seconds/10)<<4) | (seconds % 10));

    outb(RTC_INDEX_REG  , MINUTES_INDEX);
    outb(RTC_DATA_REG, ((minutes/10)<<4) | (minutes % 10));

    outb(RTC_INDEX_REG  , HOURS_INDEX);

    outb(RTC_DATA_REG, ((hours/10)<<4) | (hours % 10));

    cli();

    msg = "\e[38;2;0;255;0m    New time set to: ";
    disp(msg);
    getTime();

}

/**
 * @brief Sets the date
 * 
 */
void setDate(){
    char *msg = "    Enter new date following the format \"dd-mm-yy\"\n    ";
    disp(msg);
    int day;
    int month;
    int year;
    int leap = 0;
    int maxDays = 0;

    char input[100] = {0};
    int nread = sys_req(READ, COM1, input, sizeof(input));
    sys_req(WRITE, COM1, input, nread);

    //Verifies proper format
    if((input[2]!='-')||(input[5]!='-')){
        msg = "   \e[38;2;255;0;0m Input error: Separators ('-') missing or not in right place\n";
        disp(msg);
        return;
    }

    day = atoi(&input[0]);
    month = atoi(&input[3]);
    year = atoi(&input[6]);
    
    if((year<0)||(year>99)){
	    msg = "   \e[38;2;255;0;0m Input error: Year out of range 0-99\n";
	    return;
    }

    //Leap year check
    if(year%4 == 0){
        leap = 1;
    }

    if((month<1)||(month>12)){
        msg = "   \e[38;2;255;0;0m Input error: Month out of range 1-12\n";
        return;
    }

    if(month == 2){
        if(leap != 0){
            maxDays = 29;
        }else{
            maxDays = 28;
        }
    }else if((month == 4)||(month == 6)||(month == 9)||(month == 11)){
	    maxDays = 30;
    }else{
	    maxDays = 31;
    }
    
    if((day<1)||(day>maxDays)){
        msg = "   \e[38;2;255;0;0m Input error: Day out of range 1-";
	    disp(msg);
	    disp_int(maxDays);
	    msg = "\n";
	    disp(msg);
        return;
    }

    sti();

    //Setting up for writing to clock and converting to BCD
    outb(RTC_INDEX_REG  , YEAR_INDEX);
    outb(RTC_DATA_REG, (((year / 10)<<4) | (year % 10)));

    outb(RTC_INDEX_REG  , MONTHS_INDEX);
    outb(RTC_DATA_REG, ((month/10)<<4) | (month % 10));

    outb(RTC_INDEX_REG  , DAY_OF_MONTH_INDEX);
    outb(RTC_DATA_REG, ((day/10)<<4) | (day % 10));

    cli();
    msg = "\e[38;2;0;255;0m    New date set to: ";
    disp(msg);
    getDate();
}
