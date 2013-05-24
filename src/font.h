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

static const uint8_t bone_font[5*128] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFA, 0x00, 0x00,
    0xC0, 0xE0, 0x00, 0xE0, 0xC0,
    0x24, 0x7E, 0x24, 0x7E, 0x24,
    0x24, 0x54, 0xD6, 0x54, 0x48,
    0x32, 0x34, 0x08, 0x16, 0x26,
    0x6C, 0x92, 0x92, 0x3C, 0x12,
    0x00, 0x00, 0xE0, 0xC0, 0x00,
    0x00, 0x38, 0x44, 0x82, 0x00,
    0x00, 0x82, 0x44, 0x38, 0x00,
    0x2A, 0x1C, 0x08, 0x1C, 0x2A,
    0x08, 0x08, 0x3E, 0x08, 0x08,
    0x00, 0x00, 0x07, 0x06, 0x00,
    0x08, 0x08, 0x08, 0x08, 0x08,
    0x00, 0x00, 0x06, 0x06, 0x00,
    0x02, 0x04, 0x08, 0x10, 0x20,
    0x7C, 0x8A, 0x92, 0xA2, 0x7C,
    0x00, 0x42, 0xFE, 0x02, 0x00,
    0x42, 0x86, 0x8A, 0x92, 0x62,
    0x44, 0x82, 0x92, 0x92, 0x6C,
    0x18, 0x28, 0x48, 0xFE, 0x08,
    0xE4, 0xA2, 0xA2, 0xA2, 0x9C,
    0x7C, 0x92, 0x92, 0x92, 0x4C,
    0x80, 0x80, 0x8E, 0x90, 0xE0,
    0x6C, 0x92, 0x92, 0x92, 0x6C,
    0x64, 0x92, 0x92, 0x92, 0x7C,
    0x00, 0x00, 0x12, 0x00, 0x00,
    0x00, 0x01, 0x12, 0x00, 0x00,
    0x00, 0x08, 0x14, 0x22, 0x00,
    0x14, 0x14, 0x14, 0x14, 0x14,
    0x00, 0x22, 0x14, 0x08, 0x00,
    0x40, 0x80, 0x8A, 0x90, 0x60,
    0x7C, 0x92, 0xAA, 0x92, 0x74,
    0x3E, 0x48, 0x88, 0x48, 0x3E,
    0xFE, 0x92, 0x92, 0x92, 0x6C,
    0x7C, 0x82, 0x82, 0x82, 0x44,
    0xFE, 0x82, 0x82, 0x44, 0x38,
    0xFE, 0x92, 0x92, 0x92, 0x82,
    0xFE, 0x90, 0x90, 0x90, 0x80,
    0x7C, 0x82, 0x92, 0x92, 0x5C,
    0xFE, 0x10, 0x10, 0x10, 0xFE,
    0x00, 0x82, 0xFE, 0x82, 0x00,
    0x0C, 0x02, 0x02, 0x02, 0xFC,
    0xFE, 0x10, 0x28, 0x44, 0x82,
    0xFE, 0x02, 0x02, 0x02, 0x02,
    0xFE, 0x40, 0x30, 0x40, 0xFE,
    0xFE, 0x40, 0x20, 0x10, 0xFE,
    0x7C, 0x82, 0x82, 0x82, 0x7C,
    0xFE, 0x90, 0x90, 0x90, 0x60,
    0x7C, 0x82, 0x8A, 0x86, 0x7E,
    0xFE, 0x90, 0x98, 0x94, 0x62,
    0x64, 0x92, 0x92, 0x92, 0x4C,
    0x80, 0x80, 0xFE, 0x80, 0x80,
    0xFC, 0x02, 0x02, 0x02, 0xFC,
    0xF0, 0x0C, 0x02, 0x0C, 0xF0,
    0xFC, 0x02, 0x1C, 0x02, 0xFC,
    0xC6, 0x28, 0x10, 0x28, 0xC6,
    0xE0, 0x10, 0x0E, 0x10, 0xE0,
    0x86, 0x8A, 0x92, 0xA2, 0xC2,
    0x00, 0xFE, 0x82, 0x82, 0x00,
    0x20, 0x10, 0x08, 0x04, 0x02,
    0x00, 0x82, 0x82, 0xFE, 0x00,
    0x20, 0x40, 0x80, 0x40, 0x20,
    0x02, 0x02, 0x02, 0x02, 0x02,
    0x00, 0xC0, 0xE0, 0x00, 0x00,
    0x04, 0x2A, 0x2A, 0x2A, 0x1E,
    0xFE, 0x22, 0x22, 0x22, 0x1C,
    0x1C, 0x22, 0x22, 0x22, 0x04,
    0x1C, 0x22, 0x22, 0x22, 0xFE,
    0x1C, 0x2A, 0x2A, 0x2A, 0x18,
    0x10, 0x7E, 0x90, 0x80, 0x40,
    0x19, 0x25, 0x25, 0x25, 0x3E,
    0xFE, 0x20, 0x20, 0x20, 0x1E,
    0x00, 0x22, 0xBE, 0x02, 0x00,
    0x02, 0x01, 0x01, 0x01, 0xBE,
    0xFE, 0x08, 0x14, 0x22, 0x00,
    0x00, 0x82, 0xFE, 0x02, 0x00,
    0x3E, 0x20, 0x1E, 0x20, 0x1E,
    0x3E, 0x20, 0x20, 0x20, 0x1E,
    0x1C, 0x22, 0x22, 0x22, 0x1C,
    0x3F, 0x24, 0x24, 0x24, 0x18,
    0x18, 0x24, 0x24, 0x24, 0x3F,
    0x3E, 0x10, 0x20, 0x20, 0x20,
    0x12, 0x2A, 0x2A, 0x2A, 0x24,
    0x20, 0x20, 0xFE, 0x20, 0x20,
    0x3C, 0x02, 0x02, 0x02, 0x3E,
    0x30, 0x0C, 0x02, 0x0C, 0x30,
    0x3C, 0x02, 0x0C, 0x02, 0x3C,
    0x22, 0x14, 0x08, 0x14, 0x22,
    0x38, 0x05, 0x05, 0x05, 0x3E,
    0x22, 0x26, 0x2A, 0x32, 0x22,
    0x00, 0x10, 0x6C, 0x82, 0x82,
    0x00, 0x00, 0xFE, 0x00, 0x00,
    0x00, 0x82, 0x82, 0x6C, 0x10,
    0x60, 0x80, 0x40, 0x20, 0xC0,
    0x00, 0x00, 0x00, 0x00, 0x00 };
