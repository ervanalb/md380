#include "hal.h"
#include "stm32f4xx.h"

int main() {
    init();

    static volatile uint32_t keyboard;

    lcd_test();

    for(;;) {
    }
}
