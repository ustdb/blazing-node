/*
 * SPDX-License-Identifier: MIT
 */

#include <io/printf.hxx>

#include <iostream>
#include <string>
#include <regex>
#include <format>
#include <cmath>

namespace blazing::io::printf {

Napi::Value Printf(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Format string expected as first argument")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string fmt_str = info[0].As<Napi::String>().Utf8Value();
    std::string result;

    std::regex placeholder(R"(\{([^}]*)\})");
    std::smatch match;
    std::string::const_iterator search_start(fmt_str.cbegin());
    size_t arg_index = 1;

    try {
        while (std::regex_search(search_start, fmt_str.cend(), match, placeholder)) {
            // Append literal before placeholder
            result.append(match.prefix().first, match.prefix().second);

            std::string full_placeholder = match[0].str(); // e.g. "{:.2f}"

            if (arg_index >= info.Length()) {
                // Not enough arguments → keep placeholder as‑is
                result += full_placeholder;
            } else {
                Napi::Value arg = info[arg_index++];
                std::string formatted;

                if (arg.IsNumber()) {
                    double val = arg.As<Napi::Number>().DoubleValue();
                    // Detect if it's an integer (safe for int64)
                    if (std::floor(val) == val &&
                        val <= 9007199254740991 && val >= -9007199254740991) {
                        int64_t i = static_cast<int64_t>(val);
                        formatted = std::vformat(full_placeholder, std::make_format_args(i));
                    } else {
                        formatted = std::vformat(full_placeholder, std::make_format_args(val));
                    }
                } else if (arg.IsString()) {
                    std::string str = arg.As<Napi::String>().Utf8Value();
                    formatted = std::vformat(full_placeholder, std::make_format_args(str));
                } else if (arg.IsBoolean()) {
                    bool b = arg.As<Napi::Boolean>().Value();
                    formatted = std::vformat(full_placeholder, std::make_format_args(b));
                } else {
                    // Non‑primitive → show as "[Object]"
                    formatted = std::vformat(full_placeholder, std::make_format_args("[Object]"));
                }

                result += formatted;
            }

            search_start = match.suffix().first;
        }

        // Append the tail after the last placeholder
        result.append(search_start, fmt_str.cend());

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
