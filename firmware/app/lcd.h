#ifndef __LCD_H
#define __LCD_H

#include <stdint.h>

void lcd_on();
void lcd_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void lcd_blit(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t * data);

#endif
