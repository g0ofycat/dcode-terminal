#include <iostream>
#include "../src/parser/parser.hpp"
#include "../src/errors/error.hpp"

int main(int argc, const char* argv[])
{
    try
    {
        Parser parser(argc, argv);
        ParsedArgs result = parser.parse();

        std::cout << "input: " << result.input.value_or("none") << "\n";
        std::cout << "subcommand: " << result.subcommand.value_or("none") << "\n";
        std::cout << "global_decode: " << result.global_decode << "\n";
        std::cout << "global_encode: " << result.global_encode << "\n";
        std::cout << "pipeline:\n";

        for (const auto& op : result.pipeline)
        {
            std::cout << "  -" << op.flag;
            if (op.mode == OpMode::Encode) std::cout << " [encode]";
            if (op.mode == OpMode::Decode) std::cout << " [decode]";
            std::cout << "\n";
        }
    }
    catch (const DcodeError& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
