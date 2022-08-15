 /*
  * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
  * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
  * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
  * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
  * SPI Master library for arduino.
  *
  * This file is free software; you can redistribute it and/or modify
  * it under the terms of either the GNU General Public License version 2
  * or the GNU Lesser General Public License version 2.1, both as
  * published by the Free Software Foundation.
  *
  * Modified for Arduino_Core_Rudiron by Daniil Ignatev on 13.03.2022
  */

 #include "SPI.h"

 uint8_t SPIClass::initialized = 0;
 uint8_t SPIClass::interruptMode = 0;
 uint8_t SPIClass::interruptMask = 0;
 uint8_t SPIClass::interruptSave = 0;
 #ifdef SPI_TRANSACTION_MISMATCH_LED
 uint8_t SPIClass::inTransactionFlag = 0;
 #endif

SPISettings SPIClass::lastSettings;

 void SPIClass::begin()
 {
   // uint8_t sreg = SREG;
   // noInterrupts(); // Protect from a scheduler and prevent transactionBegin
   // if (!initialized) {
   //   // Set SS to high so a connected chip will be "deselected" by default
   //   uint8_t port = digitalPinToPort(SS);
   //   uint8_t bit = digitalPinToBitMask(SS);
   //   volatile uint8_t *reg = portModeRegister(port);

   //   // if the SS pin is not already configured as an output
   //   // then set it high (to enable the internal pull-up resistor)
   //   if(!(*reg & bit)){
   //     digitalWrite(SS, HIGH);
   //   }

   //   // When the SS pin is set as OUTPUT, it can be used as
   //   // a general purpose output port (it doesn't influence
   //   // SPI operations).
   // pinMode(SS, OUTPUT);

   //   // Warning: if the SS pin ever becomes a LOW INPUT then SPI
   //   // automatically switches to Slave, so the data direction of
   //   // the SS pin MUST be kept as OUTPUT.
   //   SPCR |= _BV(MSTR);
   //   SPCR |= _BV(SPE);

   //   // Set direction register for SCK and MOSI pin.
   //   // MISO pin automatically overrides to INPUT.
   //   // By doing this AFTER enabling SPI, we avoid accidentally
   //   // clocking in a single bit since the lines go directly
   //   // from "input" to SPI control.
   //   // http://code.google.com/p/arduino/issues/detail?id=888
   //   pinMode(SCK, OUTPUT);
   //   pinMode(MOSI, OUTPUT);
   // }
   // initialized++; // reference count
   // SREG = sreg;

    uint16_t SPH = lastSettings.dataMode % 2 == 0 ? SSP_SPH_1Edge : SSP_SPH_2Edge;
    uint16_t SPO = lastSettings.dataMode < 2 ? SSP_SPO_Low : SSP_SPO_High;

    spi_instance.begin(lastSettings.clock, SPH, SPO, SSP_WordLength8b, SSP_FRF_SPI_Motorola, SSP_HardwareFlowControl_SSE);
 }

 void SPIClass::end()
 {
   // uint8_t sreg = SREG;
   // noInterrupts(); // Protect from a scheduler and prevent transactionBegin
   // // Decrease the reference counter
   // if (initialized)
   //   initialized--;
   // // If there are no more references disable SPI
   // if (!initialized) {
   //   SPCR &= ~_BV(SPE);
   //   interruptMode = 0;
   //   #ifdef SPI_TRANSACTION_MISMATCH_LED
   //   inTransactionFlag = 0;
   //   #endif
   // }
   // SREG = sreg;
   spi_instance.end();
 }


 void SPIClass::usingInterrupt(uint8_t interruptNumber)
 {
   // uint8_t mask = 0;
   // uint8_t sreg = SREG;
   // noInterrupts(); // Protect from a scheduler and prevent transactionBegin
   // switch (interruptNumber) {
   // #ifdef SPI_INT0_MASK
   // case 0: mask = SPI_INT0_MASK; break;
   // #endif
   // #ifdef SPI_INT1_MASK
   // case 1: mask = SPI_INT1_MASK; break;
   // #endif
   // #ifdef SPI_INT2_MASK
   // case 2: mask = SPI_INT2_MASK; break;
   // #endif
   // #ifdef SPI_INT3_MASK
   // case 3: mask = SPI_INT3_MASK; break;
   // #endif
   // #ifdef SPI_INT4_MASK
   // case 4: mask = SPI_INT4_MASK; break;
   // #endif
   // #ifdef SPI_INT5_MASK
   // case 5: mask = SPI_INT5_MASK; break;
   // #endif
   // #ifdef SPI_INT6_MASK
   // case 6: mask = SPI_INT6_MASK; break;
   // #endif
   // #ifdef SPI_INT7_MASK
   // case 7: mask = SPI_INT7_MASK; break;
   // #endif
   // default:
   //   interruptMode = 2;
   //   break;
   // }
   // interruptMask |= mask;
   // if (!interruptMode)
   //   interruptMode = 1;
   // SREG = sreg;
 }

 void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
 {
   // Once in mode 2 we can't go back to 0 without a proper reference count
   // if (interruptMode == 2)
   //   return;
   // uint8_t mask = 0;
   // uint8_t sreg = SREG;
   // noInterrupts(); // Protect from a scheduler and prevent transactionBegin
   // switch (interruptNumber) {
   // #ifdef SPI_INT0_MASK
   // case 0: mask = SPI_INT0_MASK; break;
   // #endif
   // #ifdef SPI_INT1_MASK
   // case 1: mask = SPI_INT1_MASK; break;
   // #endif
   // #ifdef SPI_INT2_MASK
   // case 2: mask = SPI_INT2_MASK; break;
   // #endif
   // #ifdef SPI_INT3_MASK
   // case 3: mask = SPI_INT3_MASK; break;
   // #endif
   // #ifdef SPI_INT4_MASK
   // case 4: mask = SPI_INT4_MASK; break;
   // #endif
   // #ifdef SPI_INT5_MASK
   // case 5: mask = SPI_INT5_MASK; break;
   // #endif
   // #ifdef SPI_INT6_MASK
   // case 6: mask = SPI_INT6_MASK; break;
   // #endif
   // #ifdef SPI_INT7_MASK
   // case 7: mask = SPI_INT7_MASK; break;
   // #endif
   // default:
   //   break;
   //   // this case can't be reached
   // }
   // interruptMask &= ~mask;
   // if (!interruptMask)
   //   interruptMode = 0;
   // SREG = sreg;
 }

 SPIClass SPI;
