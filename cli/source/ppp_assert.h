#pragma once

#include <cstring>

// clang-format off

#define ppp_assert_msg(expr, msg) {          \
                                             \
  if (false == static_cast<bool>(expr)) {    \
                                             \
    char buffer[512];                        \
                                             \
    std::snprintf(                           \
        buffer, sizeof(buffer),              \
        "Failed to assert (%s) in %s%s.",    \
        #expr,                               \
        __FUNCTION__,                        \
        (((msg) && std::strlen(msg))         \
           ? (" (" msg ")")                  \
           : (""))                           \
    );                                       \
                                             \
    throw std::runtime_error(buffer);        \
  }                                          \
}

#define ppp_assert(expr) ppp_assert_msg((expr), "")

// clang-format on
