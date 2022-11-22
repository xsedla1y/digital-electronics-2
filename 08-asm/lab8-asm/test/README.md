# Lab 8: INSERT_YOUR_FIRSTNAME INSERT_YOUR_LASTNAME

### Instruction set

1. Complete the conversion table with selected instructions:

   | **Instruction** | **Binary opcode** | **Hex opcode** | **Compiler Hex opcode** |
   | :-- | :-: | :-- | :-: |
   | `add r24, r0` |  |  |  |
   | `com r26` |  |  |  |
   | `eor r26, r27` |  |  |  |
   | `mul r22, r20` |  |  |  |
   | `ret` | `1001_0101_0000_1000` | 9508 |  |

### 4-bit LFSR

2. Complete table with 4-bit LFSR values for different Tap positions:

   | **Tap position** | **Generated values** | **Length** |
   | :-: | :-- | :-: |
   | 4, 3 |  |  |
   | 4, 2 |  |  |
   | 4, 1 |  |  |

### Variable number of short pulses

3. Draw a flowchart of function `void burst_c(uint8_t number)` which generates a variable number of short pulses at output pin. Let the pulse width be the shortest one. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![your figure]()

4. Go through the `main.c` file and make sure you understand each line. Use **AVR® Instruction Set Manual** from Microchip [Online Technical Documentation](https://onlinedocs.microchip.com/), find the description of instructions used in `mac.S`, and complete the table.

   | **Instruction** | **Operation** | **Description** | **Cycles** |
   | :-- | :-: | :-- | :-: |
   | `add Rd, Rr` |  |  |  |
   | `mul Rd, Rr` |  |  |  |
   | `ret` |  |  |  |

5. Use manual's 16-bit Opcodes and convert used instructions to hexadecimal.

   | **Instruction** | **Binary opcode** | **Hex opcode** | **Compiler Hex opcode** |
   | :-- | :-: | :-: | :-: |
   | `add r24, r0` |  |  |  |
   | `mul r22, r20` |  |  |  |
   | `ret` | `1001_0101_0000_1000` | 9508 |  |