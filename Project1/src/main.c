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
#include <uart.h>

// LEDS
#define LED1 PB2
#define LED2 PB3
#define LED3 PB4
#define LED4 PB5
#define LED5 PB6
//#define LED6 PC4
//#define LED7 PC5

// ENCODER
#define OutputCLK PD1
#define OutputDT PD2
#define OutputSW PD3

// JOYSTICK
#define OutputSWJ PB7
// X-AXIS PC0
// Y-AXIS PC1

static int8_t counter = 0; 
static uint8_t aState;
static uint8_t aLastState;
static uint8_t button = 1;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    GPIO_mode_input_nopull(&DDRD,OutputCLK);
    GPIO_mode_input_nopull(&DDRD,OutputDT);
    GPIO_mode_input_pullup(&DDRD,OutputSW);

    aLastState = GPIO_read(&PIND,OutputCLK);

    // Initialize display
    lcd_init(LCD_DISP_ON);

    GPIO_mode_output(&DDRB,LED1);
    GPIO_mode_output(&DDRB,LED2);
    GPIO_mode_output(&DDRB,LED3);
    GPIO_mode_output(&DDRB,LED4);
    GPIO_mode_output(&DDRB,LED5);
 //   GPIO_mode_output(&DDRC,LED4);
 //   GPIO_mode_output(&DDRC,LED5);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX = ADMUX & ~( 1<<MUX3 | 1<<MUX2 | 1<<MUX0 | 1<<MUX1);
    // Enable ADC module
    ADCSRA |= (1<<ADEN);

    // Enable conversion complete interrupt
    ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    TIM2_overflow_2ms();
    TIM2_overflow_interrupt_enable();
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {

    }

    return 0;
}


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

button = GPIO_read(&PIND,OutputSW);

    if (button == 0){

        counter = 0;

        itoa(counter, string, 10);
        lcd_gotoxy(8, 0);
        lcd_puts("      ");
        lcd_gotoxy(8, 0);
        lcd_puts(string);
    }
}




/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
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
  //char string[4];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
  value = ADC;
    // Convert "value" to "string" and display it

  lcd_gotoxy(13,0);
  lcd_puts("KOK");

  GPIO_write_high(&PORTB,LED1);
  GPIO_write_high(&PORTB,LED2);
  GPIO_write_high(&PORTB,LED3);
  GPIO_write_high(&PORTB,LED4);
  GPIO_write_high(&PORTB,LED5);

  if(value > 800)
  {
    GPIO_write_low(&PORTB,LED5);
    GPIO_write_low(&PORTB,LED4);
    GPIO_write_low(&PORTB,LED3);
    GPIO_write_low(&PORTB,LED2);
    GPIO_write_low(&PORTB,LED1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("none");
    
  }
  else if (value > 600)
  {
    GPIO_write_low(&PORTB,LED4);
    GPIO_write_low(&PORTB,LED3);
    GPIO_write_low(&PORTB,LED2);
    GPIO_write_low(&PORTB,LED1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("select");
  }
  else if (value > 400)
  {
    GPIO_write_low(&PORTB,LED3);
    GPIO_write_low(&PORTB,LED2);
    GPIO_write_low(&PORTB,LED1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("left");
  }
  else if (value > 200)
  {
    GPIO_write_low(&PORTB,LED2);
    GPIO_write_low(&PORTB,LED1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("down");
  }
  else if (value > 0)
  {
    GPIO_write_low(&PORTB,LED1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("up");
  }
  
}