/* libbone - A library for working with the BeagleBone in C
 * Copyright (c) 2012-2013 Clayton G. Hobbs
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

#ifndef __BONE_GPIO_H__
#define __BONE_GPIO_H__

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


enum bone_gpio_edge {
    NONE = 0,
    RISING,
    FALLING,
    BOTH
};

int bone_gpio_export(int pnpin);
int bone_gpio_unexport(int pnpin);
int bone_gpio_get_value(int pnpin);
int bone_gpio_set_value(int pnpin, int value);
int bone_gpio_open_value(int pnpin);
int bone_gpio_get_dir(int pnpin);
int bone_gpio_set_dir(int pnpin, int dir);
enum bone_gpio_edge bone_gpio_get_edge(int pnpin);
int bone_gpio_set_edge(int pnpin, enum bone_gpio_edge e);
/* TODO: active low */


#endif /* __BONE_GPIO_H__ */
