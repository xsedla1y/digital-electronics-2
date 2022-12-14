# Lab 1: Ondřej Sedlák

### Morse code

1. Listing of C code which repeats one "dot" and one "comma" (BTW, in Morse code it is letter `A`) on a LED. Always use syntax highlighting, meaningful comments, and follow C guidelines:

```c
int main(void)
{
    uint8_t led_value = LOW;  // Local variable to keep LED status

    // Set pin where on-board LED is connected as output
    pinMode(LED_GREEN, OUTPUT);

    // Infinite loop - A letter to morse code

    /// PREDEFINED ///
    //#define SHORT_DELAY 250 // Delay in milliseconds
    //#define DELAY 500
    //#define LONG_DELAY 1000

    while (1)
    {
        led_value = HIGH;
        digitalWrite(LED_GREEN, led_value);
        _delay_ms(SHORT_DELAY);

        led_value = LOW;
        digitalWrite(LED_GREEN, led_value);
        _delay_ms(DELAY);

        led_value = HIGH;
        digitalWrite(LED_GREEN, led_value);
        _delay_ms(LONG_DELAY);

        led_value = LOW;
        digitalWrite(LED_GREEN, led_value);
        _delay_ms(DELAY);
        _delay_ms(LONG_DELAY);

    }

    // Will never reach this
    return 0;
}
```

2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure](https://github.com/xsedla1y/digital-electronics-2/blob/42afcea66525adbcfd95ea479622e0213703e4c5/01-tools/01-photos/atmega_01-tools.png)
