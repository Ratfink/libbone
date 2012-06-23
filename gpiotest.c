// $ gcc -g -o gpiotest gpiotest.c -Lbuild -lbone -Isrc/
#include <stdio.h>
#include <unistd.h>
#include "libbone.h"

int main(void)
{
    int i, j;

    bone_gpio_export(P8+3);
    i = bone_gpio_get_dir(P8+3);
    bone_gpio_set_dir(P8+3, 1); /* Output */
    for (j = 0; j < 10; j++) {
        i = bone_gpio_get_value(P8+3); /* See what it is right now */
        if (i == 0) {
            bone_gpio_set_value(P8+3, 1);
        } else if (i == 1) {
            bone_gpio_set_value(P8+3, 0);
        }
        usleep(500000);
    }

    bone_gpio_unexport(P8+3);
    return 0;
}
