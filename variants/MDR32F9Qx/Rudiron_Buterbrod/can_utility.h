/*
This file is part of Arduino_Core_Rudiron.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CAN_UTILITY_H
#define CAN_UTILITY_H

#include "MDR32Fx.h"

namespace Rudiron
{
    // Пакет CAN
    typedef struct
    {
        uint32_t ID;
        uint8_t Data[8];
        uint8_t Mask;
    } CAN_RX_Package;
}

#endif // CAN_UTILITY_H
