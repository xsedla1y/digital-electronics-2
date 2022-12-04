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

// ENCODER
#define OutputCLK PC5
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
static uint8_t buttonJOY = 1;
static uint8_t internalCounter = 0;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    GPIO_mode_input_nopull(&DDRC,OutputCLK);
    GPIO_mode_input_nopull(&DDRD,OutputDT);
    GPIO_mode_input_pullup(&DDRD,OutputSW);
    GPIO_mode_input_pullup(&DDRC,OutputSWJ);

    aLastState = GPIO_read(&PINC,OutputCLK);

    // Initialize display
    lcd_init(LCD_DISP_ON);

    GPIO_mode_output(&DDRB,LED1);
    GPIO_mode_output(&DDRB,LED2);
    GPIO_mode_output(&DDRB,LED3);
    GPIO_mode_output(&DDRB,LED4);
    GPIO_mode_output(&DDRC,LED5);
    

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);
    // Enable ADC module
    ADCSRA |= (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_4ms();
    TIM1_overflow_interrupt_enable();
    TIM0_overflow_16ms();
    TIM0_overflow_interrupt_enable();
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {

    }

    return 0;
}


ISR(TIMER0_OVF_vect)
{

  char string[4];
  aState = GPIO_read(&PINC,OutputCLK);

  lcd_gotoxy(0, 0);
  lcd_puts("Cntr:");

  itoa(counter, string, 10);

  /* Encoder reading cycle ----------------------------------------------*/
  if (aState != aLastState && aState == 1){

      if (GPIO_read(&PIND,OutputDT) != aState) {
      counter ++;
      }
      
      else {
      counter --;
      }

      if(counter>10)
        counter = 10;
      else if(counter<-10)
        counter = -10;

      itoa(counter, string, 10);
      lcd_gotoxy(5, 0);
      lcd_puts("     ");
      lcd_gotoxy(5, 0);
      lcd_puts(string);
  }

  aLastState = aState;

  /* Encoder reset button ----------------------------------------------*/
  button = GPIO_read(&PIND,OutputSW);

    if (button == 0){

        counter = 0;

        itoa(counter, string, 10);
        lcd_gotoxy(5, 0);
        lcd_puts("     ");
        lcd_gotoxy(5, 0);
        lcd_puts(string);
    }

}

ISR(TIMER1_OVF_vect)
{
  /* ADC conversion */
  ADCSRA |= (1<<ADSC);
}


ISR(ADC_vect)
{
  char charX[4];
  char charY[4];
  uint16_t valueX;
  uint16_t valueY;
  uint8_t channel = 0;


  static int8_t nooverflow = 0;
  nooverflow++;
  if(nooverflow > 3)
  {
    nooverflow = 0;
    internalCounter++;
  }
  
  /* address shifting ADMUX -----------------------------------------*/

  if (channel == 0){
    // Select input channel ADC0 (voltage divider pin)
    ADMUX = ADMUX & ~( 1<<MUX3 | 1<<MUX2 | 1<<MUX0 | 1<<MUX1);
    ADCSRA |= (1<<ADSC);
    valueX = ADC;

    itoa(valueX, charX, 10);
    lcd_gotoxy(0,1);
    lcd_puts("       ");
    lcd_gotoxy(0,1);
    lcd_puts("X:");
    lcd_gotoxy(2,1);
    lcd_puts(charX);

    channel = 1;
  }
  if (channel == 1){
    // Select input channel ADC1 (voltage divider pin)
    ADMUX &= ~((1<<MUX3 | 1<<MUX2 | 1<<MUX1)); ADMUX |= (1<<MUX0);
    ADCSRA |= (1<<ADSC);
    valueY = ADC;
    
    itoa(valueY, charY, 10);
    lcd_gotoxy(7,1);
    lcd_puts("       ");
    lcd_gotoxy(7,1);
    lcd_puts("Y:");
    lcd_gotoxy(9,1);
    lcd_puts(charY);

    channel = 0;
  }
  
  /* Turning all LEDs OFF ------------------------------------------------*/
  GPIO_write_high(&PORTB,LED1);
  GPIO_write_high(&PORTB,LED2);
  GPIO_write_high(&PORTB,LED3);
  GPIO_write_high(&PORTB,LED4);
  GPIO_write_high(&PORTC,LED5);
  
  
  /* Blinking LEDS and fucntions -----------------------------------------*/
  if(valueX > 800)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
          GPIO_write_low(&PORTC,LED5);
        }
        else if(i == 0){
         GPIO_write_low(&PORTC,LED5);
        } 
    }
    }    
      lcd_gotoxy(11, 0);
      lcd_puts("      ");
      lcd_gotoxy(11, 0);
      lcd_puts("LED_5");
      
    }

  else if (valueX > 600)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
          GPIO_write_low(&PORTB,LED4);
        }
        else if(i == 0){
          GPIO_write_low(&PORTB,LED4);
        } 
      }
      }
    lcd_gotoxy(11, 0);
    lcd_puts("      ");
    lcd_gotoxy(11, 0);
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
        } 
    }
    }
    lcd_gotoxy(11, 0);
    lcd_puts("      ");
    lcd_gotoxy(11, 0);
    lcd_puts("LED_3");
  }

  else if (valueX > 200)
  {
    for(uint8_t i = 0; i<11; i++){  
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
          GPIO_write_low(&PORTB,LED2);
        } 
        else if(i == 0){
          GPIO_write_low(&PORTB,LED2);
        }
    }
    }
    lcd_gotoxy(11, 0);
    lcd_puts("      ");
    lcd_gotoxy(11, 0);
    lcd_puts("LED_2");
  }

  else if (valueX >= 0)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
            GPIO_write_low(&PORTB,LED1);
        }
        else if(i == 0){
          GPIO_write_low(&PORTB,LED1);
        } 
    }
    }
    lcd_gotoxy(11, 0);
    lcd_puts("      ");
    lcd_gotoxy(11, 0);
    lcd_puts("LED_1");
  }

  if(valueY < 80)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
          
          GPIO_write_low(&PORTC,LED5);
          GPIO_write_low(&PORTB,LED4);
        }
        else if(i == 0){
          
          GPIO_write_low(&PORTC,LED5);
          GPIO_write_low(&PORTB,LED4);
        }
        }
    }
    }

    if(valueY > 940)
  {
    for(uint8_t i = 0; i<11; i++){
      if(abs(counter)==i){
        if(internalCounter%(11-i) == 0){
         
          GPIO_write_low(&PORTB,LED2);
          GPIO_write_low(&PORTB,LED1);
        }
        else if(i == 0){
          
          GPIO_write_low(&PORTB,LED2);
          GPIO_write_low(&PORTB,LED1);
        }
        }
    }
    }       
      
  buttonJOY = GPIO_read(&PINC,OutputSWJ);

    if (buttonJOY == 0){
      for(uint8_t i = 0; i<11; i++){
        if(abs(counter)==i){
          if(internalCounter%(11-i) == 0){
            
            GPIO_write_low(&PORTC,LED5);
            GPIO_write_low(&PORTB,LED4);
            GPIO_write_low(&PORTB,LED3);
            GPIO_write_low(&PORTB,LED2);
            GPIO_write_low(&PORTB,LED1);
          }
          else if(i == 0){
            
            GPIO_write_low(&PORTC,LED5);
            GPIO_write_low(&PORTB,LED4);
            GPIO_write_low(&PORTB,LED3);
            GPIO_write_low(&PORTB,LED2);
            GPIO_write_low(&PORTB,LED1);
          }
        }
      }

    }    
    
}