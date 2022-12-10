#ifndef F_CPU
    #define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>
#include "lcd.h"
#include "timer.h"
#include "adc.h"
#include "uart.h"

#define ENCODER_CLK PB2
#define ENCODER_DT PB3

#define MAIN_MENU_N_OF_ITEMS 2
#define N_OF_RECORDS 10
#define RECORDS_PER_PAGE 2

#define BTN_DEBOUNCE_TIME_MS 40
#define ENCODER_DEBOUNCE_TIME_MS 10

#define SERVO_DEFAULT 1500
#define SERVO_MIN 600
#define SERVO_MAX 2400
#define SERVO_STEP 25


uint64_t millis();


// number of milliseconds since power-on
volatile uint64_t timer_millis = 0;

volatile uint32_t servo1_position = SERVO_DEFAULT;
volatile uint32_t servo2_position = SERVO_DEFAULT;

volatile bool btn_joystick_prev = true;
volatile uint64_t btn_joystick_prev_ms = 0;

int main(void)
{
    // initialize UART for debugging
    uart_init(UART_BAUD_SELECT(9600, F_CPU));


    // Enable A/D converter with internal reference, prescaler value 128 and interrupt after the conversion is completed
    adc_internal_ref();
    adc_select_channel(0);
    adc_enable();
    adc_enable_interrupt();
    adc_prescaler_128();

    // Timer for counting milliseconds since device power up
    TIM0_overflow_1ms();
    TIM0_overflow_interrupt_enable();


    DDRB |= (1 << PB1) | (1 << PB2);

    TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B |= (1 << WGM13);
    ICR1 = 20000;
    OCR1A = servo1_position;
    OCR1B = servo2_position;
    TCCR1B |= (1 << CS11);


    // Set encoder and joystick button pins (INT0/1) as inputs with pull-up resistor
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    // Set activation of both external interrupts to any logical change
    EICRA |= (1 << ISC00);
    // Enable external interrupts INT0 and INT1 
    EIMSK |= (1 << INT0);


    // enable interrupts
    sei();

    // Ininite loop
    while (1)
    {
        
    }

    return 0;
}

// Returns number of milliseconds since power-up
uint64_t millis()
{
    // disable interrupts
    cli(); // prevents the timer from changing the value while reading the variable
    uint64_t val = timer_millis;
    // enable interrupts
    sei();
    
    return val;
}



// Increments the number of milliseconds elapsed since the power-up
ISR(TIMER0_OVF_vect)
{
    timer_millis++;
    static uint8_t n;
    if (n >= 50) {
        n = 0;
        adc_start_conversion();
    }
    n++;
}

// Senses the joystick position
ISR(ADC_vect)
{

    uint16_t value;
    value = ADC;

    uint8_t channel = adc_get_current_channel();
    // Move the player according to sensed value from the current channel 
    switch (channel)
    {
        case 0:
            if (value > 750 && servo1_position <= SERVO_MAX) 
            {
                servo1_position += SERVO_STEP;
            }
            else if (value < 250 && servo1_position >= SERVO_MIN)
            {
                servo1_position -= SERVO_STEP;
            }

            // Switch ADC to the channel 1 and start the conversion again
            adc_select_channel(1);
            adc_start_conversion();
            break;
        
        case 1:
            if (value < 250 && servo2_position <= SERVO_MAX) 
            {
                servo2_position += SERVO_STEP;
            }
            else if (value > 750 && servo2_position >= SERVO_MIN)
            {
                servo2_position -= SERVO_STEP;
            }
            // Switch the ADC back to the channel 0
            adc_select_channel(0);
            break;
    }
    OCR1A = servo1_position;
    OCR1B = servo2_position;
}

ISR(INT0_vect)
{
    // read the current pin value (0 if pressed)
    bool btn_joystick = PIND & (1 << PD2);
    // check whether the button has been pushed and whether the passed time between the two interrupts was greater than BTN_DEBOUNCE_TIME_MS
    if (btn_joystick_prev != btn_joystick && btn_joystick == false && (millis() - btn_joystick_prev_ms) >= BTN_DEBOUNCE_TIME_MS)
    {
        servo1_position = SERVO_DEFAULT;
        servo2_position = SERVO_DEFAULT;
    }

    btn_joystick_prev = btn_joystick;
    btn_joystick_prev_ms = millis();
}