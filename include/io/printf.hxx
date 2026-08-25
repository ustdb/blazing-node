/*
 * SPDX-License-Identifier: MIT
 */

#ifndef BLAZING_PRINTF_HXX
#define BLAZING_PRINTF_HXX

#include <napi.h>

namespace blazing::io::printf {

Napi::Value Printf(const Napi::CallbackInfo& info);
Napi::Object Init(Napi::Env env, Napi::Object exports);

} // namespace blazing::printf

#endif // BLAZING_PRINTF_HXX

