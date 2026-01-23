#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <asm/directive.h>
#include "core/instruction_set.h"

#include <deque>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <string>

namespace vv::assembler
{
    class Assembler
    {
    public:
        Assembler(const std::filesystem::path& source, const std::filesystem::path& output, const bool verbose);

        using HandlerFunction = void (Assembler::*)(const bool first_pass, std::deque<std::string>& operands);

        struct DirectiveInfo
        {
            Directive directive;
            HandlerFunction handler;
        };

        bool assemble();

    private:
        std::filesystem::path source;
        std::filesystem::path output;
        bool verbose;

        uint32_t memory_location = 0;
        uint32_t line_counter = 0;
        bool had_error = false;

        std::unordered_map<std::string, uint32_t> symbol_table;
        static std::unordered_map<std::string, DirectiveInfo> directive_handlers;

        inline void error_occurred(const std::string& message);
        inline void print_verbose(const std::string& message, const bool line = false) const;

        void make_pass(std::ifstream& asm_file, const bool first_pass);

        // Directive handlers
        void handle_org(const bool first_pass, std::deque<std::string>& operands);
        void handle_dat(const bool first_pass, std::deque<std::string>& operands);
        void handle_byt(const bool first_pass, std::deque<std::string>& operands);
        void handle_str(const bool first_pass, std::deque<std::string>& operands);
        void handle_res(const bool first_pass, std::deque<std::string>& operands);
        void handle_ent(const bool first_pass, std::deque<std::string>& operands);
        void handle_equ(const bool first_pass, std::deque<std::string>& operands);
    };
}

#endif
