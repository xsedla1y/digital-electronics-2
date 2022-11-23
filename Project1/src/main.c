/***********************************************************************
 * 
 * Stopwatch by Timer/Counter2 on the Liquid Crystal Display (LCD)
 *
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2017 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 * Components:
 *   16x2 character LCD with parallel interface
 *     VSS  - GND (Power supply ground)
 *     VDD  - +5V (Positive power supply)
 *     Vo   - (Contrast)
 *     RS   - PB0 (Register Select: High for Data transfer, Low for Instruction transfer)
 *     RW   - GND (Read/Write signal: High for Read mode, Low for Write mode)
 *     E    - PB1 (Read/Write Enable: High for Read, falling edge writes data to LCD)
 *     D3:0 - NC (Data bits 3..0, Not Connected)
 *     D4   - PD4 (Data bit 4)
 *     D5   - PD5 (Data bit 5)
 *     D6   - PD6 (Data bit 6)
 *     D7   - PD7 (Data bit 7)
 *     A+K  - Back-light enabled/disabled by PB2
 * 
 **********************************************************************/

// ENCODER
#define OutputCLK PD2
#define OutputDT PD3
#define OutputSW PB2

// JOYSTICK
#define OutputSWJ PB3

// LEDS
#define LED_1 PC6
#define LED_2 PC5
#define LED_3 PC4
#define LED_4 PC3
#define LED_5 PC2
#define LED_6 PB5
#define LED_7 PB4


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions

    static int8_t counter = 0; 
    static uint8_t aState;
    static uint8_t aLastState;
    static uint8_t button = 1;
 

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD screen when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    GPIO_mode_input_nopull(&DDRD,OutputCLK);
    GPIO_mode_input_nopull(&DDRD,OutputDT);
    GPIO_mode_input_pullup(&DDRB,OutputSW);

    aLastState = GPIO_read(&PIND,OutputCLK);

    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(0, 0);
    lcd_gotoxy(0, 0);
    lcd_puts("Counter:0");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"

    ADMUX = ADMUX | (1<<REFS0);

    // Select input channel ADC0 (voltage divider pin)

    ADMUX = ADMUX & ~(1<<MUX0 | 1<<MUX1 | 1<<MUX2 | 1<<MUX3);

    // Enable ADC module

    ADCSRA |= (1<<ADEN);

    // Enable conversion complete interrupt

    ADCSRA |= (1<<ADIE);

    // Set clock prescaler to 128

    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Configuration of 8-bit Timer/Counter2 for Stopwatch update
    // Set the overflow prescaler to 16 ms and enable interrupt

    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();
    TIM2_overflow_1ms();
    TIM2_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {

    }
    // Will never reach this
    return 0;
}



/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD screen every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER2_OVF_vect)
{
    char string[4];
    aState = GPIO_read(&PIND,OutputCLK);
    lcd_gotoxy(0, 0);
    lcd_puts("Counter:");

    if (aState != aLastState && aState == 1){

        if (GPIO_read(&PIND,OutputDT) != aState) {
        counter ++;
        }
        
        else {
        counter --;
        }

        itoa(counter, string, 10);
        lcd_gotoxy(8, 0);
        lcd_puts("      ");
        lcd_gotoxy(8, 0);
        lcd_puts(string);
    }

aLastState = aState;

button = GPIO_read(&PINB,OutputSW);

    if (button == 0){

        counter = 0;

        itoa(counter, string, 10);
        lcd_gotoxy(8, 0);
        lcd_puts("      ");
        lcd_gotoxy(8, 0);
        lcd_puts(string);
    }
}


ISR(TIMER1_OVF_vect)
{
    static uint8_t no_of_overflows = 0;

    no_of_overflows++;
    if (no_of_overflows >= 3)
    {
      // Do this every 6 x 16 ms = 100 ms
      no_of_overflows = 0;
      // Count tenth of seconds 0, 1, ..., 9, 0, 1, ...

      // Start ADC conversion
      ADCSRA |= (1<<ADSC);
    }
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
    uint16_t value;
    char string[4];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    value = ADC;
    // Convert "value" to "string" and display it


/*    itoa(value, string, 10);
    lcd_gotoxy(8, 0);
    lcd_puts("    ");
    lcd_gotoxy(8, 0);
    lcd_puts(string);
    itoa(value, string, 16);
    lcd_gotoxy(13,0); 
    lcd_puts("   ");
    lcd_gotoxy(13,0); 
    lcd_puts(string);
*/

    // ADC measured:
    // none: 1023
    // select: 640
    // left: 410
    // down: 257
    // up: 100
    // right: 0

    itoa(value, string, 10);
    // Converting a numeric string
    int x = atoi(string);

    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    if (x > 830)
        lcd_puts("none");
    else if (x > 520)
        lcd_puts("Select");
    else if (x > 330)
        lcd_puts("Left");
    else if (x > 180)
        lcd_puts("Down");
    else if (x > 50)
        lcd_puts("Up");
    else
      lcd_puts("Right");
}