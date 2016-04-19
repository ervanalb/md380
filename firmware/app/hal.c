#include "hal.h"
#include "stm32f4xx.h"

void init() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    // Green / Red LED
    red_led_off();
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void green_led_on() {
    GPIO_SetBits(GPIOE, GPIO_Pin_0);
}

void green_led_off() {
    GPIO_ResetBits(GPIOE, GPIO_Pin_0);
}

void red_led_on() {
    GPIO_SetBits(GPIOE, GPIO_Pin_1);
}

void red_led_off() {
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
}
