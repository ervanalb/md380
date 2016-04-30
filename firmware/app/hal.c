#include "hal.h"
#include "stm32f4xx.h"

// TODO improve this a lot
static void delay_us(uint32_t n) {
    volatile uint32_t q;
    for(uint32_t i=0; i<n; i++) {
        for(q=0; q<16; q++);
    }
}

static void reset_lcd() {
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    delay_us(10);
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    delay_us(5000);
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

static void set_lcd_data_to_outputs() {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
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

void init() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC |
                           RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD, ENABLE);

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

    // LCD control lines
    // ~CS  = PD6
    // D/~C = PD12
    // ~RD  = PD4
    // ~WR  = PD5
    // ~RST = PD13
    GPIO_SetBits(GPIOD, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_12);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | 
                                  GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    set_lcd_data_to_inputs();
    set_keyboard_to_floating();
    reset_lcd();

    // LCD backlight
    lcd_backlight_off();
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void lcd_backlight_on() {GPIO_SetBits(GPIOC, GPIO_Pin_6);}
void lcd_backlight_off() {GPIO_ResetBits(GPIOC, GPIO_Pin_6);}

void lcd_test() {
    uint8_t a[5];
    set_keyboard_to_floating();

    GPIO_ResetBits(GPIOD, GPIO_Pin_6); // ~CS goes low
    delay_us(1);
    GPIO_ResetBits(GPIOD, GPIO_Pin_5); // ~WR goes low
    GPIO_ResetBits(GPIOD, GPIO_Pin_12); // D/~C goes C
    set_lcd_data_to_outputs();
    put_lcd_data(0x11);
    pause();
    GPIO_SetBits(GPIOD, GPIO_Pin_5); // ~WR goes high
    delay_us(120000);

    GPIO_ResetBits(GPIOD, GPIO_Pin_5); // ~WR goes low
    GPIO_ResetBits(GPIOD, GPIO_Pin_12); // D/~C goes C
    put_lcd_data(0x29);
    pause();
    GPIO_SetBits(GPIOD, GPIO_Pin_5); // ~WR goes high
    pause();

    pause();
    GPIO_ResetBits(GPIOD, GPIO_Pin_5); // ~WR goes low
    GPIO_ResetBits(GPIOD, GPIO_Pin_12); // D/~C goes C
    set_lcd_data_to_outputs();
    put_lcd_data(0x09);
    pause();
    GPIO_SetBits(GPIOD, GPIO_Pin_5); // ~WR goes high
    pause();
    GPIO_SetBits(GPIOD, GPIO_Pin_12); // D/~C goes D
    set_lcd_data_to_inputs();
    for(int i=0; i<8; i++) {
        GPIO_ResetBits(GPIOD, GPIO_Pin_4); // ~RD goes low
        pause();
        a[i] = get_lcd_data();
        GPIO_SetBits(GPIOD, GPIO_Pin_4); // ~RD goes high
        pause();
    }
    set_lcd_data_to_outputs();
    GPIO_SetBits(GPIOD, GPIO_Pin_6); // ~CS goes high
    pause();
    lcd_backlight_on();
    delay_us(10000000);
    lcd_backlight_off();
    for(;;);
}

// LCD Pin mapping:
// D0 = PD14
// D1 = PD15
// D2 = PD0
// D3 = PD1
// D4 = PE7
// D5 = PE8
// D6 = PE9
// D7 = PE10

uint8_t get_lcd_data() {
    uint16_t gpiod;
    uint16_t gpioe;
    gpiod = GPIO_ReadInputData(GPIOD);
    gpioe = GPIO_ReadInputData(GPIOE);
    return ((gpiod & (GPIO_Pin_14 | GPIO_Pin_15)) >> 14) | 
           ((gpiod & (GPIO_Pin_0 | GPIO_Pin_1)) << 2) |
           ((gpioe & (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10)) >> 3);
}

void put_lcd_data(uint8_t data) {
    GPIOD->BSRRL = ((data & 0x03) << 14) | ((data & 0x0C) >> 2);
    GPIOD->BSRRH = (((~data) & 0x03) << 14) | (((~data) & 0x0C) >> 2);
    GPIOE->BSRRL = ((data & 0xF0) << 3);
    GPIOE->BSRRH = (((~data) & 0xF0) << 3);
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
