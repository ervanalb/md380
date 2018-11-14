#ifndef __HAL_H
#define __HAL_H

#include <stdint.h>

void init();
void green_led_on();
void green_led_off();
void red_led_on();
void red_led_off();
uint32_t read_keyboard();
uint8_t get_ptt();
void lcd_backlight_on();
void lcd_backlight_off();
void lcd_assert_cs();
void lcd_deassert_cs();
void lcd_write16(uint16_t data);
void lcd_write(uint8_t byte);
void lcd_write_command(uint8_t byte);
uint8_t lcd_read();
void delay_us(uint32_t n);
uint8_t get_encoder();

// Keyboard bits
#define KEY_1     0x000001
#define KEY_2     0x000002
#define KEY_3     0x000004
#define KEY_4     0x000008
#define KEY_5     0x000010
#define KEY_6     0x000020
#define KEY_0     0x000040
#define KEY_STAR  0x000080
#define KEY_GREEN 0x000100
#define KEY_UP    0x000200
#define KEY_DOWN  0x000400
#define KEY_7     0x0000800
#define KEY_8     0x001000
#define KEY_9     0x002000
#define KEY_POUND 0x004000
#define KEY_RED   0x008000
#define KEY_SIDE  0x400000
#define KEY_MONI  0x800000
 
#endif
