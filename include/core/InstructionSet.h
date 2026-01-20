#include <cstdint>

/*
Instructions (32 bits):
    OP: 8 bits
    RA: 4 bits
    RB: 4 bits
    Immediate/address: 16 bits

OP RA (RB | Immediate/address)
*/

namespace VirtualVoid::Core
{
    public enum Opcode : uint8_t
    {
        // Memory
        SET = 0x01 // Load immediate value into a register
        MOV = 0x02 // Copy value from RB to RA
        LDR = 0x03 // Load value from memory address (in RB) into RA
        STR = 0x04 // Store value from RA into memory address (in RB)
        PUSH = 0x05 // Push RA onto the stack (decrements SP first)
        POP = 0x06 // Pop from the stack into RA (load, then increment SP)

        // Math and Logic
        ADD = 0x10 // RA = RA + RB
        ADDI = 0x11 // RA = RA + Immediate
        SUB = 0x12 // RA = RA - RB
        SUBI = 0x13 // RA = RA - Immediate
        MUL = 0x14 // RA = RA * RB
        DIV = 0x15 // RA = RA * RB
        AND = 0x16 // RA = RA & RB
        OR = 0x17 // RA = RA | RB
        XOR = 0x18 // RA = RA ^ RB
        SHL = 0x19 // RA = RA << RB
        SHR = 0x20 // RA = RA >> RB

        // Control Flow
        CMP = 0x21 // RA == RB, sets zero flag
        CMPI = 0x22 // RA == Immediate, sets zero flag 
        JMP = 0x23 // Unconditional jump to address
        JZ = 0x24,// Jump to address if zero flag set (A == B)
        JNZ = 0x25 // Jump to address if zero Flag is not set (A != B)
        JLT = 0x26 // Jump to address if sign Flag is set (A < B)
        CALL = 0x27 // Push PC to stack and jump to address.
        RET = 0x28 // Pop from stack into PC.

        // System & I/O
        HALT = 0x00 // Stop execution
        IN = 0x30 // Read from I/O port(RB) into RA.
        OUT = 0x31 // Write value in RA to I/O port(RB).
        WAIT = 0x32 // Pause execution for RA milliseconds.
    };

    const uint8_t MASK_OP = 0xFF000000;
    const uint8_t MASK_RA = 0x00F00000;
    const uint8_t MASK_RB = 0x000F0000;
    const uint8_t MASK_IM = 0x0000FFFF;
}
