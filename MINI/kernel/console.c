// kernel/console.c
#include <vbe.h>
#include <console.h>

static uint32_t console_color = 0x44883cff, console_background = 0x1e2229ff;
static uint16_t console_x = 0, console_y = 0, console_fixed_height;

void init_console(void) {
    int i = 0, j = 0;
    uint8_t background[3] = {
        Red(console_background),
        Green(console_background),
        Blue(console_background)
    };

    console_fixed_height = (ScreenHeight / 16) * 16;

    for (; i < ScreenHeight; ++i) {
        for (j = 0; j < ScreenWidth; ++j) {
            putPixel(j, i, background[0], background[1], background[2]);
        }
    }

    setColor(console_color, console_background);

    return;
}

void console_out(const char *string, uint32_t color, uint32_t background) {
    if (color ^ background) {
        setColor(color, background);
    }
    while (*string) {
        switch (*string) {
        case '\n':
            console_roll();
        break;
        case '\t':
            /* console_x += (4 - (console_x / 8) % 4) * 8; */
            console_x += (4 - (console_x >> 3) % 4) << 3;
            if (console_x >= ScreenWidth) {
                console_roll();
            }
        break;
        case '\b':
            if (!console_x) {
                console_x -= 8;
            }
        break;
        case '\r':
            console_x = 0;
        break;
        default:
            putChar(*string, console_x, console_y);
            console_x += 8;
            if (console_x >= ScreenWidth) {
                console_roll();
            }
        break;
        }
        string++;
    }

    return;
}

void console_roll(void) {
    console_x = 0, console_y += 16;
    if (console_y >= console_fixed_height) {
        console_y -= 16;
        int i = 16, j = 0, k = 0;

        for (; i < ScreenHeight; ++i) {
            for (j = 0, k = i-16; j < ScreenWidth; ++j) {
                sendPixel(j, i, j, k);
            }
        }
    }
    return;
}

void console_cls(void) {
    init_console();
    console_x = console_y = 0;
    return;
}
