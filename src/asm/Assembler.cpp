#include "asm/assembler.h"
#include "asm/directive.h"
#include "core/instruction_set.h"

#include <algorithm>
#include <ios>
#include <iostream>
#include <sstream>

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

    static inline std::vector<std::string> tokenize(const std::string& text)
    {
        std::string copy = text;
        std::replace(copy.begin(), copy.end(), ',', ' ');

        std::istringstream buffer(text);
        std::vector<std::string> result;
        std::string s;

        while (buffer >> s)
        {
            result.push_back(s);
        }

        return result;
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

            strip_comment(line);
            trim(line);
            std::vector<std::string> tokens = tokenize(line);

            if (tokens.empty())
            {
                continue;
            }

            uint8_t current_token = 0;

            if (tokens.at(current_token).ends_with(':'))
            {
                // We have a label
                std::string label_token = tokens.at(current_token);

                std::string label = label_token.substr(0, label_token.size() - 1);
                auto result = this->symbol_table.try_emplace(label, this->memory_location);
                if (!result.second)
                {
                    std::cerr << "Duplicate label \"" << label << "\" on line " << this->line_counter << std::endl;
                    return false;
                }

                if (this->verbose)
                {
                    std::cout << "Symbol \"" << label << "\" added to symbol table at memory location " << this->memory_location << std::endl;
                }

                ++current_token;
            }

            if (tokens.at(current_token).starts_with('.'))
            {
                std::string directive_token = tokens.at(current_token);
            }
        }

        return true;
    }
}
