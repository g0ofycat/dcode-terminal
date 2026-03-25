#include <iostream>

#include "../src/parser/parser.hpp"
#include "../src/parser/flags/valid_flags.hpp"
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
        "Flags:\n";

    for (const auto& f : VALID_FLAGS)
    {
        std::string col = "  -" + std::string(f.name);
        if (!f.usage_hint.empty())
            col += " " + std::string(f.usage_hint);

        constexpr int COL_WIDTH = 20;
        if ((int)col.size() < COL_WIDTH)
            col += std::string(COL_WIDTH - col.size(), ' ');

        std::cout << col << f.description << "\n";
    }

    std::cout <<
        "\nRules:\n"
        "  last --e/--d wins\n"
        "  op=e|d overrides mode\n"
        "  order of input/ops doesn't matter\n\n";
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
