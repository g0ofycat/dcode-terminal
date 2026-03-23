#include "./error.hpp"

// ======================
// -- IMPL
// ======================

std::string make_parse_error(std::string_view flag)
{
    return "Unknown flag: '" + std::string(flag) + "'";
}

std::string make_lex_error(std::string_view token)
{
    return "Malformed token: '" + std::string(token) + "'";
}
