// $ gcc -g -o oled2 oled2.c -Lbuild -lbone -Isrc/
// # LD_LIBRARY_PATH="build/" ./oled2
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "libbone.h"
#include "ssd1306.h"


int main(void)
{
    bone_ssd1306_t *display = bone_ssd1306_init(P8+3, I2C, 1, 0x3c, 128, 64);
    if (display == NULL) {
        perror("error initializing display");
        return 1;
    }
    bone_ssd1306_setup(display);

    /* program goes here */
    bone_ssd1306_clear(display, 0);
    bone_ssd1306_str(display, 0, 56, 1,
            "ABCDEFGHIJKLMNOPQRSTU\n"
            "VWXYZ 1234567890-=\n"
            "abcdefghijklmnopqrstu\n"
            "vwxyz !@#$%^&*()_+\n"
            "[];'`,./\\{}:\"~<>?|\n"
            "Hello, world! ?\b!x");
    bone_ssd1306_line(display, 126, 0, 126, 63, 1);
    bone_ssd1306_line(display, 127, 0, 127, 63, 1);
    bone_ssd1306_draw(display);
    sleep(1);

    bone_ssd1306_free(display);

    return 0;
}
