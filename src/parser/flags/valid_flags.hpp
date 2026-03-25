#ifndef VALID_FLAGS_HPP
#define VALID_FLAGS_HPP

#include <string_view>
#include <array>

#include "../tokens/token_data.hpp"

// ======================
// -- FlagInfo
// ======================

struct FlagInfo
{
    std::string_view name;
    bool takes_value;
    FlagRole role;
    std::string_view description;
    std::string_view usage_hint;
};

// ======================
// -- FLAG REGISTRY
// ======================

inline constexpr std::array VALID_FLAGS =
{
    FlagInfo{ "i", true, FlagRole::Input, "Input string", "<input>" },
    FlagInfo{ "d", false, FlagRole::GlobalDecode, "Decode the input", "" },
    FlagInfo{ "e", false, FlagRole::GlobalEncode, "Encode the input", "" },
    FlagInfo{ "b64", false, FlagRole::Pipeline, "Base64 encode/decode", "" },
    FlagInfo{ "r", false, FlagRole::Pipeline, "Reverse the input", "" },
};

// ======================
// -- PUBLIC API
// ======================

/// @brief Check if a flag name is known
/// @param name: Stripped flag name
/// @return Pointer to FlagInfo if found, nullptr if unknown
[[nodiscard]] inline constexpr const FlagInfo* lookup_flag(std::string_view name) noexcept
{
    for (const auto& f : VALID_FLAGS) {
        if (f.name == name) return &f;
    }

    return nullptr;
}

#endif
