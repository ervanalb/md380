#include "hal.h"

int main() {
    static volatile int i;
    for(;;) {
        for(i = 0; i < 16800000; i++);
        green_led_on();
        for(i = 0; i < 16800000; i++);
        green_led_off();
    }
}
