#include "hal.h"
#include "lcd.h"
#include "stm32f4xx.h"

uint16_t sprite[] = {
    0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF,
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
};

int main() {
    init();

    static volatile uint32_t keyboard;

    lcd_on();
    lcd_backlight_on();
    lcd_fill(0, 0, 80, 30, 0xF800);
    lcd_fill(30, 30, 80, 30, 0x07E0);
    lcd_fill(60, 60, 80, 30, 0x001F);
    lcd_blit(45, 45, 5, 5, sprite);

    for(;;) {
    }
}
