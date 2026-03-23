#ifndef TOKEN_DATA_HPP
#define TOKEN_DATA_HPP

#include <string_view>
#include <cstdint>

// ======================
// -- DATA
// ======================

enum class TokenType : uint8_t
{
    SUBCOMMAND,
    FLAG,
    VALUE,
    END
};

struct Token
{
    std::string_view lexeme;
    TokenType type;
};

#endif
