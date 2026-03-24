#ifndef REVERSE_HPP
#define REVERSE_HPP

#include <string>
#include <string_view>
#include <algorithm>

namespace reverse
{
    /// @brief Apply string reverse
    /// @param input: std::string_view
    /// @return std::string
    [[nodiscard]] inline std::string apply(std::string_view input)
    {
        std::string out(input);
        std::reverse(out.begin(), out.end());

        return out;
    }

}

#endif
