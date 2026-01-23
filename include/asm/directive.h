#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include <cstdint>

namespace vv::assembler
{
    enum class Directive : uint8_t
    {
#define Y(name, enum_val) enum_val,
#include "asm/directive.def"
#undef Y
    };
}

#endif
