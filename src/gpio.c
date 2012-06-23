/* libbone - A library for working with the BeagleBone in C
 * Copyright (c) 2012 Clayton G. Hobbs
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "gpio.h"


int bone_gpio_pins[] = {
    -1,  /* P8 */
    -1,  /* GND */
    -1,  /* GND */
    38,  /* GPIO1_6 */
    39,  /* GPIO1_7 */
    34,  /* GPIO1_2 */
    35,  /* GPIO1_3 */
    66,  /* TIMER4 */
    67,  /* TIMER7 */
    69,  /* TIMER5 */
    68,  /* TIMER6 */
    45,  /* GPIO1_13 */
    44,  /* GPIO1_12 */
    23,  /* EHRPWM2B */
    26,  /* GPIO0_26 */
    47,  /* GPIO1_15 */
    46,  /* GPIO1_14 */
    27,  /* GPIO0_27 */
    65,  /* GPIO2_1 */
    22,  /* EHRPWM2A */
    63,  /* GPIO1_31 */
    62,  /* GPIO1_30 */
    37,  /* GPIO1_5 */
    36,  /* GPIO1_4 */
    33,  /* GPIO1_1 */
    32,  /* GPIO1_0 */
    61,  /* GPIO1_29 */
    54,  /* GPIO1_22 */
    56,  /* GPIO1_24 */
    55,  /* GPIO1_23 */
    57,  /* GPIO1_25 */
    10,  /* UART5_CTSN */
    11,  /* UART5_RTSN */
    9,   /* UART4_RTSN */
    81,  /* UART3_RTSN */
    8,   /* UART4_CTSN */
    80,  /* UART3_CTSN */
    78,  /* UART5_TXD */
    79,  /* UART5_RXD */
    76,  /* GPIO2_12 */
    77,  /* GPIO2_13 */
    74,  /* GPIO2_10 */
    75,  /* GPIO2_11 */
    72,  /* GPIO2_8 */
    73,  /* GPIO2_9 */
    70,  /* GPIO2_6 */
    71,  /* GPIO2_7 */
    -1,  /* n/a */
    -1,  /* n/a */
    -1,  /* n/a */
    -1,  /* P9 */
    -1,  /* GND */
    -1,  /* GND */
    -1,  /* DC_3.3V */
    -1,  /* DC_3.3V */
    -1,  /* VDD_5V */
    -1,  /* VDD_5V */
    -1,  /* SYS_5V */
    -1,  /* SYS_5V */
    -1,  /* PWR_BUT */
    -1,  /* SYS_RESETn */
    30,  /* UART4_RXD */
    60,  /* GPIO1_28 */
    31,  /* UART4_TXD */
    50,  /* EHRPWM1A */
    48,  /* GPIO1_16 */
    51,  /* EHRPWM1B */
    5,   /* I2C1_SCL */
    4,   /* I2C1_SDA */
    13,  /* I2C2_SCL */
    12,  /* I2C2_SDA */
    3,   /* UART2_TXD */
    2,   /* UART2_RXD */
    49,  /* GPIO1_17 */
    15,  /* UART1_TXD */
    117, /* GPIO3_21 */
    14,  /* UART1_RXD */
    115, /* GPIO3_19 */
    113, /* SPI1_CS0 */
    111, /* SPI1_D0 */
    112, /* SPI1_D1 */
    110, /* SPI1_SCLK */
    -1,  /* VDD_ADC (1.8V) */
    -1,  /* AIN4 */
    -1,  /* GNDA_ADC */
    -1,  /* AIN6 */
    -1,  /* AIN5 */
    -1,  /* AIN2 */
    -1,  /* AIN3 */
    -1,  /* AIN0 */
    -1,  /* AIN1 */
    20,  /* CLKOUT2 */
    7,   /* GPIO0_7 */
    -1,  /* GND */
    -1,  /* GND */
    -1,  /* GND */
    -1   /* GND */
};


int bone_gpio_export(int pnpin)
{
    int export;
    char gpio[4];

    snprintf(gpio, 4, "%i", bone_gpio_pins[pnpin]);
    if ((export = open("/sys/class/gpio/export", O_WRONLY, 0)) == -1)
        return -1;
    if (write(export, gpio, 3) == -1)
        return -1;
    if (close(export) == -1)
        return -1;

    return 0;
}


int bone_gpio_unexport(int pnpin)
{
    int unexport;
    char gpio[4];

    snprintf(gpio, 4, "%i", bone_gpio_pins[pnpin]);
    if ((unexport = open("/sys/class/gpio/unexport", O_WRONLY, 0)) == -1)
        return -1;
    if (write(unexport, gpio, 3) == -1)
        return -1;
    if (close(unexport) == -1)
        return -1;

    return 0;
}


int bone_gpio_get_value(int pnpin)
{
    int valfd;
    char filename[30];
    char value;

    snprintf(filename, 30, "/sys/class/gpio/gpio%i/value",
             bone_gpio_pins[pnpin]);
    if ((valfd = open(filename, O_RDONLY, 0)) == -1)
        return -1;
    if (read(valfd, &value, 1) == -1)
        return -1;
    if (close(valfd) == -1)
        return -1;

    return value - '0';
}


int bone_gpio_set_value(int pnpin, int value)
{
    int valfd;
    char filename[30];

    snprintf(filename, 30, "/sys/class/gpio/gpio%i/value",
             bone_gpio_pins[pnpin]);
    value = value + '0';
    if ((valfd = open(filename, O_RDONLY, 0)) == -1)
        return -1;
    if (write(valfd, &value, 1) == -1)
        return -1;
    if (close(valfd) == -1)
        return -1;

    return 0;
}


int bone_gpio_get_dir(int pnpin)
{
    int dirfd;
    char filename[34];
    char dir;

    snprintf(filename, 34, "/sys/class/gpio/gpio%i/direction",
             bone_gpio_pins[pnpin]);
    if ((dirfd = open(filename, O_RDONLY, 0)) == -1)
        return -1;
    if (read(dirfd, &dir, 1) == -1)
        return -1;
    if (close(dirfd) == -1)
        return -1;

    if (dir == 'i') {
        return 0;
    } else if (dir == 'o') {
        return 1;
    } else {
        return -2; /* This is an odd situation */
    }
}


int bone_gpio_set_dir(int pnpin, int dir)
{
    int dirfd;
    char filename[34];

    snprintf(filename, 34, "/sys/class/gpio/gpio%i/value",
             bone_gpio_pins[pnpin]);
    if ((dirfd = open(filename, O_RDONLY, 0)) == -1)
        return -1;
    if (dir == 0) {
        if (write(dirfd, "in", 2) == -1)
            return -1;
    } else if (dir == 1) {
        if (write(dirfd, "out", 3) == -1)
            return -1;
    } else {
        return -2; /* Only 0 and 1 are allowed directions */
    }
    if (close(dirfd) == -1)
        return -1;

    return 0;
}
