#include "lcd.h"
#include "hal.h"

static void set_addresses(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    lcd_write_command(0x2A); // CASET
    lcd_write16(x);
    lcd_write16(x + w - 1);
    lcd_write_command(0x2B); // PASET
    lcd_write16(y);
    lcd_write16(y + h - 1);
    lcd_write_command(0x2C); // RAMWR
}

void lcd_on() {
    lcd_assert_cs();
    lcd_write_command(0x11); // Sleep out
    delay_us(120000);
    lcd_write_command(0x3A); // PIXFMT
    lcd_write(0x55); // 16-bit color 565RGB
    lcd_write_command(0x36); // MAC
    lcd_write(0xA0); // Sets proper orientation
    lcd_write_command(0x29); // Disp on
    lcd_deassert_cs();

    lcd_fill(0, 0, 160, 128, 0x0000);
}

void lcd_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    lcd_assert_cs();
    set_addresses(x, y, w, h);
    for(uint16_t i=0; i<h; i++) {
        for(uint16_t j=0; j<w; j++) {
            lcd_write16(color);
        }
    }
    lcd_deassert_cs();
}

void lcd_blit(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t * data) {
    lcd_assert_cs();
    set_addresses(x, y, w, h);
    for(uint16_t i=0; i<h; i++) {
        for(uint16_t j=0; j<w; j++) {
            lcd_write16(*data++);
        }
    }
    lcd_deassert_cs();
}
