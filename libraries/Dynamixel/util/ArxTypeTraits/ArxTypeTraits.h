// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#pragma once

#ifndef ARX_TYPE_TRAITS_H
#define ARX_TYPE_TRAITS_H

#if __cplusplus < 201103L
    #error "C++11 must be enabled in the compiler for this library to work, please check your compiler flags"
#endif

#include "ArxTypeTraits/has_include.h"
#include "ArxTypeTraits/has_libstdcplusplus.h"

// Make sure std namespace exists
namespace std { }

// Import everything from the std namespace into arx::std, so that
// anything we import rather than define is also available through
// arx::stdx.
// This includes everything yet to be defined, so we can do this early
// (and must do so, to allow e.g. the C++14 additions in the arx::std
// namespace to reference the C++11 stuff from the system headers.
namespace arx {
    namespace stdx {
        using namespace ::std;
    }
}

// Import everything from arx::std back into the normal std namespace.
// This ensures that you can just use `std::foo` everywhere and you get
// the standard library version if it is available, falling back to arx
// versions for things not supplied by the standard library. Only when
// you really need the arx version (e.g. for constexpr numeric_limits
// when also using ArduinoSTL), you need to qualify with arx::stdx::
namespace std {
    using namespace ::arx::stdx;
}

#include "ArxTypeTraits/replace_minmax_macros.h"

#include "ArxTypeTraits/type_traits.h"

#endif // ARX_TYPE_TRAITS_H
