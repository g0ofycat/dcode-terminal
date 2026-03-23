#include "./parser.hpp"
#include "./flags/valid_flags.hpp"
#include "../errors/error.hpp"

#include <string>

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

        if (!out.input.has_value())
            throw ParseError("No input provided; use -i \"...\"");

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
Token Parser::current() const noexcept { return _tokens[_pos]; }

/// @brief Peek at the next token
/// @return Token
Token Parser::peek_next() const noexcept
{
    if (_pos + 1 >= _tokens.size()) return _tokens.back();
    return _tokens[_pos + 1];
}

/// @brief Consume the current token and return it
/// @return Token
Token Parser::consume() noexcept { return _tokens[_pos++]; }

/// @brief Check if the current token matches the given type
/// @param type: TokenType to match
/// @return bool
bool Parser::match(TokenType type) const noexcept
{
    return !is_at_end() && current().type == type;
}

/// @brief Check if the current token is any flag type
/// @return bool
bool Parser::match_flag() const noexcept
{
    return match(TokenType::FLAG)
        || match(TokenType::FLAG_WITH_MODE)
        || match(TokenType::LONG_FLAG)
        || match(TokenType::LONG_FLAG_WITH_MODE);
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

    TokenType next = peek_next().type;
    if (next != TokenType::FLAG && next != TokenType::FLAG_WITH_MODE)
        return;

    out.subcommand = consume().lexeme;
}

/// @brief Consume all flag/value pairs into the pipeline
/// @param out: ParsedArgs to fill
void Parser::parse_pipeline(ParsedArgs& out)
{
    while (!is_at_end())
    {
        if (!match_flag())
            throw ParseError("Unexpected token '" + std::string(current().lexeme) + "'");

        Op op = parse_op();
        const FlagInfo* info = lookup_flag(op.flag);

        switch (info->role)
        {
            case FlagRole::Input:
                out.input = op.value;
                break;

            case FlagRole::GlobalDecode:
                if (!is_at_end())
                    throw ParseError("--d must be the last flag in the pipeline");
                out.global_decode = true;
                break;

            case FlagRole::GlobalEncode:
                if (!is_at_end())
                    throw ParseError("--e must be the last flag in the pipeline");
                out.global_encode = true;
                break;

            case FlagRole::Pipeline:
                out.pipeline.push_back(op);
                break;
        }
    }
}

/// @brief Resolve OpMode from a token suffix
/// @param suffix: The suffix string_view from the token
/// @return OpMode
OpMode Parser::resolve_mode(std::string_view suffix) noexcept
{
    if (suffix == "d") return OpMode::Decode;
    if (suffix == "e") return OpMode::Encode;
    return OpMode::Default;
}

/// @brief Consume one flag and optional value into an Op
/// @return Op
/// @throws ParseError: On unknown flag or non-flag token
Op Parser::parse_op()
{
    if (!match_flag())
        throw ParseError("Expected a flag but got '" + std::string(current().lexeme) + "'");

    Token tok = consume();
    const FlagInfo* info = lookup_flag(tok.lexeme);

    if (!info)
        throw ParseError(make_parse_error(tok.lexeme));

    bool is_long = tok.type == TokenType::LONG_FLAG || tok.type == TokenType::LONG_FLAG_WITH_MODE;

    if (is_long && info->role == FlagRole::Pipeline)
        throw ParseError("Unknown flag: '--" + std::string(tok.lexeme) + "'");

    OpMode mode = resolve_mode(tok.suffix);

    if (!info->takes_value)
        return Op{ tok.lexeme, std::nullopt, mode };

    if (is_at_end() || match_flag())
        throw ParseError("Flag '-" + std::string(tok.lexeme) + "' expects a value");

    return Op{ tok.lexeme, consume().lexeme, mode };
}
