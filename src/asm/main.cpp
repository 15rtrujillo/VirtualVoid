#include <iostream>
#include <string>

static void print_error(std::string error)
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

    std::string path;
    std::string output;
    bool verboseFlag = false;
    bool foundPath = false;
    bool outputFlag = false;

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
            verboseFlag = true;
        }

        else if (arg == "-o")
        {
            outputFlag = true;
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
            if (foundPath)
            {
                print_error("More than one non-command argument provided. Please only provide one path.");
                return -1;
            }

            path = arg;
            foundPath = true;
        }
    }

    if (!foundPath)
    {
        print_error("You must provide the path to an assembly file.");
        return -1;
    }

    if (!outputFlag)
    {
        // Get output file name from asm file name
    }

    // Begin assembly

    return 0;
}
