#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "uart1.h"
#include "max7219.h"

#define DIN_PORT GPIOB
#define DIN_PIN GPIO_PIN_4
#define CS_PORT GPIOB
#define CS_PIN GPIO_PIN_3
#define CLK_PORT GPIOB
#define CLK_PIN GPIO_PIN_2

void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
}

void display(uint8_t address, uint8_t data) {
    uint8_t mask;
    LOW(CS); // začátek přenosu

    /* pošlu adresu */
    mask = 128;
    mask = 1 << 7;
    mask = 0b10000000;
    while (mask) {
        if (address & mask) {
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1;
        LOW(CLK);
    }
    /* pošlu data */
    mask = 0b10000000;
    while (mask) {
        if (data & mask) {
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1;
        LOW(CLK);
    }

    HIGH(CS); // konec přenosu
}

int main(void) {

    uint32_t time = 0;
    uint8_t number = 0;
    uint16_t namber = 0;
    uint16_t nuumber = 0;

    init();

    display(DECODE_MODE, 0b11111111);
    display(SCAN_LIMIT, 7);
    display(INTENSITY, 1);
    display(DISPLAY_TEST, DISPLAY_TEST_OFF);
    display(SHUTDOWN, SHUTDOWN_ON);
    display(DIGIT0, 0xF);
    display(DIGIT1, 0xF);
    display(DIGIT2, 0xF);
    display(DIGIT3, 0xF);
    display(DIGIT4, 0xF);
    display(DIGIT5, 0xF);
    display(DIGIT6, 0xF);
    display(DIGIT7, 0xF);

    while (1) {
        if (milis() - time > 333) {
            time = milis();

            // Počítání 0-9
            display(DIGIT0, number);
            number = (number + 1) % 10;

            // Počítání 0-99
            uint8_t des = (namber / 10) % 10;
            uint8_t jed = namber % 10;
            display(DIGIT3, des);
            display(DIGIT2, jed);
            namber = (namber + 1) % 100;

            // Počítání 0-999
            uint8_t hundreds = (nuumber / 100) % 10;
            uint8_t tens = (nuumber / 10) % 10;
            uint8_t ones = nuumber % 10;
            display(DIGIT7, hundreds);
            display(DIGIT6, tens);
            display(DIGIT5, ones);
            nuumber = (nuumber + 1) % 1000;
            
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
