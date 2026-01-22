#ifndef ASSEMBLER_H
#define ASSEMBLER_H

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

        bool assemble();

    private:
        std::filesystem::path source;
        std::filesystem::path output;

        std::unordered_map<std::string, uint32_t> symbol_table;

        uint32_t memory_pointer = 0;
        uint32_t line_counter = 0;
        bool verbose;

        bool first_pass(std::ifstream& asm_file);
    };
}

#endif
