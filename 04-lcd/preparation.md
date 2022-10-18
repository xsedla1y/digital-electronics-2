<a name="preparation"></a>

## Pre-Lab preparation - ONDŘEJ SEDLÁK

1. Use schematic of the [LCD keypad shield](https://oshwlab.com/tomas.fryza/arduino-shields) and find out the connection of LCD display. What data and control signals are used? What is the meaning of these signals?

   | **LCD signal(s)** | **AVR pin(s)** | **Description** |
   | :-: | :-: | :-- |
   | RS | PB0 | Register selection signal. Selection between Instruction register (RS=0) and Data register (RS=1) |
   | R/W | -- | Permanently conected to GND |
   | E | PB1 | Enable. This loads data into the HD44780 on the falling edge. |
   | D[3:0] | -- | Not used, for 8-bit LCD operation. |
   | D[7:4] | PB[7:4] | Upper nibble used in 4-bit mode.  |
   | K | PB2 | Backlight cathode. |

2. What is the ASCII table? What are the codes/values for uppercase letters `A` to `Z`, lowercase letters `a` to `z`, and numbers `0` to `9` in this table?

   | **Char** | **Decimal** | **Hexadecimal** |
   | :-: | :-: | :-: |
   | `A` | 65 | 0x41 |
   | `B` | 66 | 0x42 |
   | ... |  |  |
   | `a` | 97 | 0x61 |
   | `b` | 98 | 0x62 |
   | ... |  |  |
   | `0` | 48 | 0x30 |
   | `1` | 49 | 0x31 |
   | ... |  |  |
