/*
Instructions (32 bits):
    OP: 8 bits
    RA: 4 bits
    RB: 4 bits
    Immediate/address: 16 bits

OP RA (RB | Immediate/address)
*/

#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <cstdint>

namespace vv::core
{
    enum class Opcode : uint8_t
    {
#define X(name, enum_val, hex) enum_val = hex,
#include "core/instruction_set.def"
#undef X
    };

    const uint32_t MASK_OP = 0xFF000000;
    const uint32_t MASK_RA = 0x00F00000;
    const uint32_t MASK_RB = 0x000F0000;
    const uint32_t MASK_IM = 0x0000FFFF;
}

#endif
