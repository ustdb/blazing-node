/*
 * SPDX-License-Identifier: MIT
 */

#include <napi.h>
#include <io/printf.hxx>

NAPI_MODULE_INIT() {
    Napi::Object exports_obj(env, exports);  // wrap raw napi_value
    blazing::io::printf::Init(env, exports_obj);
    return exports;
}
