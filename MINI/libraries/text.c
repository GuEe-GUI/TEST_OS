#include <text.h>

char* to_Dec_string(int decimal_number) {
    char result_tmp[32];
    static char result[34];
    int i = 0, j = 0;

    if (decimal_number < 0) result[i++] = '-';

    while (decimal_number > 0) {
        *(result_tmp + j) = '0' + (decimal_number % 10);
        ++j;
        decimal_number /= 10;
    }

    for (--j; j >= 0; result[i++] = result_tmp[j--]);

    result[i] = '\0';

    return result;
}

char* to_Oct_string(int decimal_number) {
    if (decimal_number < 0) return "Error: This number is smaller than 0";

    char result_tmp[32];
    static char result[34];
    int i = 0, j = 0;

    result[i++] = '0';

    while (decimal_number > 0) {
        *(result_tmp + j) = '0' + (decimal_number % 8);
        ++j;
        decimal_number >>= 3;
    }

    for (--j; j >= 0; result[i++] = result_tmp[j--]);

    return result;
}

char* to_Hex_string(int decimal_number, bool_t sign) {
    if (decimal_number < 0) return "Error: This number is small than 0";
    char result_tmp[32];
    static char result[35];
    const char Hex_char[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int i = 0, j = 0;

    if (sign == true) {
        result[i++] = '0';
        result[i++] = 'x';
    }

    while (decimal_number > 0) {
        *(result_tmp + j) = Hex_char[decimal_number % 16];
        ++j;
        decimal_number >>= 4;
    }

    for (--j; j >= 0; result[i++] = result_tmp[j--]);

    result[i] = '\0';

    return result;
}

char* to_Bin_string(int decimal_number) {
    if (decimal_number < 0) return "The parameter is error!";

    char result_tmp[32];
    static char result[34];
    int i = 0, j = 0;

    while (decimal_number > 0) {
        *(result_tmp + j) = (decimal_number % 2) ? '1' : '0';
        ++j;
        decimal_number >>= 1;
    }

    for (--j; j >= 0; result[i++] = result_tmp[j--]);

    result[i++] = 'b';
    result[i]   = '\0';

    return result;
}
