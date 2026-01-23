#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include <cstdint>

namespace vv::assembler
{
    enum class Directive : uint8_t
    {
#define X(name, enum_val, handler_func) enum_val,
#include "asm/directive.def"
#undef X
    };
}

#endif
