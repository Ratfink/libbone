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

#include "ssd1306.h"


bone_ssd1306_t *bone_ssd1306_init(int rst, int spi, int cs, int x, int y)
{
    bone_ssd1306_t *disp = malloc(sizeof (bone_ssd1306_t));
    char dev[20];

    if (disp == NULL) {
        return NULL;
    }
    disp->rst = rst;
    disp->spi = spi;
    disp->cs = cs;
    snprintf(dev, 20, "/dev/spidev%i.%i", spi, cs);
    if ((disp->fd = open(dev, O_RDWR)) == -1) {
        return NULL;
    }
    disp->x = x;
    disp->y = y;
    disp->buf = malloc(x * y / 8); /* x*y bits */
    if (disp->buf == NULL) {
        return NULL;
    }
    return disp;
}


void bone_ssd1306_free(bone_ssd1306_t *disp)
{
    /* Shut down first */
    bone_ssd1306_cmd(disp, 0xAE);
    bone_gpio_unexport(disp->rst);
    close(disp->fd); /* If this can actually fail I'll eat my hat */
    free(disp->buf);
    free(disp);
}


int bone_ssd1306_setup(bone_ssd1306_t *disp)
{
    bone_gpio_export(disp->rst);
    bone_gpio_set_dir(disp->rst, 1);
    bone_gpio_set_value(disp->rst, 0);
    usleep(3000);
    bone_gpio_set_value(disp->rst, 1);

    bone_ssd1306_cmd(disp, 0xAE);//--turn off oled panel
    bone_ssd1306_cmd(disp, 0x40);//--set start line address
    bone_ssd1306_cmd(disp, 0x20);//--set memory addressing mode
    bone_ssd1306_cmd(disp, 0x00);//---horizontal addressing mode
    bone_ssd1306_cmd(disp, 0x81);//--set contrast control register
    bone_ssd1306_cmd(disp, 0xCF);
    bone_ssd1306_cmd(disp, 0xA1);//--set segment re-map 95 to 0
    bone_ssd1306_cmd(disp, 0xA6);//--set normal display
    bone_ssd1306_cmd(disp, 0xA8);//--set multiplex ratio(1 to 64)
    bone_ssd1306_cmd(disp, 0x3F);//--1/64 duty
    bone_ssd1306_cmd(disp, 0xD3);//-set display offset
    bone_ssd1306_cmd(disp, 0x00);//-not offset
    bone_ssd1306_cmd(disp, 0xD5);//--set display clock divide ratio/oscillator frequency
    bone_ssd1306_cmd(disp, 0xF0);//--set divide ratio
    bone_ssd1306_cmd(disp, 0xD9);//--set pre-charge period
    bone_ssd1306_cmd(disp, 0xF1);
    bone_ssd1306_cmd(disp, 0xDA);//--set com pins hardware configuration
    bone_ssd1306_cmd(disp, 0x12);
    bone_ssd1306_cmd(disp, 0xDB);//--set vcomh
    bone_ssd1306_cmd(disp, 0x40);
    bone_ssd1306_cmd(disp, 0x8D);//--set Charge Pump enable/disable
    bone_ssd1306_cmd(disp, 0x14);//--set(0x10) disable
    bone_ssd1306_cmd(disp, 0xAF);//--turn on oled panel

    return 0;
}


void bone_ssd1306_draw(bone_ssd1306_t *disp)
{
    for (int i = 0; i < disp->x * disp->y / 8; i++) {
        bone_ssd1306_dat(disp, disp->buf[i]);
    }
}


int bone_ssd1306_cmd(bone_ssd1306_t *disp, uint8_t cmd)
{
    uint16_t cmd9 = cmd & ~0x100;
    struct spi_ioc_transfer trans = {
        .tx_buf = (unsigned long) &cmd9,
        .rx_buf = (unsigned long) NULL,
        .len = 2,
        .delay_usecs = 0,
        .speed_hz = 10000000,
        .bits_per_word = 9
    };

    if (ioctl(disp->fd, SPI_IOC_MESSAGE(1), &trans) == -1) {
        return -1;
    }
    return 0;
}


int bone_ssd1306_dat(bone_ssd1306_t *disp, uint8_t dat)
{
    uint16_t dat9 = dat | 0x100;
    struct spi_ioc_transfer trans = {
        .tx_buf = (unsigned long) &dat9,
        .rx_buf = (unsigned long) NULL,
        .len = 2,
        .delay_usecs = 0,
        .speed_hz = 10000000,
        .bits_per_word = 9
    };

    if (ioctl(disp->fd, SPI_IOC_MESSAGE(1), &trans) == -1) {
        return -1;
    }
    return 0;
}


void bone_ssd1306_clear(bone_ssd1306_t *disp, bool color)
{
    uint8_t block = color ? 0xff : 0x00;
    
    memset(disp->buf, block, disp->x * disp->y / 8);
}


int bone_ssd1306_point(bone_ssd1306_t *disp, uint16_t x, uint16_t y,
                       bool color)
{
    uint16_t element;

    if (x >= disp->x || y >= disp->y)
        return -1;

    element = ((y/8)*disp->x)+x;
    if (color == 1) {
        disp->buf[element] |= color << (y % 8);
    } else if (color == 0) {
        disp->buf[element] &= color << (y % 8);
    }

    return 0;
}


int bone_ssd1306_line(bone_ssd1306_t *disp, uint16_t x0, uint16_t y0,
                       uint16_t x1, uint16_t y1, bool color)
{
    uint16_t dx = abs(x1 - x0);
    uint16_t dy = abs(y1 - y0); 
    uint16_t sx, sy;
    float err, e2;

    if (x0 >= disp->x || x1 >= disp->x || y0 >= disp->y || y1 >= disp->y) {
        return -1;
    }

    if (x0 < x1) {
        sx = 1;
    } else {
        sx = -1;
    }
    if (y0 < y1) {
        sy = 1;
    } else {
        sy = -1;
    }
    err = dx - dy;
 
    for (;;) {
        bone_ssd1306_point(disp, x0, y0, color);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = 2*err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    return 0;
}


int bone_ssd1306_char(bone_ssd1306_t *disp, uint16_t x, uint16_t y, bool color,
                      char c)
{
    int bufi = (disp->x*(y/8))+x;
    int bufiup = bufi + disp->x;

    if (x >= disp->x || y >= disp->y)
        return -1;

    if (y % 8 == 0) {
        for (int i = 0; i < 5 && x + i < disp->x; i++) {
            if (color == 1)
                disp->buf[bufi+i] |= bone_font[(5*c)+i];
            else
                disp->buf[bufi+i] &= ~bone_font[(5*c)+i];
        }
    } else {
        for (int i = 0; i < 5 && x + i < disp->x; i++) {
            if (color == 1)
                disp->buf[bufi+i] |= bone_font[(5*c)+i] << y % 8;
            else
                disp->buf[bufi+i] &= ~bone_font[(5*c)+i] << y % 8;
        }
        if (y + 8 < disp->y) {
            for (int i = 0; i < 5 && x + i < disp->x; i++) {
                if (color == 1)
                    disp->buf[bufiup+i] |= bone_font[(5*c)+i] >> (8 - (y % 8));
                else
                    disp->buf[bufiup+i] &= ~bone_font[(5*c)+i] >> (8 - (y % 8));
            }
        } else {
            return 1;
        }
    }

    return 0;
}


int bone_ssd1306_str(bone_ssd1306_t *disp, uint16_t x, uint16_t y, bool color,
                     char *s)
{
    int xx = x, yy = y, rv = 0;

    for (; *s != '\0' && rv >= 0; s++) {
        if (*s == '\n') {
            xx = x;
            yy -= 8;
            continue;
        } else if (*s == '\r') {
            xx = x;
            continue;
        } else if (*s == '\v') {
            yy -= 8;
            continue;
        } else if (*s == '\b') {
            xx -= 6;
            continue;
        }
        rv = bone_ssd1306_char(disp, xx, yy, color, *s);
        xx += 6;
    }

    return rv;
}
