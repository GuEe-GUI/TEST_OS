/* File: console.h */
#ifndef _CONSOLE_H_
#define _CONSOLE_H_

void init_console(void);
void console_out(const char *string, uint32_t color, uint32_t background);
void console_roll(void);
void console_cls(void);

#endif
