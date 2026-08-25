/*
 * SPDX-License-Identifier: MIT
 */

#include <io/printf.hxx>

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <format>
#include <regex>
#include <algorithm>

namespace blazing::io::printf {

// Helper to format a single value using std::format with an optional specifier
template<typename T>
std::string format_value(const T& value, std::string_view spec = "{}") {
    // spec is like "{}", "{:d}", "{:.2f}" etc.
    return std::format(spec, value);
}

Napi::Value Printf(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Format string expected as first argument")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string fmt_str = info[0].As<Napi::String>().Utf8Value();
    std::string result;

    // Use regex to find placeholders: {} or {:[^}]*}
    // We'll process sequentially, matching with arguments.
    std::regex placeholder(R"(\{([^}]*)\})");
    std::smatch match;
    std::string::const_iterator search_start(fmt_str.cbegin());
    size_t arg_index = 1;  // first argument is format string

    // We'll collect the output parts
    auto append_literal = [&](const std::string& literal) {
        result += literal;
    };

    try {
        while (std::regex_search(search_start, fmt_str.cend(), match, placeholder)) {
            // Append literal before the placeholder
            result.append(match.prefix().first, match.prefix().second);

            // Get the specifier inside the braces (e.g., ":d", ".2f" etc.)
            std::string spec = match[1].str();
            std::string full_placeholder = match[0].str();

            // Check if we have a corresponding argument
            if (arg_index >= info.Length()) {
                // Not enough arguments, leave placeholder as is
                result += full_placeholder;
            } else {
                Napi::Value arg = info[arg_index++];

                // Format the value according to its type and the specifier
                std::string formatted;
                if (arg.IsNumber()) {
                    double val = arg.As<Napi::Number>().DoubleValue();
                    // Check if integer to avoid ".0"
                    if (val == static_cast<int64_t>(val)) {
                        formatted = format_value(static_cast<int64_t>(val), full_placeholder);
                    } else {
                        formatted = format_value(val, full_placeholder);
                    }
                } else if (arg.IsString()) {
                    std::string str = arg.As<Napi::String>().Utf8Value();
                    formatted = format_value(str, full_placeholder);
                } else if (arg.IsBoolean()) {
                    bool b = arg.As<Napi::Boolean>().Value();
                    formatted = format_value(b, full_placeholder);
                } else {
                    // Non‑primitive: convert to string representation
                    formatted = format_value("[Object]", full_placeholder);
                }
                result += formatted;
            }

            // Continue after the placeholder
            search_start = match.suffix().first;
        }

        // Append the remaining literal after the last placeholder
        result.append(search_start, fmt_str.cend());

        // Output to stdout (like console.log)
        std::cout << result;

        return Napi::String::New(env, result);

    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(
        Napi::String::New(env, "printf"),
        Napi::Function::New(env, Printf)
    );
    return exports;
}

} // namespace blazing::io::printf
