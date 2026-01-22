#include "asm/assembler.h"

#include <filesystem>
#include <iostream>
#include <string>

static void print_error(const std::string& error)
{
    std::cerr << "vv-asm: " << error << std::endl;
    std::cerr << "Use vv-asm -h to see usage." << std::endl;
}

static void print_help()
{
    std::cout << "Usage: vv-asm executable-path [commands]" << std::endl;
    std::cout << std::endl;
    std::cout << "asm-file-path" << std::endl;
    std::cout << "  The path to a .vvasm assembly file." << std::endl;
    std::cout << std::endl;
    std::cout << "commands:" << std::endl;
    std::cout << "  -h|--help\t\tDisplay help." << std::endl;
    std::cout << "  -v|--verbose\t\tOutput progress of assembler." << std::endl;
    std::cout << "  -o OUTPUT-NAME\tSpecify the output file name." << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_error("You must provide a command or path.");
        return -1;
    }

    std::string source;
    std::string output;
    bool verbose_flag = false;
    bool found_source = false;
    bool output_flag = false;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = std::string(argv[i]);
        if (arg == "-h" || arg == "--help")
        {
            print_help();
            return 0;
        }

        else if (arg == "-v" || arg == "--verbose")
        {
            verbose_flag = true;
        }

        else if (arg == "-o")
        {
            output_flag = true;
            if (i + 1 < argc)
            {
                output = std::string(argv[i + 1]);
                ++i;
            }

            else
            {
                print_error("You must provide an output file name for the -o command.");
                return -1;
            }
        }

        else
        {
            if (found_source)
            {
                print_error("More than one non-command argument provided. Please only provide one path.");
                return -1;
            }

            source = arg;
            found_source = true;
        }
    }

    if (!found_source)
    {
        print_error("You must provide the path to a .vvasm assembly file.");
        return -1;
    }

    // Verify the path
    std::filesystem::path source_path = source;
    if (!std::filesystem::exists(source_path) || !std::filesystem::is_regular_file(source_path))
    {
        print_error("The file you provided does not exist.");
        return -1;
    }

    if (source_path.extension().string() != ".vvasm")
    {
        print_error("The file you provided is not a .vvasm file.");
        return -1;
    }

    // Setup the output path
    std::filesystem::path output_path;
    std::filesystem::path binary_extension = ".vv";

    if (output_flag)
    {
        output_path = output;
    }

    else
    {
        output_path = std::filesystem::path(source_path).replace_extension(binary_extension);
    }

    vv::assembler::Assembler assembler(source_path, output_path, verbose_flag);
    if (assembler.assemble())
    {
        return 0;
    }

    else
    {
        return -1;
    }
}
