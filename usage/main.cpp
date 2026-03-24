#include <iostream>

#include "../src/parser/parser.hpp"
#include "../src/executor/executor.hpp"
#include "../src/errors/error.hpp"

/// @brief Standard print with 1 argument
static void print_usage()
{
    std::cout <<
        "dcode - data encoder/decoder\n"
        "\n"
        "Usage:\n"
        "  dcode -i <input> [ops...] [--e|--d]\n"
        "\n"
        "Ops:\n"
        "  -b64        base64\n"
        "  -r          reverse\n"
        "\n"
        "Modes:\n"
        "  --e         encode (default)\n"
        "  --d         decode\n"
        "\n"
        "Rules:\n"
        "  last --e/--d wins\n"
        "  op=e|d overrides mode\n"
        "  order of input/ops doesn't matter\n"
        "\n";
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
