<a name="preparation"></a>

## Pre-Lab preparation - ONDŘEJ SEDLÁK

1. Use schematic of the [LCD keypad shield](https://oshwlab.com/tomas.fryza/arduino-shields) and find out the connection of five push buttons: Select, Left, Up, Down, and Right.

   ![your figure](https://github.com/xsedla1y/digital-electronics-2/blob/main/05-adc/05-photos/schematic_lab5.png)

2. Remind yourself, what a [voltage divider](https://www.allaboutcircuits.com/tools/voltage-divider-calculator/) is.

   ![your figure](https://github.com/xsedla1y/digital-electronics-2/blob/main/05-adc/05-photos/equation.png)

   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** | **ADC value (measured, hex)** |
   | :-: | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0   | 0 | 0 |
   | Up     | 0.495&nbsp;V | 101 |  |  |
   | Down   | 1.203&nbsp;V | 246 |  |  |
   | Left   | 1.967&nbsp;V | 402 |  |  |
   | Select | 3.181&nbsp;V | 651 |  |  |
   | none   | 5&nbsp;V | 1023 |  |  |
   
 3. The operation with the AD converter is performed through ADMUX, ADCSRA, ADCL+ADCH, ADCSRB, and DIDR0 registers. See [ATmega328P datasheet](https://www.microchip.com/wwwproducts/en/ATmega328p) (**Analog-to-Digital Converter > Register Description**) and complete the following table.

   | **Operation** | **Register(s)** | **Bit(s)** | **Description** |
   | :-- | :-: | :-: | :-- |
   | Voltage reference    | ADMUX | REFS1:0 | 00: AREF, Internal Vref turned off, 01: AVcc voltage reference (5V), 10: Reserved, 11: Internal 1.1V Voltage Reference with external capacitor at AREF pin |
   | Input channel        | ADMUX | MUX3:0 | 0000: ADC0, 0001: ADC1, 0010: ADC2, 0011: ADC3, 0100: ADC4, 0101: ADC5, 0110: ADC6, 0111: ADC7, 1000: ADC8 |
   | ADC enable           | ADCSRA | ADEN bit7 | 1: enables the ADC. 0: the ADC is turned off. Turning the ADC off while aconversion is in progress, will terminate this conversion. |
   | Start conversion     | ADCSRA | ADSC bit6 | Single Conversion mode: start each conversion. Free Running mode: start the first conversion. ADSC will read as 1 as long as a conversion is in progress. When the conversion is complete, it returns to 0. Writing 0 to this bit has no effect. |
   | ADC interrupt enable | ADCSRA |  | When this bit is written to 1 and the I-bit in SREG is set, the ADC Conversion Complete Interrupt is activated. |
   | ADC clock prescaler  | ADCSRA | ADPS2:0 | 000: Division factor 2, 001:2, 010:4, 011:8, 100:16, 101:32, 110:64, 111:128|
   | ADC 10-bit result    |  |  |  |
