#include <iostream>

#include "../src/parser/parser.hpp"
#include "../src/executor/executor.hpp"
#include "../src/errors/error.hpp"

/// @brief Standard print with 1 argument
static void print_usage()
{
    std::cout <<
        "usage: dcode -i <input> [-e|-d] <ops...>\n"
        "\n"
        "mode flags:\n"
        "  -e           encode\n"
        "  -d           decode\n"
        "\n"
        "pipeline ops:\n"
        "  -b64         base64\n"
        "  -r           reverse\n"
        "\n"
        "examples:\n"
        "  dcode -i \"hello world\" -e -b64\n"
        "  dcode -i \"aGVsbG8gd29ybGQ=\" -d -b64\n"
        "  dcode -i \"hello\" -e -b64 -r\n";
}

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        print_usage();
        return 0;
    }

    try
    {
        Parser parser(argc, argv);
        ParsedArgs result = parser.parse();

        Executor executor(result);
        std::cout << executor.run() << "\n";
    }
    catch (const DcodeError& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
