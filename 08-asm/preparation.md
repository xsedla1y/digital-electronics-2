## Pre-Lab preparation

1. Use **AVR® Instruction Set Manual** from Microchip [Online Technical Documentation](https://onlinedocs.microchip.com/), find the description of selected instructions, and complete the table.

   | **Instruction** | **Operation** | **Description** | **Cycles** |
   | :-- | :-: | :-- | :-: |
   | `add Rd, Rr` | Rd = Rd+Rr | Adds two registers and places the result in the register Rd | 1 |
   | `andi Rd, K` | Rd = Rd and K | Logical AND between register Rd and 8-bit constant K | 1 |
   | `bld Rd, b` | Rd(b) = b | Copies b bit in the status register to bit b in register Rd | 1 |
   | `bst Rd, b` | T = Rd(b) | Stores bit b from Rd to the T bit in SREG (Status Register) | 1 |
   | `com Rd` | Rd = 0xFF-Rd |Performs a One’s Complement of register Rd | 1 |
   | `eor Rd, Rr` | Rd = Rd xor Rr | Performs the logical XOR between the  register Rd and register Rr and places the result in the destination register Rd | 1 |
   | `mul Rd, Rr` | R1:R0 = Rd * Rr |Performs 8-bit × 8-bit → 16-bit unsigned multiplication. | 2 |
   | `pop Rd` | Rd = STACK | Loads register Rd with a byte from the STACK. The Stack Pointer is pre-incremented by 1 before the POP | 2 |
   | `push Rr` | STACK = Rr | Stores the contents of register Rr on the STACK. The Stack Pointer is post-decremented by 1 after the PUSH. | 2 |
   | `ret` | --- | Returns from the subroutine. The return address is loaded from the STACK. | 4 |
   | `rol Rd` | Rd<<1 | Shifts all bits in Rd one place to the left | 1 |
   | `ror Rd` | Rd>>1 | Shifts all bits in Rd one place to the right. | 1 |
