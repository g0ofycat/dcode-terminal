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
        std::string_view raw{ _argv[i] };

        if (raw.empty())
            throw LexError(make_lex_error(raw));

        TokenType type = classify(raw);

        if (type == TokenType::FLAG)
        {
            bool is_long = (raw.size() >= 2 && raw[0] == '-' && raw[1] == '-');
            std::string_view suffix;
            std::string_view name = strip_suffix(strip_dashes(raw), suffix);

            if (!suffix.empty() && !valid_suffix(suffix))
                throw LexError(make_lex_error(raw));

            TokenType final_type = suffix.empty() ? TokenType::FLAG : TokenType::FLAG_WITH_MODE;
            tokens.push_back(Token{ name, suffix, final_type, is_long });
            continue;
        }

        tokens.push_back(Token{ raw, {}, type });
    }

    tokens.push_back(Token{ "", {}, TokenType::END });

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

/// @brief Strip the mode suffix from a flag lexeme
/// @param s: The stripped flag string
/// @param out_suffix: The suffix if found
/// @return The flag name without suffix
std::string_view Lexer::strip_suffix(std::string_view s, std::string_view& out_suffix) noexcept
{
    auto sep = s.find('=');

    if (sep == std::string_view::npos)
    {
        out_suffix = {};
        return s;
    }

    out_suffix = s.substr(sep + 1);
    return s.substr(0, sep);
}

/// @brief Validate a mode suffix
/// @param suffix: The suffix to validate
/// @return True if valid
bool Lexer::valid_suffix(std::string_view suffix) noexcept
{
    return suffix == "e" || suffix == "d";
}
