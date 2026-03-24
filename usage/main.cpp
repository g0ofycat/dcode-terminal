#include <iostream>

#include "../src/parser/parser.hpp"
#include "../src/executor/executor.hpp"
#include "../src/errors/error.hpp"

/// @brief Standard print with 1 argument
static void print_usage()
{
    std::cout <<
        "usage: dcode -i <input> <ops...> [--e|--d]\n"
        "\n"
        "notes: last flags for encoding and decoding override normal ops (default is encoding). you can specify if you want encoded or decoded ops manually by doing something like -b64=e OR -b64=d and etc. input and ops position are interchangeable (they can be anywhere). manual coding ops for specific flags override the last flag [--e|--d]"
        "\n"
        "mode flags:\n"
        "  --e          encode\n"
        "  --d          decode\n"
        "\n"
        "pipeline ops:\n"
        "  -b64         base64\n"
        "  -r           reverse\n"
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
