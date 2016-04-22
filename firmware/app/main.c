#include "hal.h"
#include "stm32f4xx.h"

int main() {
    static volatile int i;

    init();

    for(;;) {
        for(i = 0; i < 1680000; i++);
        green_led_on();
        for(i = 0; i < 1680000; i++);
        green_led_off();
        for(i = 0; i < 1680000; i++);
        red_led_on();
        for(i = 0; i < 1680000; i++);
        red_led_off();
    }
}
