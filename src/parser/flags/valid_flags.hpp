#ifndef VALID_FLAGS_HPP
#define VALID_FLAGS_HPP

#include <string_view>
#include <array>

// ======================
// -- FlagInfo
// ======================

/// @brief Metadata for a known flag
struct FlagInfo
{
    std::string_view name;
    bool takes_value;
    std::string_view description;
};

// ======================
// -- FLAG REGISTRY
// ======================

inline constexpr std::array VALID_FLAGS =
{
    FlagInfo{ "b64", false, "Base64 encode/decode" },
    FlagInfo{ "r", false, "Reverse the input" },
    FlagInfo{ "e", false, "Encode the input" },
    FlagInfo{ "d", false, "Decode the input" }
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
        if (f.name != name)
            continue;

        return &f;
    }

    return nullptr;
}

#endif
