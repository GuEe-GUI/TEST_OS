// ​include/timer.h
#ifndef __TIMER__
#define __TIMER__

#include <types.h>
#include <interrupt.h>

void timer_callback(Registers_S *regs);
void init_Timer(uint32_t frequency);

struct rtc_time {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned int year;
};

extern struct rtc_time OS_RTC_Time;
void init_RTC();
int get_update_in_progress_flag();
unsigned char get_RTC_register(int reg);
void read_rtc();

#endif
