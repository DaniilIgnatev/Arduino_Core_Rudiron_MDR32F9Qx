/// @file cpp_compat.h
/// Compatibility functions based on C++ version

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#ifndef __INC_CPP_COMPAT_H
#define __INC_CPP_COMPAT_H

#include "FastLED.h"

#if __cplusplus <= 199711L

/// Compile-time assertion checking, introduced in C++11
/// @see https://en.cppreference.com/w/cpp/language/static_assert
#define static_assert(expression, message)

/// Declares that it is possible to evaluate a value at compile time, introduced in C++11
/// @see https://en.cppreference.com/w/cpp/language/constexpr
#define constexpr const

#else

// things that we can turn on if we're in a C++11 environment
#endif

#if __cplusplus < 201703L
#define FASTLED_REGISTER register
#else
  #ifdef FASTLED_REGISTER
    #undef FASTLED_REGISTER
  #endif
#define FASTLED_REGISTER

#endif

#endif
