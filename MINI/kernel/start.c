/* File: start.c */

#include <vbe.h>
#include <console.h>
#include <kernel.h>
#include <interrupt.h>
#include <timer.h>

unsigned char port_byte_in(unsigned short port) {
    // A handy C wrapper function that reads a byte from the specified port
    // "=a" ( result ) means : put AL register in variable RESULT when finished
    // "d" ( port ) means : load EDX with port
    unsigned char result;
    __asm__ ("in %%dx, %%al"
             :"=a"(result)
             :"d"(port)
             );
    return result;
}

void show_logo() {
    /*
     * @@@@@@..@@@@@@...@@@@...@@@@@@...@@@@....@@@@.
     * ..@@....@@......@@........@@....@@..@@..@@....
     * ..@@....@@@@.....@@@@.....@@....@@..@@...@@@@.
     * ..@@....@@..........@@....@@....@@..@@......@@
     * ..@@....@@@@@@...@@@@.....@@.....@@@@....@@@@.
     */
    const char *logo = "626343634441-224262824222224-224454524222341-2242|10|242422262-2246345254441";
    int i;
    char print_char[4] = {'@', '\0', '@', ' '};
    while (*logo) {
        i = 0;
        if (*logo >= '0' && *logo <= '9') {
            i = *logo - '0';
        } else if (*logo == '+') {
            print_char[0] = print_char[2];
            console_out("\n", 0, 0);
            goto END;
        } else if (*logo == '-') {
            print_char[0] = print_char[3];
            console_out("\n", 0, 0);
            goto END;
        } else {
            ++logo;
            while (*logo != '|') {
                i *= 10;
                i += *logo - '0';
                ++logo;
            }
        }
        while (i--) {
            console_out(print_char, 0, 0);
        }
        print_char[0] = print_char[0] == print_char[2] ? print_char[3] : print_char[2];
    END:
        ++logo;
    }

    console_out("\n\nTESTOS [Version 0.0.1]\n(C) 2020 GuEe Studio. GPL-3.0 License.\n\n", 0, 0);

    return;
}

int os_main(void) {
    init_VBE();
    init_console();
    init_8259A();
    init_IDT();

    show_logo();


    init_RTC();
    // init_Timer(200);

    asm volatile ("int $0x1");
    asm volatile ("sti");

    /* 进入死循环 */
    while(true) {
        read_rtc();
        printk("%d/%d/%d %d:%d:%d\n", OS_RTC_Time.year, OS_RTC_Time.month, OS_RTC_Time.day, OS_RTC_Time.hour + 8, OS_RTC_Time.minute, (int)OS_RTC_Time.second);
    }

    return 0;
}
