#include <string_view>
#include <vector>
#include <optional>

#include "./parser.hpp"
#include "./flags/valid_flags.hpp"
#include "../errors/error.hpp"

// ======================
// -- CONSTRUCTOR
// ======================

/// @brief Construct from raw argv
/// @param argc: Argument count from main
/// @param argv: Argument vector from main
Parser::Parser(int argc, const char* const* argv)
    : _tokens(Lexer(argc, argv).lex()) {}

    // ======================
    // -- PUBLIC API
    // ======================

    /// @brief Run the full parse
    /// @return ParsedArgs with ordered pipeline and optional subcommand
    /// @throws ParseError: On unknown flag or malformed input
    ParsedArgs Parser::parse()
    {
        ParsedArgs out;

        parse_subcommand(out);
        parse_pipeline(out);

        return out;
    }

// ======================
// -- TOKEN ITERATING
// ======================

/// @brief Check if there are no more tokens
/// @return bool
bool Parser::is_at_end() const noexcept
{
    return _tokens[_pos].type == TokenType::END;
}

/// @brief Get the current token
/// @return Token
Token Parser::current() const noexcept
{
    return _tokens[_pos];
}

/// @brief Peek at the next token
/// @return Token
Token Parser::peek_next() const noexcept
{
    if (_pos + 1 >= _tokens.size())
        return _tokens.back();

    return _tokens[_pos + 1];
}

/// @brief Consume the current token and return it
/// @return Token
Token Parser::consume() noexcept
{
    return _tokens[_pos++];
}

/// @brief Check if the current token matches the given type
/// @param type: TokenType to match
/// @return bool
bool Parser::match(TokenType type) const noexcept
{
    if (is_at_end())
        return false;

    return current().type == type;
}

// ======================
// -- PARSING
// ======================

/// @brief Optionally consume a leading subcommand token
/// @param out: ParsedArgs to fill
void Parser::parse_subcommand(ParsedArgs& out)
{
    if (!match(TokenType::SUBCOMMAND))
        return;

    out.subcommand = consume().lexeme;
}

/// @brief Consume all flag/value pairs into the pipeline
/// @param out: ParsedArgs to fill
void Parser::parse_pipeline(ParsedArgs& out)
{
    while (!is_at_end())
        out.pipeline.push_back(parse_op());
}

/// @brief Consume one flag and optional value into an Op
/// @return Op
/// @throws ParseError: On unknown flag
Op Parser::parse_op()
{
    Token tok = consume();
    std::string_view name = tok.lexeme;

    const FlagInfo* info = lookup_flag(name);

    if (!info)
        throw ParseError(make_parse_error(name));

    if (!info->takes_value)
        return Op{ name, std::nullopt };

    if (!match(TokenType::VALUE))
        throw ParseError(make_parse_error(name));

    return Op{ name, consume().lexeme };
}
