/***********************************************************************
 * 
 * Blink LEDs in Arduino-style and use function from the delay library.
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2022 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Defines -----------------------------------------------------------*/
#define LED_GREEN PB5   // PB5 is AVR pin where green on-board LED
                        // is connected
#define LED_RED PB0  
#define LED_YELLOW PB1
#define LED_RED_PB PB2
#define LED_GREEN_PB PB3
#define BUTTON PD0   // External active-low LED
#define SHORT_DELAY 250 // Delay in milliseconds
#ifndef F_CPU
# define F_CPU 16000000 // CPU frequency in Hz required for delay funcs
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>     // AVR device-specific IO definitions
#include <util/delay.h> // Functions for busy-wait delay loops
#include<gpio.h>

// -----
// This part is needed to use Arduino functions but also physical pin
// names. We are using Arduino-style just to simplify the first lab.
//#include "Arduino.h"
//#define PB5 13          // In Arduino world, PB5 is called "13"
//#define PB0 8

// -----


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle LEDs and use delay library.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    //uint8_t led_value = 0;  // Local variable to keep LED status

    // Set pin where on-board LED is connected as output
    //pinMode(LED_GREEN, OUTPUT);
    // Set second pin as output
    //pinMode(LED_RED, OUTPUT);
    //DDRB = DDRB | (1<<LED_GREEN);
    //DDRB = DDRB | (1<<LED_RED);
    GPIO_mode_output(&DDRB, LED_GREEN_PB);
    GPIO_mode_output(&DDRB, LED_RED_PB); 
    GPIO_mode_output(&DDRB, LED_GREEN);
    GPIO_mode_output(&DDRB, LED_RED);
    GPIO_mode_output(&DDRB, LED_YELLOW);
    GPIO_mode_input_pullup(&DDRD, BUTTON);
    // Infinite loop
    while (1)
    {
        // Turn ON/OFF on-board LED ...
        //digitalWrite(LED_GREEN, led_value);
        // ... and external LED as well
        //digitalWrite(LED_RED, led_value);

        // Pause several milliseconds
        _delay_ms(SHORT_DELAY);
        // Change LED value
        GPIO_write_high(&PORTB,LED_RED);
        _delay_ms(2000);
        GPIO_write_high(&PORTB,LED_YELLOW);
        _delay_ms(500);
        GPIO_write_low(&PORTB,LED_RED);
        GPIO_write_low(&PORTB,LED_YELLOW);
        _delay_ms(50);
        GPIO_write_high(&PORTB,LED_GREEN);
        _delay_ms(3000);
        GPIO_write_low(&PORTB,LED_GREEN);
        _delay_ms(500);
        GPIO_write_high(&PORTB,LED_YELLOW);
        _delay_ms(500);
        GPIO_write_low(&PORTB,LED_YELLOW);
        _delay_ms(500);
        if (GPIO_read(&PIND, BUTTON) == 1)
        {
            //led_value = 1;
            //PORTB = PORTB | (1<<LED_GREEN);
            GPIO_write_high(&PORTB,LED_RED);
            _delay_ms(200);
            //PORTB = PORTB | (1<<LED_RED);
            GPIO_write_high(&PORTB,LED_GREEN_PB);
            GPIO_write_low(&PORTB,LED_RED_PB);
            _delay_ms(2000);
            GPIO_write_low(&PORTB,LED_GREEN_PB);
            GPIO_write_high(&PORTB,LED_RED_PB);
        }
        else
        {
            GPIO_write_low(&PORTB,LED_GREEN_PB);
            //led_value = 0;
            //PORTB = PORTB & ~(1<<LED_GREEN);
            //PORTB = PORTB & ~(1<<LED_RED);
            GPIO_write_high(&PORTB,LED_RED_PB);
            // GPIO_write_low(&PORTB,LED_GREEN);
            // GPIO_write_low(&PORTB,LED_RED);
        }
    }

    // Will never reach this
    return 0;
}