#ifndef ERROR_HPP
#define ERROR_HPP

#include <string_view>
#include <string>
#include <stdexcept>

// ======================
// -- DcodeError
// ======================

/// @brief Base class for all dcode errors
class DcodeError : public std::runtime_error
{
    public:
        explicit DcodeError(std::string_view msg)
            : std::runtime_error(std::string(msg)) {}
};

// ======================
// -- ParseError
// ======================

/// @brief Thrown when the parser encounters an unknown flag or malformed input
class ParseError : public DcodeError
{
    public:
        explicit ParseError(std::string_view msg)
            : DcodeError(msg) {}
};

// ======================
// -- LexError
// ======================

/// @brief Thrown when the lexer encounters malformed argv input
class LexError : public DcodeError
{
    public:
        explicit LexError(std::string_view msg)
            : DcodeError(msg) {}
};

// ======================
// -- ERROR BUILDERS
// ======================

/// @brief Build a parse error message for an unknown flag
/// @param flag: The unrecognised flag string
/// @return Formatted error string
[[nodiscard]] std::string make_parse_error(std::string_view flag);

/// @brief Build a lex error message for a malformed token
/// @param token: The malformed token string
/// @return Formatted error string
[[nodiscard]] std::string make_lex_error(std::string_view token);

#endif
