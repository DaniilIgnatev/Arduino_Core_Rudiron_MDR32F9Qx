/**
  ******************************************************************************
  * @file    Examples/MDR32F9Q3_EVAL/BKP/RTC/MDR32F9Qx_it.h
  * @author  Milandr Application Team
  * @version V1.2.0
  * @date    04/07/2011
  * @brief   This file contains all the functions prototypes for the interrupt.
  *
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Milandr</center></h2>
  * 
  * Modified for Arduino_Core_Rudiron_MDR32F9Qx by Daniil Ignatev on 08.03.2022
  */

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32F9QX_IT_H
#define __MDR32F9QX_IT_H

#include "MDR32Fx.h"

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif


void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void CAN1_IRQHandler(void);
void CAN2_IRQHandler(void);
//void USB_IRQHandler(void);
void DMA_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
void SSP1_IRQHandler(void);
void I2C_IRQHandler(void);
void POWER_IRQHandler(void);
void WWDG_IRQHandler(void);
void Timer1_IRQHandler(void);
void Timer2_IRQHandler(void);
void Timer3_IRQHandler(void);
void ADC_IRQHandler(void);
void COMPARATOR_IRQHandler(void);
void SSP2_IRQHandler(void);
void BACKUP_IRQHandler(void);
void EXT_INT1_IRQHandler(void);
void EXT_INT2_IRQHandler(void);
void EXT_INT3_IRQHandler(void);
void EXT_INT4_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MDR32F9QX_IT_H */

/******************* (C) COPYRIGHT 2011 Milandr *********/

/* END OF FILE MDR32F9Qx_it.h */
