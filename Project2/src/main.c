#include <Arduino.h>
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>



#define sv_1 PB2
#define sv_2 PB3

uint8_t overflow = 0;

int main(void) 
{
  GPIO_mode_output(&DDRB, sv_1);
  GPIO_mode_output(&DDRB, sv_2);

  TCCR1B |= (1 << WGM12);
  TCCR1B &= ~(1 << WGM13);
  TCCR1A |= (1 << WGM10) | (1 << WGM11);

  TCCR1A |=  (1<<COM1A1);
  TCCR1A &= ~(1<<COM1A0);

  //TCCR1A=0b10100010;//COM1A0,COM1B0 are 0, COM1A1, COM1B1 are 0 0
  //also WGM10 are 0 and WGM11=1
  
  //TCCR1B &=~ ((1<<ICNC1)|(1<<ICES1));  
  TCCR1B |= (1<<CS10);
  TCCR1B &= ~(1<<CS11);
  TCCR1B &= ~(1<<CS12);
  // ICR1=20000;
 // DDRB |= (1<<DDB1);

  TIM1_overflow_262ms();
  TIM1_overflow_interrupt_enable();
  sei();
  //TCCR1B=0b00010010;//WGM13 is 1 and WGM12 is 0 with prescaler equal 8
  while(1)
  {

  }
  return 0;
}

ISR(TIMER1_OVF_vect)
{
  
  overflow++;
  if (overflow < 6)
  {
    OCR1A=1000;
  }
  else if (overflow > 6)
  {
    OCR1A = 2500;
    overflow = 0;
  }  
                //here you set the values for duty cycle and also angle
                //OCR1B=2000;//you can connect another servo here and control it
}