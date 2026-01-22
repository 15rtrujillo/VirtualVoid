#include "asm/assembler.h"

#include <ios>
#include <iostream>

namespace vv::assembler
{
    Assembler::Assembler(const std::filesystem::path& source, const std::filesystem::path& output, const bool verbose = false) : source(source), output(output), verbose(verbose)
    {

    }

    bool Assembler::assemble()
    {
        std::ifstream asm_file(this->source);
        if (!asm_file.is_open())
        {
            std::cerr << "There was an error opening " << this->source.string() << std::endl;
            return false;
        }

        if (!first_pass(asm_file))
        {
            return false;
        }

        return true;
    }

    bool Assembler::first_pass(std::ifstream& asm_file)
    {
        if (this->verbose)
        {
            std::cout << "Beginning first pass to build symbol table." << std::endl;
        }

        std::string line;
        while (std::getline(asm_file, line))
        {
            // Skip comments
            if (line.starts_with(";"))
            {
                ++this->line_counter;
                continue;
            }
        }

        return true;
    }
}
