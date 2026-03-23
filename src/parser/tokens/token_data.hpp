#ifndef TOKEN_DATA_HPP
#define TOKEN_DATA_HPP

#include <string_view>
#include <cstdint>

// ======================
// -- OpMode
// ======================

enum class OpMode : uint8_t { Default, Encode, Decode };

// ======================
// -- TokenType
// ======================

enum class TokenType : uint8_t { SUBCOMMAND, FLAG, FLAG_WITH_MODE, VALUE, END };

// ======================
// -- FlagRole
// ======================

enum class FlagRole : uint8_t
{
    Input,
    GlobalDecode,
    GlobalEncode,
    Pipeline
};

// ======================
// -- Token
// ======================

struct Token
{
    std::string_view lexeme;
    std::string_view suffix;
    TokenType type;
    bool is_long = false;
};

#endif
