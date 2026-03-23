#ifndef PARSER_HPP
#define PARSER_HPP

#include <string_view>
#include <vector>
#include <optional>

#include "./tokens/token_data.hpp"
#include "../lexer/lexer.hpp"
#include "../errors/error.hpp"

// ======================
// -- Op
// ======================

struct Op
{
    std::string_view flag;
    std::optional<std::string_view> value;
};

// ======================
// -- ParsedArgs
// ======================

struct ParsedArgs
{
    std::optional<std::string_view> subcommand;
    std::vector<Op> pipeline;
};

// ======================
// -- Parser
// ======================

class Parser
{
    public:
        // ======================
        // -- CONSTRUCTOR
        // ======================

        /// @brief Construct from raw argv
        /// @param argc: Argument count from main
        /// @param argv: Argument vector from main
        Parser(int argc, const char* const* argv);

        // ======================
        // -- PUBLIC API
        // ======================

        /// @brief Run the full parse
        /// @return ParsedArgs: with ordered pipeline and optional subcommand
        /// @throws ParseError: on unknown flag or malformed input
        [[nodiscard]] ParsedArgs parse();

    private:
        // ======================
        // -- PRIVATE DATA
        // ======================

        std::vector<Token> _tokens;
        std::size_t _pos = 0;

        // ======================
        // -- TOKEN ITERATING
        // ======================

        /// @brief Check if there are no more tokens
        /// @return bool
        [[nodiscard]] bool is_at_end() const noexcept;

        /// @brief Get the current token
        /// @return Token
        [[nodiscard]] Token current() const noexcept;

        /// @brief Peek at the next token
        /// @return Token
        [[nodiscard]] Token peek_next() const noexcept;

        /// @brief Consume the current token and return it
        /// @return Token
        Token consume() noexcept;

        /// @brief Check if the current token matches the given type
        /// @param type: TokenType to match
        /// @return bool
        [[nodiscard]] bool match(TokenType type) const noexcept;

        // ======================
        // -- PARSING
        // ======================

        /// @brief Optionally consume a leading subcommand token
        /// @param out: ParsedArgs to fill
        void parse_subcommand(ParsedArgs& out);

        /// @brief Consume all flag/value pairs into the pipeline
        /// @param out: ParsedArgs to fill
        void parse_pipeline(ParsedArgs& out);

        /// @brief Consume one flag and optional value into an Op
        /// @return Op
        /// @throws ParseError: On unknown flag
        [[nodiscard]] Op parse_op();
};

#endif
