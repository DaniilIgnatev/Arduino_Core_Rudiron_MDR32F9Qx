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

#ifndef ADC_H
#define ADC_H

#include "MDR_config.h"
#include "MDR32Fx.h"
#include "MDR_port.h"
#include "MDR_rst_clk.h"
#include "MDR_adc.h"
#include "MDR_dma.h"
#include "MDR_config.h"
#include "gpio.h"


namespace Rudiron
{
    typedef enum
    {
        ADC_Channel0,
        ADC_Channel1,
        ADC_Channel2,
        ADC_Channel3,
        ADC_Channel4,
        ADC_Channel5,
        ADC_Channel6,
        ADC_Channel7
    } ADCChannelName;


    struct ADCResult
    {
        bool valid = false;
        bool override = false;
        uint16_t value = 0;
        ADCChannelName channel = ADC_Channel0;
    };


    class ADC
    {
    private:
        static uint32_t channelMask;

        static ADCResult readValues[7];

        static void initPinADC(PortPinName pinName);

    public:
        static void configureDefault();

        static bool configurePin(PortPinName pinName, bool enable);

        static void start();

        static void stop();

        static ADCResult readPin(PortPinName pinName);
    };
}

#endif // ADC_H
