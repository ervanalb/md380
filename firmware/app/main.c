#include "hal.h"

int main() {
    static volatile int i;
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
