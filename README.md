# Virtual Void
Virtual Void is a 32-bit virtual machine and assembler.

## Instruction Set
### Instructions
Instructions for Virutal Void are 32 bits.

#### Structure
Opcode RegisterA (RegisterB | Immediate/address)

##### Example - ADD
Store the sum of R1 and R2 in R1.

| Opcode | RA | RB | No Used |
| --- | --- | --- | --- |
| 00001010 | 0001 | 0002 | 0000000000000000 |

##### Example SET
Store the immediate value 39,350 in R1.

| Opcode | RA | Not Used | Immediate
| --- | --- | --- | --- |
| 00000001 | 0001 | 0000 | 1001100110110110 |

### Opcodes
#### Memory
| Opcode | Description |
| --- | --- |
| SET | Load immediate value into a register |
| MOV | Copy value from RB to RA |
| LDR | Load value from memory address (in RB) into RA |
| STR | Store value from RA into memory address (in RB) |
| PUSH | Push RA onto the stack (decrements SP first) |
| POP | Pop from the stack into RA (load, then increment SP) |

#### Math and Logic
| Opcode | Description |
| --- | --- |
| ADD | RA = RA + RB |
| ADDI | RA = RA + Immediate |
| SUB | RA = RA - RB |
| SUBI | RA = RA - Immediate |
| MUL | RA = RA * RB |
| DIV | RA = RA / RB |
| AND | RA = RA & RB |
| OR | RA = RA \| RB |
| XOR | RA = RA ^ RB |
| SHL | RA = RA << RB |
| SHR | RA = RA >> RB |

#### Control Flow
| Opcode | Description |
| --- | --- |
| CMP | RA == RB, sets zero flag |
| CMPI | RA == Immediate, sets zero flag  |
| JMP | Unconditional jump to address |
| JZ | Jump to address if zero flag set (A == B) |
| JNZ | Jump to address if zero Flag is not set (A != B) |
| JLT | Jump to address if sign Flag is set (A < B) |
| CALL | Push PC to stack and jump to address. |
| RET | Pop from stack into PC. |

#### System and I/O
| Opcode | Description |
| --- | --- |
| HALT | Stop execution |
| IN | Read from I/O port(RB) into RA. |
| OUT | Write value in RA to I/O port(RB). |
| WAIT | Pause execution for RA milliseconds. |
