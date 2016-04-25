#include "hal.h"
#include "stm32f4xx.h"

int main() {
    init();

    static volatile uint32_t keyboard;

    for(;;) {
        if(get_ptt()) {
            red_led_on();
        } else {
            red_led_off();
        }
    }
}
