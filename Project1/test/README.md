# Project 1 - 

Application of analog joy-stick (2 ADC channels, 1 push button), rotary encoder, and Digilent PmodCLP LCD module.

## Instructions

The goal of the project is cooperation in pairs, further study of the topic, design of own solutions, implementation, creation of project documentation and presentation of results. The division of roles and tasks within the team is up to their members.

* Students work on a project in the labs during the 10th and 11th weeks of the semester.

* Through the BUT e-learning, students submit a link to the GitHub repository, which contains the complete project in PlatfomIO and a descriptive README file. The submission deadline is the day before the next laboratory, i.e. in 2 weeks.

* The source code for the AVR must be written in C and/or Assembly and must be implementable on Arduino Uno board using toolchains from the semester. No other development tools are allowed.

## Recommended GitHub repository structure

   ```c
   YOUR_PROJECT        // PlatfomIO project
   ├── include         // Included files
   ├── lib             // Libraries
   ├── src             // Source file(s)
   │   └── main.c
   ├── test            // No need this
   ├── platformio.ini  // Project Configuration File
   └── README.md       // Report of this project
   ```

## Recommended README.md file structure

### Team members

* Josef Komár 230271 (responsible for xxx)
* Ondřej Sedlák 230312 (responsible for cicina)

## Hardware description

Insert descriptive text and schematic(s) of your implementation.

## Software description

Put flowchats of your algorithm(s). Write descriptive text of your libraries and source files. Put direct links to these files in `src` or `lib` folders.

## Video

Insert a link to a short video with your practical implementation example (1-3 minutes, e.g. on YouTube).

## References

1. Write your text here.
2. ...









//////////////// zaloha /////////////////










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
#define LED_1 PB6
#define LED_2 PB3
#define LED_3 PB4
#define LED_4 PB5
#define LED_5 PC1
//#define LED_6 PB5
//#define LED_7 PB4


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions
#include <uart.h>

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


    GPIO_mode_output(&DDRB, LED_1);
    GPIO_mode_output(&DDRB, LED_2);
    GPIO_mode_output(&DDRB, LED_3);
    GPIO_mode_output(&DDRB, LED_4);
    GPIO_mode_output(&DDRC, LED_5);
    //GPIO_mode_output(&DDRC, LED_6);
    //GPIO_mode_output(&DDRC, LED_7);

    // Initialize display
    lcd_init(LCD_DISP_ON);
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
    // Start ADC conversion
  static int8_t nooverflow = 0;
  nooverflow++;
  if(nooverflow > 6)
  {
    nooverflow = 0;
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
  itoa(value, string, 10);
  lcd_gotoxy(8,0);
  lcd_puts("    ");
  lcd_gotoxy(8,0);
  lcd_puts(string);

  itoa(value, string, 16);
  lcd_gotoxy(13,0);
  lcd_puts("   ");
  lcd_gotoxy(13,0);
  lcd_puts(string);

  GPIO_write_high(&PORTB,LED_1);
  GPIO_write_high(&PORTB,LED_2);
  GPIO_write_high(&PORTB,LED_3);
  GPIO_write_high(&PORTB,LED_4);
  GPIO_write_high(&PORTC,LED_5);

  if(value > 800)
  {
    GPIO_write_low(&PORTC,LED_5);
    GPIO_write_low(&PORTB,LED_4);
    GPIO_write_low(&PORTB,LED_3);
    GPIO_write_low(&PORTB,LED_2);
    GPIO_write_low(&PORTB,LED_1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED1-5");
    
  }
  else if (value > 600)
  {
    GPIO_write_low(&PORTB,LED_4);
    GPIO_write_low(&PORTB,LED_3);
    GPIO_write_low(&PORTB,LED_2);
    GPIO_write_low(&PORTB,LED_1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED1-4");
  }
  else if (value > 400)
  {
    GPIO_write_low(&PORTB,LED_3);
    GPIO_write_low(&PORTB,LED_2);
    GPIO_write_low(&PORTB,LED_1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED1-3");
  }
  else if (value > 200)
  {
    GPIO_write_low(&PORTB,LED_2);
    GPIO_write_low(&PORTB,LED_1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED1-2");
  }
  else if (value > 0)
  {
    GPIO_write_low(&PORTB,LED_1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED1");
  }
  
}