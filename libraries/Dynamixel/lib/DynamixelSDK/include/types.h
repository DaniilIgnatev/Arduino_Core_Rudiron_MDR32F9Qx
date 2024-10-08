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
#ifndef DYNAMIXEL_SDK_TYPES_H
#define DYNAMIXEL_SDK_TYPES_H

namespace dynamixel {

#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L  // Have libstdc++11
template <typename T>
using Vec = std::vector<T>;
template <typename T, typename U>
using Map = std::map<T, U>;
#else  // Do not have libstdc++11
template <typename T>
using Vec = arx::vector<T>;
template <typename T, typename U>
using Map = arx::map<T, U>;
#endif

}  // namespace dynamixel

#endif  // DYNAMIXEL_SDK_TYPES_H
