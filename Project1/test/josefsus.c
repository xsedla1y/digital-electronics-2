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
#define LED5 PC3
#define LED6 PC4
#define LED7 PC5

// ENCODER
#define OutputCLK PD1
#define OutputDT PD2
#define OutputSW PD3

// JOYSTICK
#define OutputSWJ PC2
// X-AXIS PC0
// Y-AXIS PC1

static int8_t counter = 0; 
static uint8_t aState;
static uint8_t aLastState;
static uint8_t button = 1;
static uint8_t internalCounter = 0;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted valueX to LCD screen.
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
    GPIO_mode_output(&DDRC,LED5);
    GPIO_mode_output(&DDRC,LED6);
    GPIO_mode_output(&DDRC,LED7);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);
    // Select input channel ADC0 & ADC1 (voltage divider pin)
    ADMUX = ADMUX & ~( 1<<MUX3 | 1<<MUX2 | 1<<MUX0 | 1<<MUX1);
    ADMUX &= ~((1<<MUX3 | 1<<MUX2 | 1<<MUX1)); ADMUX |= (1<<MUX0);
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
 * Purpose:  Use single conversion mode and start conversion every 132 ms.
 **********************************************************************/

ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
  static int8_t nooverflow = 0;
  nooverflow++;
  if(nooverflow > 3)
  {
  nooverflow = 0;
  ADCSRA |= (1<<ADSC);
  }
  
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted valueX on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
  internalCounter++;
  uint16_t valueX;
  char string[4];  // String for converted numbers by itoa()

    // Read converted valueX
    // Note that, register pair ADCH and ADCL can be read as a 16-bit valueX ADC
  valueX = ADC;
    // Convert "valueX" to "string" and display it

  itoa(valueX, string, 10);
  lcd_gotoxy(0,1);
  lcd_puts("    ");
  lcd_gotoxy(0,1);
  lcd_puts(string);

  GPIO_write_high(&PORTB,LED1);
  GPIO_write_high(&PORTB,LED2);
  GPIO_write_high(&PORTB,LED3);
  GPIO_write_high(&PORTB,LED4);
  GPIO_write_high(&PORTC,LED5);
  GPIO_write_high(&PORTC,LED6);
  GPIO_write_high(&PORTC,LED7);

  if(valueX > 800)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
            GPIO_write_low(&PORTC,LED7);
        }
        else if(i == 0){
          GPIO_write_low(&PORTC,LED7);
        } // 1sec pause
    }
    }
    
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED_7");
    
  }
  else if (valueX > 700)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
            GPIO_write_low(&PORTC,LED6);
        }
        else if(i == 0){
          GPIO_write_low(&PORTC,LED6);
        } // 1sec pause
    }
    }
    
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED_6");
  }
  else if (valueX > 600)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
            GPIO_write_low(&PORTC,LED5);
        }
        else if(i == 0){
          GPIO_write_low(&PORTC,LED5);
        } // 1sec pause
    }
    }
    
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED_5");
  }
  else if (valueX > 500)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
          GPIO_write_low(&PORTB,LED4);
        } // 1sec pause
        else if(i == 0){
          GPIO_write_low(&PORTB,LED4);
        }
    }
    }

    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED_4");
  }
  else if (valueX > 400)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
            GPIO_write_low(&PORTB,LED3);
        }
        else if(i == 0){
          GPIO_write_low(&PORTB,LED3);
        } // 1sec pause
    }
    }
    
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED_3");
  }
  else if (valueX > 300)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
            GPIO_write_low(&PORTB,LED2);
        }
        else if(i == 0){
          GPIO_write_low(&PORTB,LED2);
        } // 1sec pause
    }
    }
    
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED_2");
  }
  else if (valueX > 200)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
            GPIO_write_low(&PORTB,LED1);
        }
        else if(i == 0){
          GPIO_write_low(&PORTB,LED1);
        } // 1sec pause
    }
    }
    
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("LED_1");
  }
  else if (valueX > 100)
  {
    GPIO_write_low(&PORTB,LED1);
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    lcd_gotoxy(8, 1);
    lcd_puts("OFF");
  }
  
}