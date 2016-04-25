#include "hal.h"
#include "stm32f4xx.h"

void init() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD, ENABLE);

    // Green / Red LED
    green_led_off();
    red_led_off();
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // PTT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

static void set_lcd_data_to_inputs() {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

static void set_keyboard_to_output() {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void set_keyboard_to_floating() {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void pause() {
    static volatile int i;
    for(i = 0; i < 15; i++);
}

uint8_t get_lcd_data() {
    uint32_t gpiod;
    uint32_t gpioe;
    gpiod = GPIO_ReadInputData(GPIOD);
    gpioe = GPIO_ReadInputData(GPIOE);
    return ((gpiod & (GPIO_Pin_14 | GPIO_Pin_15)) >> 14) | 
           ((gpiod & (GPIO_Pin_0 | GPIO_Pin_1)) << 2) |
           ((gpioe & (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10)) >> 3);
}

uint32_t read_keyboard() {
    uint32_t keyboard = 0;
    set_keyboard_to_output();
    set_lcd_data_to_inputs();
    GPIO_SetBits(GPIOA, GPIO_Pin_6); // KS1
    pause();
    keyboard |= get_lcd_data();
    GPIO_ResetBits(GPIOA, GPIO_Pin_6); // KS1
    GPIO_SetBits(GPIOD, GPIO_Pin_2); // KS2
    pause();
    keyboard |= (get_lcd_data() << 8);
    GPIO_ResetBits(GPIOD, GPIO_Pin_2); // KS2
    GPIO_SetBits(GPIOD, GPIO_Pin_3); // KS3
    pause();
    keyboard |= ((uint32_t)get_lcd_data() << 16);
    GPIO_ResetBits(GPIOD, GPIO_Pin_3); // KS3
    set_keyboard_to_floating();
    return keyboard;
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

uint8_t get_ptt() {
    return !(GPIO_ReadInputData(GPIOE) & GPIO_Pin_11);
}
