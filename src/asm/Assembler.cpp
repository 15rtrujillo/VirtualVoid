#include "asm/assembler.h"

#include <algorithm>
#include <cstdint>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

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

    static inline std::deque<std::string> tokenize(const std::string& text)
    {
        std::string copy = text;
        std::replace(copy.begin(), copy.end(), ',', ' ');

        std::istringstream buffer(copy);
        std::deque<std::string> result;
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

    std::unordered_map<std::string, Assembler::DirectiveInfo> Assembler::directive_handlers = {
#define X(name, enum_val, handler_func) { name, { Directive::enum_val, &Assembler::handler_func } },
#include "asm/directive.def"
#undef X
    };

    bool Assembler::assemble()
    {
        std::ifstream asm_file(this->source);
        if (!asm_file.is_open())
        {
            std::cerr << "There was an error opening " << this->source.string() << std::endl;
            return false;
        }

        if (this->verbose)
        {
            print_verbose("Beginning first pass to build symbol table...");
        }

        make_pass(asm_file, true);

        if (this->had_error)
        {
            std::cerr << "[ERROR] An error occurred during the first assembler pass." << std::endl;
            return false;
        }

        asm_file.clear();
        asm_file.seekg(0, std::ios::beg);

        return true;
    }

    inline void Assembler::error_occurred(const std::string& message)
    {
        std::cerr << "[ERROR] Line: " << this->line_counter << ": " << message << std::endl;
        had_error = true;
    }

    inline void Assembler::print_verbose(const std::string& message, const bool line) const
    {
        if (line)
        {
            std::cout << "[VERBOSE] Line: " << this->line_counter << ": " << message << std::endl;
        }

        else
        {
            std::cout << "[VERBOSE] " << message << std::endl;
        }
    }

    void Assembler::make_pass(std::ifstream& asm_file, const bool first_pass)
    {
        std::string line;
        while (std::getline(asm_file, line))
        {
            ++this->line_counter;

            strip_comment(line);
            trim(line);
            std::deque<std::string> tokens = tokenize(line);

            if (tokens.empty())
            {
                continue;
            }

            if (tokens.front().ends_with(':'))
            {
                // We have a label
                std::string label_token = tokens.front();
                tokens.pop_front();

                if (!first_pass)
                {
                    return;
                }

                std::string label = label_token.substr(0, label_token.size() - 1);
                auto result = this->symbol_table.try_emplace(label, this->memory_location);
                if (!result.second)
                {
                    error_occurred("Duplicate label \"" + label + "\"");
                }

                if (this->verbose)
                {
                    std::ostringstream oss;
                    oss << "Symbol \"" << label << "\" added to symbol table and is mapped to memory location " << this->memory_location;
                    print_verbose(oss.str());
                }
            }

            if (tokens.front().starts_with('.'))
            {
                std::string directive_token = tokens.front();
                tokens.pop_front();

                DirectiveInfo directive_info = this->directive_handlers.at(directive_token);
                if (directive_info.directive == Directive::STR)
                {
                    std::deque<std::string> arg = { line };
                    (this->*directive_info.handler)(first_pass, arg);
                }

                else
                {
                    (this->*directive_info.handler)(first_pass, tokens);
                }
            }
        }
    }

    void Assembler::handle_org(const bool first_pass, std::deque<std::string>& operands)
    {
        print_verbose("ORG directive:", true);
        if (operands.size() != 1)
        {
            error_occurred("Invalid number of operands. Expected 1: Base-16 memory address");
            return;
        }

        unsigned long address;
        try
        {
            address = std::stoul(operands.front(), nullptr, 16);
        }
        catch (std::invalid_argument)
        {
            error_occurred("Operand is not a valid base-16 number.");
            return;
        }
        catch (std::out_of_range)
        {
            error_occurred("Operand is out of range.");
            return;
        }

        if (static_cast<long>(address) < 0)
        {
            error_occurred("Operand is out of range.");
            return;
        }

        if (this->verbose)
        {
            print_verbose("\tMemory pointer moved to " + operands.front());
        }

        else if (!first_pass)
        {
            // TODO: Pad binary file with zeros until we reach new ORG
        }

        this->memory_location = address;
    }

    void Assembler::handle_dat(const bool first_pass, std::deque<std::string>& operands)
    {
        if (this->verbose)
        {
            print_verbose("DAT directive:", true);
        }

        for (const std::string& operand : operands)
        {
            int32_t value;
            try
            {
                value = std::stoi(operand, nullptr, 0);
            }
            catch (std::invalid_argument)
            {
                error_occurred("Operand \"" + operand + "\" could not be converted to integer.");
                return;
            }
            catch (std::out_of_range)
            {
                try
                {
                    value = static_cast<int32_t>(std::stoul(operand, nullptr, 0));
                }
                catch (std::out_of_range)
                {
                    error_occurred("Operand \"" + operand + "\" is out of range.");
                    return;
                }
            }

            if (first_pass && this->verbose)
            {
                print_verbose("\tAdvancing memory pointer 4 bytes.");
            }

            else if (!first_pass)
            {
                // TODO: Write to memory buffer
            }

            this->memory_location += sizeof(value);
        }
    }

    void Assembler::handle_byt(const bool first_pass, std::deque<std::string>& operands)
    {
        if (this->verbose)
        {
            print_verbose("BYT directive:", true);
        }

        for (const std::string& operand : operands)
        {
            uint8_t value;

            // Check for char literal
            if (operand.starts_with('\''))
            {
                // Char literal should have at least three characters ', [char], and '
                if (operand.size() < 3)
                {
                    error_occurred("Invalid char literal: " + operand);
                    return;
                }

                // Check for escape sequence literal
                if (operand.at(1) == '\\')
                {
                    // Escape sequence should have 4 chars. ', \, [sequence], and '
                    if (operand.size() > 4)
                    {
                        error_occurred("Invalid char literal: " + operand);
                        return;
                    }

                    switch (operand.at(2))
                    {
                        case '\\':
                            value = 0x5C;
                            break;
                        case 'a':
                            value = 0x07;
                            break;
                        case 'b':
                            value = 0x08;
                            break;
                        case 'f':
                            value = 0x0C;
                            break;
                        case 'n':
                            value = 0x0A;
                            break;
                        case 'r':
                            value = 0x0D;
                            break;
                        case 't':
                            value = 0x09;
                            break;
                        case 'v':
                            value = 0x0B;
                            break;
                        default:
                            error_occurred("Operand \"" + operand + "\" is not a valid escape sequence.");
                            return;
                    }
                }

                // Regular char literal
                else
                {
                    // Regular char should have 3 chars. ', [char], and '
                    if (operand.size() > 3)
                    {
                        error_occurred("Invalid char literal: " + operand);
                        return;
                    }

                    value = operand.at(1);
                }
            }

            // We likely have a numeric value
            else
            {
                int temp;
                try
                {
                    temp = std::stoi(operand, nullptr, 0);
                }
                catch (std::invalid_argument)
                {
                    error_occurred("Operand \"" + operand + "\" could not be converted to integer.");
                    return;
                }

                // Check if the value is valid for a char/unsigned char
                if (temp < INT8_MIN || temp > UINT8_MAX)
                {
                    error_occurred("Operand \"" + operand + "\" is out of range.");
                    return;
                }

                value = static_cast<uint8_t>(temp);
            }

            if (first_pass && this->verbose)
            {
                print_verbose("\tAdancing memory pointer 1 byte.");
            }

            else if (!first_pass)
            {
                // TODO: Write to memory buffer
            }

            this->memory_location += sizeof(value);
        }
    }

    void Assembler::handle_str(const bool first_pass, std::deque<std::string>& operands)
    {

    }

    void Assembler::handle_res(const bool first_pass, std::deque<std::string>& operands)
    {

    }

    void Assembler::handle_ent(const bool first_pass, std::deque<std::string>& operands)
    {

    }

    void Assembler::handle_equ(const bool first_pass, std::deque<std::string>& operands)
    {

    }
}
