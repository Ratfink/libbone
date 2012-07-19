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

#ifndef __BONE_SSD1306_H__
#define __BONE_SSD1306_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "gpio.h"
#include "font.h"


typedef struct bone_ssd1306 {
    int rst; /* Pn+pin */
    int spi;
    int cs;
    int fd;
    uint16_t x;
    uint16_t y;
    uint8_t *buf; /* x*y/8 bytes long */
} bone_ssd1306_t;

/* Setup functions */
bone_ssd1306_t *bone_ssd1306_init(int rst, int spi, int cs, int x, int y);
void bone_ssd1306_free(bone_ssd1306_t *disp);
int bone_ssd1306_setup(bone_ssd1306_t *disp);
/* Low-ish level stuff */
void bone_ssd1306_draw(bone_ssd1306_t *disp);
int bone_ssd1306_cmd(bone_ssd1306_t *disp, uint8_t cmd);
int bone_ssd1306_dat(bone_ssd1306_t *disp, uint8_t dat);
/* Graphics functions */
void bone_ssd1306_clear(bone_ssd1306_t *disp, bool color);
int bone_ssd1306_point(bone_ssd1306_t *disp, uint16_t x, uint16_t y,
                       bool color);
int bone_ssd1306_line(bone_ssd1306_t *disp, uint16_t x0, uint16_t y0,
                       uint16_t x1, uint16_t y1, bool color);
void bone_ssd1306_circle(bone_ssd1306_t *disp, int x0, int y0, int radius,
                         bool color);
/* Text functions */
int bone_ssd1306_char(bone_ssd1306_t *disp, uint16_t x, uint16_t y, bool color,
                      char c);
int bone_ssd1306_str(bone_ssd1306_t *disp, uint16_t x, uint16_t y, bool color,
                     char *s);


#endif /* __BONE_SSD1306_H__ */
