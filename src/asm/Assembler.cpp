#include "asm/assembler.h"
#include "core/instruction_set.h"

#include <ios>
#include <iostream>

namespace vv::assembler
{
    static inline void strip_comment(std::string& text)
    {
        text.erase(std::find(text.begin(), text.end(), ';'), text.end());
    }

    static inline void trim(std::string& text)
    {
        // ltrim
        text.erase(text.begin(), std::find_if_not(text.begin(), text.end(), [](unsigned char ch) {
            return std::isspace(ch);
            }));

        // rtrim
        text.erase(std::find_if_not(text.rbegin(), text.rend(), [](unsigned char ch) {
            return std::isspace(ch);
            }).base(), text.end());
    }

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
            ++this->line_counter;

            std::cout << line << std::endl;
            strip_comment(line);
            std::cout << "No comment: " << line << std::endl;
            trim(line);
            std::cout << "No whitespace: " << line << std::endl;
        }

        return true;
    }
}
