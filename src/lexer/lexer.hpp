#ifndef LEXER_HPP
#define LEXER_HPP

#include <string_view>
#include <vector>

#include "../parser/tokens/token_data.hpp"

// ======================
// -- Lexer
// ======================

class Lexer
{
    public:
        // ======================
        // -- CONSTRUCTOR
        // ======================

        /// @brief Construct from raw argv
        /// @param argc: Argument count from main
        /// @param argv: Argument vector from main
        Lexer(int argc, const char* const* argv) noexcept;

        // ======================
        // -- PUBLIC API
        // ======================

        /// @brief Lex argv into a token stream
        /// @return Vector of tokens
        /// @throws LexError: On malformed token
        [[nodiscard]] std::vector<Token> lex();

    private:
        // ======================
        // -- PRIVATE DATA
        // ======================

        int _argc;
        const char* const* _argv;

        // ======================
        // -- PRIVATE HELPERS
        // ======================

        /// @brief Classify a single argv string into a TokenType
        /// @param s: The argv string
        /// @return TokenType
        [[nodiscard]] static TokenType classify(std::string_view s) noexcept;

        /// @brief Strip leading dashes from a flag lexeme
        /// @param s: The flag string
        /// @return Stripped string_view
        [[nodiscard]] static std::string_view strip_dashes(std::string_view s) noexcept;
};

#endif
