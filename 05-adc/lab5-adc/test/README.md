# Lab 5: ONDŘEJ SEDLÁK

### Analog-to-Digital Conversion

1. Complete table with voltage divider, calculated, and measured ADC values for all five push buttons.

   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** | **ADC value (measured, hex)** |
   | :-: | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0   | 0 | 0 |
   | Up     | 0.495&nbsp;V | 101 | 100 | 64 |
   | Down   | 1.203&nbsp;V | 246 | 257 | 101 |
   | Left   | 1.967&nbsp;V | 402 | 410 | 19A |
   | Select | 3.181&nbsp;V | 651 | 640 | 280 |
   | none   | 5&nbsp;V | 1023 | 1023 | 3FF |
   
   | **Operation** | **Register(s)** | **Bit(s)** | **Description** |
   | :-- | :-: | :-: | :-- |
   | Voltage reference    | ADMUX | REFS1:0 | 00: AREF, Internal Vref turned off, 01: AVcc voltage reference (5V), 10: Reserved, 11: Internal 1.1V Voltage Reference with external capacitor at AREF pin |
   | Input channel        | ADMUX | MUX3:0 | 0000: ADC0, 0001: ADC1, 0010: ADC2, 0011: ADC3, 0100: ADC4, 0101: ADC5, 0110: ADC6, 0111: ADC7, 1000: ADC8 |
   | ADC enable           | ADCSRA | ADEN bit7 | 1: enables the ADC. 0: the ADC is turned off. Turning the ADC off while aconversion is in progress, will terminate this conversion. |
   | Start conversion     | ADCSRA | ADSC bit6 | Single Conversion mode: start each conversion. Free Running mode: start the first conversion. ADSC will read as 1 as long as a conversion is in progress. When the conversion is complete, it returns to 0. Writing 0 to this bit has no effect. |
   | ADC interrupt enable | ADCSRA | ADIE bit3 | When this bit is written to 1 and the I-bit in SREG is set, the ADC Conversion Complete Interrupt is activated. |
   | ADC clock prescaler  | ADCSRA | ADPS2:0 | 000: Division factor 2, 001:2, 010:4, 011:8, 100:16, 101:32, 110:64, 111:128|
   | ADC 10-bit result    | ADLAR1:0 | ADC9:0 (H/L) |  |

### Temperature meter

Consider an application for temperature measurement. Use analog temperature sensor [TC1046](http://ww1.microchip.com/downloads/en/DeviceDoc/21496C.pdf), LCD, and a LED. Every 30 seconds, the temperature is measured and the value is displayed on LCD screen. When the temperature is too high, the LED will turn on.

2. Draw a schematic of temperature meter. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![your figure](https://github.com/xsedla1y/digital-electronics-2/blob/main/05-adc/05-photos/schematic2_lab5.png)

3. Draw two flowcharts for interrupt handler `TIMER1_OVF_vect` (which overflows every 1&nbsp;sec) and `ADC_vect`. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![your figure]()
