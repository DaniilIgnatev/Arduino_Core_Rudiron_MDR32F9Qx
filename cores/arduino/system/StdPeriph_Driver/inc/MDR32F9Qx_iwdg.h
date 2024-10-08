//Source: https://github.com/eldarkg/emdr1986x-std-per-lib

/**
  * FILE MDR32F9Qx_iwdg.h
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
#ifndef __MDR32F9Qx_IWDG_H
#define __MDR32F9Qx_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_lib.h"

/** @addtogroup __MDR32F9Qx_StdPeriph_Driver MDR32F9Qx Standard Peripherial Driver
  * @{
  */

/** @addtogroup IWDG
  * @{
  */

/** @defgroup IWDG_Exported_Types IWDG Exported Types
  * @{
  */

/** @} */ /* End of group IWDG_Exported_Types */

/** @defgroup IWDG_Exported_Constants IWDG Exported Constants
  * @{
  */

/** @defgroup IWDG_prescaler  IWDG prescaler
  * @{
  */

#define IWDG_Prescaler_4            ((uint32_t)0x00)
#define IWDG_Prescaler_8            ((uint32_t)0x01)
#define IWDG_Prescaler_16           ((uint32_t)0x02)
#define IWDG_Prescaler_32           ((uint32_t)0x03)
#define IWDG_Prescaler_64           ((uint32_t)0x04)
#define IWDG_Prescaler_128          ((uint32_t)0x05)
#define IWDG_Prescaler_256          ((uint32_t)0x06)
#define IS_IWDG_PRESCALER(PRESCALER) (((PRESCALER) == IWDG_Prescaler_4)  || \
                                      ((PRESCALER) == IWDG_Prescaler_8)  || \
                                      ((PRESCALER) == IWDG_Prescaler_16) || \
                                      ((PRESCALER) == IWDG_Prescaler_32) || \
                                      ((PRESCALER) == IWDG_Prescaler_64) || \
                                      ((PRESCALER) == IWDG_Prescaler_128)|| \
                                      ((PRESCALER) == IWDG_Prescaler_256))

/** @} */ /* End of group IWDG_prescaler */

/** @defgroup IWDG_Flag  IWDG Flags
  * @{
  */

#define IWDG_FLAG_PVU               IWDG_SR_PVU
#define IWDG_FLAG_RVU               IWDG_SR_RVU
#define IS_IWDG_FLAG(FLAG) (((FLAG) == IWDG_FLAG_PVU) || ((FLAG) == IWDG_FLAG_RVU))

/** @} */ /* End of group IWDG_Flag */

/** @defgroup IWDG_Reload  IWDG Reload
  * @{
  */

#define IS_IWDG_RELOAD(RELOAD) ((RELOAD) <= 0xFFF)

/** @} */ /* End of group IWDG_Reload */

/** @} */ /* End of group IWDG_Exported_Constants */

/** @defgroup IWDG_Exported_Macros IWDG Exported Macros
  * @{
  */

/** @} */ /* End of group IWDG_Exported_Macros */

/** @defgroup IWDG_Exported_Functions IWDG Exported Functions
  * @{
  */

void IWDG_WriteAccessEnable(void);
void IWDG_WriteAccessDisable(void);
void IWDG_SetPrescaler(uint32_t IWDG_Prescaler);
void IWDG_SetReload(uint32_t Reload);
void IWDG_ReloadCounter(void);
void IWDG_Enable(void);
FlagStatus IWDG_GetFlagStatus(uint32_t IWDG_FLAG);

/** @} */ /* End of group IWDG_Exported_Functions */

/** @} */ /* End of group IWDG */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32F9Qx_IWDG_H */

/*
*
* END OF FILE MDR32F9Qx_iwdg.h */
