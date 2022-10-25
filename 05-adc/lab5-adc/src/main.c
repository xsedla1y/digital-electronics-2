/***********************************************************************
 * 
 * Use Analog-to-digital conversion to read push buttons on LCD keypad
 * shield and display it on LCD screen.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");  // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");  // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");  // Put button name here

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

    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/

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
    uint16_t value2;
    char string[4];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    value = ADC;
    // Convert "value" to "string" and display it
    itoa(value, string, 2);
    lcd_gotoxy(8, 0);
    lcd_puts("    ");
    lcd_gotoxy(8, 0);
    lcd_puts(string);
    itoa(value, string, 16);
    lcd_gotoxy(13,0); 
    lcd_puts("   ");
    lcd_gotoxy(13,0); 
    lcd_puts(string);

    // ADC measured:
    // none: 1023
    // select: 640
    // left: 410
    // down: 257
    // up: 100
    // right: 0
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    if (string > 830)
        lcd_puts("none");
    else if (string > 520)
        lcd_puts("Select");
    else if (string > 330)
        lcd_puts("Left");
    else if (string > 180)
        lcd_puts("Down");
    else if (string > 50)
        lcd_puts("Up");
    else
      lcd_puts("Down");
}