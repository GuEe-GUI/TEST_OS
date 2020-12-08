#include <timer.h>

struct rtc_time OS_RTC_Time;

void init_RTC() {
    cli();                      // 关中断

    io_out8(0x70, 0x8A);        // 选择状态寄存器A，并禁用NMI（通过将0x80位置1）
    io_out8(0x71, 0x20);        // 写入CMOS / RTC RAM

    io_out8(0x70, 0x8B);        // 选择寄存器B，并禁用NMI
    char prev = io_in8(0x71);   // 读取寄存器B的当前值
    io_out8(0x70, 0x8B);        // 再次设置索引（读取将重置索引以进行注册
    io_out8(0x71, prev | 0x40); // 写入先前的值与0x40进行“或”运算。 这将打开第6位

    sti();                      // 开中断
}

#define CURRENT_YEAR 2020
#define century_register 0x00

int get_update_in_progress_flag() {
    io_out8(0x70, 0x0A);
    return (io_in8(0x71) & 0x80);
}

unsigned char get_RTC_register(int reg) {
    io_out8(0x70, reg);
    return io_in8(0x71);
}

void read_rtc() {
    unsigned char century = 20;
    unsigned char last_second;
    unsigned char last_minute;
    unsigned char last_hour;
    unsigned char last_day;
    unsigned char last_month;
    unsigned char last_year;
    unsigned char last_century;
    unsigned char registerB;

    while (get_update_in_progress_flag());
    OS_RTC_Time.second = get_RTC_register(0x00);
    OS_RTC_Time.minute = get_RTC_register(0x02);
    OS_RTC_Time.hour = get_RTC_register(0x04);
    OS_RTC_Time.day = get_RTC_register(0x07);
    OS_RTC_Time.month = get_RTC_register(0x08);
    OS_RTC_Time.year = get_RTC_register(0x09);
    if(century_register != 0) {
        century = get_RTC_register(century_register);
    }

    do {
        last_second = OS_RTC_Time.second;
        last_minute = OS_RTC_Time.minute;
        last_hour = OS_RTC_Time.hour;
        last_day = OS_RTC_Time.day;
        last_month = OS_RTC_Time.month;
        last_year = OS_RTC_Time.year;
        last_century = century;

        while (get_update_in_progress_flag());
        OS_RTC_Time.second = get_RTC_register(0x00);
        OS_RTC_Time.minute = get_RTC_register(0x02);
        OS_RTC_Time.hour = get_RTC_register(0x04);
        OS_RTC_Time.day = get_RTC_register(0x07);
        OS_RTC_Time.month = get_RTC_register(0x08);
        OS_RTC_Time.year = get_RTC_register(0x09);
        if(century_register != 0) {
            century = get_RTC_register(century_register);
        }
    } while( (last_second != OS_RTC_Time.second) || (last_minute != OS_RTC_Time.minute) || (last_hour != OS_RTC_Time.hour) ||
             (last_day != OS_RTC_Time.day) || (last_month != OS_RTC_Time.month) || (last_year != OS_RTC_Time.year) ||
             (last_century != century) );

    registerB = get_RTC_register(0x0B);

    if (!(registerB & 0x04)) {
        OS_RTC_Time.second = (OS_RTC_Time.second & 0x0F) + ((OS_RTC_Time.second / 16) * 10);
        OS_RTC_Time.minute = (OS_RTC_Time.minute & 0x0F) + ((OS_RTC_Time.minute / 16) * 10);
        OS_RTC_Time.hour = ( (OS_RTC_Time.hour & 0x0F) + (((OS_RTC_Time.hour & 0x70) / 16) * 10) ) | (OS_RTC_Time.hour & 0x80);
        OS_RTC_Time.day = (OS_RTC_Time.day & 0x0F) + ((OS_RTC_Time.day / 16) * 10);
        OS_RTC_Time.month = (OS_RTC_Time.month & 0x0F) + ((OS_RTC_Time.month / 16) * 10);
        OS_RTC_Time.year = (OS_RTC_Time.year & 0x0F) + ((OS_RTC_Time.year / 16) * 10);
        if(century_register != 0) {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }

    if (!(registerB & 0x02) && (OS_RTC_Time.hour & 0x80)) {
        OS_RTC_Time.hour = ((OS_RTC_Time.hour & 0x7F) + 12) % 24;
    }

    if(century_register != 0) {
        OS_RTC_Time.year += century * 100;
    } else {
        OS_RTC_Time.year += (CURRENT_YEAR / 100) * 100;
        if(OS_RTC_Time.year < CURRENT_YEAR) OS_RTC_Time.year += 100;
    }
}
