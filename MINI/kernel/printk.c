/* File: printk.c */
#include <kernel.h>
#include <stdarg.h>
#include <text.h>
#include <console.h>

int printk(const char * fmt, ...) {
    int string_length = 0;
    static char result[2048] = {0};
    char* string_tmp = NULL;
    va_list arg_ptr;

    va_start(arg_ptr, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            result[string_length++] = *(fmt++);
            continue;
        }

        ++fmt;

        switch (*fmt) {
        case 'd': case 'D': {
            string_tmp = to_Dec_string(va_arg(arg_ptr, int));
            while (*string_tmp) result[string_length++] = *(string_tmp++);
            break;
        }
        case 'x': case 'X': {
            string_tmp = to_Hex_string(va_arg(arg_ptr, int), false);
            while (*string_tmp) result[string_length++] = *(string_tmp++);
            break;
        }
        case 'p': {
            string_tmp = to_Hex_string(va_arg(arg_ptr, int), true);
            while (*string_tmp) result[string_length++] = *(string_tmp++);
            break;
        }
        case 'b': case 'B': {
            string_tmp = to_Bin_string(va_arg(arg_ptr, int));
            while (*string_tmp) result[string_length++] = *(string_tmp++);
            break;
        }
        case 'o': case 'O': {
            string_tmp = to_Oct_string(va_arg(arg_ptr, int));
            while (*string_tmp) result[string_length++] = *(string_tmp++);
            break;
        }
        case 'c': case 'C': {
            result[string_length++] = (char)va_arg(arg_ptr, int);
            break;
        }
        case 's': case 'S': {
            string_tmp = va_arg(arg_ptr, char*);
            while (*string_tmp) {
                result[string_length++] = *string_tmp++;
            }
            break;
        }
        case '%': {
            result[string_length++] = '%';
            break;
        }
        /* ...未完待续 */
        default: result[string_length++] = *fmt; break;
        }

        ++fmt;
    }
    va_end(arg_ptr);

    result[string_length] = '\0';
    console_out(result, 0, 0);

    return string_length;
}
