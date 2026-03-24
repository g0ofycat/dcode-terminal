#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <string>
#include <string_view>
#include <stdexcept>
#include <unordered_map>

#include "../parser/parser.hpp"
#include "../errors/error.hpp"
#include "../libs/base_64.hpp"
#include "../libs/reverse.hpp"

// ======================
// -- CodecFn
// ======================

using CodecFn = std::string(*)(std::string_view, OpMode);

// ======================
// -- Executor
// ======================

class Executor
{
    public:
        /// @brief Executor constructor
        /// @param args: The arguments to run
        /// @return data: The output data
        explicit Executor(const ParsedArgs& args) : args_(args) {}

        [[nodiscard]] std::string run() const
        {
            std::string data = std::string(args_.input.value()); 

            for (const auto& op : args_.pipeline)
            {
                auto it = table_.find(std::string(op.flag)); 

                if (it == table_.end())
                    throw DcodeError("executor: unknown operation '-" + std::string(op.flag) + "'"); 

                data = it->second(data, op.mode);
            }

            return data;
        }

    private:
        const ParsedArgs& args_;

        inline static const std::unordered_map<std::string, CodecFn> table_ =
        {
            {
                "b64", [](std::string_view data, OpMode mode) -> std::string
                {
                    try
                    {
                        return mode == OpMode::Encode
                            ? base64::to_base64(data)
                            : base64::from_base64(data);
                    }
                    catch (const std::runtime_error& e)
                    {
                        throw DcodeError(std::string("base64: ") + e.what());
                    }
                }
            },
            {
                "r", [](std::string_view data, OpMode) -> std::string
                {
                    return reverse::apply(data);
                }
            },
        };
};

#endif
