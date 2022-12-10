#include <Arduino.h>
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>
#include <lcd.h>            // Peter Fleury's LCD library

// JOYSTICK
// OutputVRX PC1
// OutputVRY PC0
#define OutputSWJ PD2

// Define Servo pins
#define servo_1 PB1
#define servo_2 PB2

// Servo default values
#define Servo_default 1500
#define Servo_left 800
#define Servo_right 2200
#define Servo_step 25

uint32_t servo1_position = Servo_default;
uint32_t servo2_position = Servo_default;

uint8_t overflow = 0;

int main(void) 
{
  GPIO_mode_output(&DDRB, servo_1);
  GPIO_mode_output(&DDRB, servo_2);
  GPIO_mode_input_pullup(&DDRD,OutputSWJ);
  
  // Initialize display
  lcd_init(LCD_DISP_ON);
  lcd_gotoxy(0, 0);
  
  // Configure Analog-to-Digital Convertion unit
  // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
  ADMUX |= (1<<REFS0);
  ADMUX &= ~(1<<REFS1); 
  // Select input channel ADC0 (voltage divider pin)
  ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0) );
  // Enable ADC module
  ADCSRA |= (1<<ADEN);
  // Enable conversion complete interrupt
  ADCSRA |= (1<<ADIE);
  // Set clock prescaler to 128
  ADCSRA |= (1<<ADPS2);
  ADCSRA |= (1<<ADPS1);
  ADCSRA |= (1<<ADPS0);


  // Set PWM registers, PWM mode
  TCCR1A &= ~((1 << COM1A0) | (1 << COM1B0));
  TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
  TCCR1B |= (1 << WGM13);
  // PWM period
  ICR1 = 20000;
  OCR1A = servo1_position;
  OCR1B = servo2_position;
  // Set prescalar
  TCCR1B |= (1 << CS11);


  // Set timer 0, enable interrupt
  TIM0_overflow_1ms();
  TIM0_overflow_interrupt_enable();
  sei();
  
  while(1)
  {

  }
  return 0;
}


ISR(TIMER0_OVF_vect)
{
  static int8_t nooverflow = 0;
  nooverflow++;
  if(nooverflow > 50)
  {
    nooverflow = 0;
    /* ADC conversion */
    ADCSRA |= (1 << ADSC);
  }
 
}


ISR(ADC_vect)
{
  char charX[4];
  char charY[4];
  uint16_t valueX;
  uint16_t valueY;
  uint8_t channel = 0;
    
  /* address shifting ADMUX -----------------------------------------*/
    
  if (channel == 0){
    // Select input channel ADC0 (voltage divider pin)
    ADMUX = ADMUX & ~( 1<<MUX3 | 1<<MUX2 | 1<<MUX0 | 1<<MUX1);
    ADCSRA |= (1<<ADSC);
    valueX = ADC;

    // Servo1 logic
    if (valueX > 800 && servo1_position <= Servo_right) 
    {
        servo1_position += Servo_step;
    }
    else if (valueX < 200 && servo1_position >= Servo_left)
    {
        servo1_position -= Servo_step;
    }
    itoa(valueX, charX, 10);
    lcd_gotoxy(0,1);
    lcd_puts("       ");
    lcd_gotoxy(0,1);
    lcd_puts("X:");
    lcd_gotoxy(2,1);
    lcd_puts(charX);

    lcd_gotoxy(0,0);
    lcd_puts("S1:");
    lcd_gotoxy(3,0);
    lcd_puts("    ");
    lcd_gotoxy(3,0);
    itoa(servo1_position, charX, 10);
    lcd_puts(charX);

    channel = 1;
    
  }
  if (channel == 1){
    // Select input channel ADC1 (voltage divider pin)
    ADMUX &= ~((1<<MUX3 | 1<<MUX2 | 1<<MUX1)); ADMUX |= (1<<MUX0);
    ADCSRA |= (1<<ADSC);
    valueY = ADC;
    
    // Servo2 logic
    if (valueY < 200 && servo2_position <= Servo_right) 
    {
        servo2_position += Servo_step;
    }
    else if (valueY > 800 && servo2_position >= Servo_left)
    {
        servo2_position -= Servo_step;
    }
    itoa(valueY, charY, 10);
    lcd_gotoxy(8,1);
    lcd_puts("       ");
    lcd_gotoxy(8,1);
    lcd_puts("Y:");
    lcd_gotoxy(10,1);
    lcd_puts(charY);

    lcd_gotoxy(8,0);
    lcd_puts("S2:");
    lcd_gotoxy(11,0);
    lcd_puts("    ");
    lcd_gotoxy(11,0);
    itoa(servo2_position, charY, 10);
    lcd_puts(charY);

    channel = 0;
    
  } 
    // Write servo values to registers
    OCR1A = servo1_position;
    OCR1B = servo2_position;
    
}