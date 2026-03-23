#include <string_view>
#include <vector>

#include "lexer.hpp"
#include "../errors/error.hpp"

// ======================
// -- CONSTRUCTOR
// ======================

/// @brief Construct from raw argv
/// @param argc: Argument count from main
/// @param argv: Argument vector from main
Lexer::Lexer(int argc, const char* const* argv) noexcept
: _argc(argc), _argv(argv) {}

// ======================
// -- PUBLIC API
// ======================

/// @brief Lex argv into a token stream
/// @return Vector of tokens
/// @throws LexError: On malformed token
std::vector<Token> Lexer::lex()
{
    std::vector<Token> tokens;
    tokens.reserve(_argc);

    for (int i = 1; i < _argc; ++i)
    {
        std::string_view lexeme{ _argv[i] };

        if (lexeme.empty())
            throw LexError(make_lex_error(lexeme));

        TokenType type = classify(lexeme);

        if (type == TokenType::FLAG)
            lexeme = strip_dashes(lexeme);

        tokens.push_back(Token{ lexeme, type });
    }

    tokens.push_back(Token{ "", TokenType::END });

    return tokens;
}

// ======================
// -- HELPERS
// ======================

/// @brief Classify a single argv string into a TokenType
/// @param s: The argv string
/// @return TokenType
TokenType Lexer::classify(std::string_view s) noexcept
{
    if (s.size() >= 2 && s[0] == '-' && s[1] == '-')
        return TokenType::FLAG;

    if (s.size() >= 1 && s[0] == '-')
        return TokenType::FLAG;

    return TokenType::SUBCOMMAND;
}

/// @brief Strip leading dashes from a flag lexeme
/// @param s: The flag string
/// @return Stripped string_view
std::string_view Lexer::strip_dashes(std::string_view s) noexcept
{
    std::size_t i = 0;

    while (i < s.size() && s[i] == '-')
        ++i;

    return s.substr(i);
}
