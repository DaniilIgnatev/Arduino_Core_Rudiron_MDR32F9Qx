//Source: https://github.com/eldarkg/emdr1986x-std-per-lib

/**
  * FILE MDR32F9Qx_dac.h
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
#ifndef __MDR32F9QX_DAC_H
#define __MDR32F9QX_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_lib.h"

/** @addtogroup __MDR32F9Qx_StdPeriph_Driver MDR32F9Qx Standard Peripherial Driver
  * @{
  */

/** @addtogroup DAC
  * @{
  */

/** @defgroup DAC_Exported_Types DAC Exported Types
  * @{
  */

/** @} */ /* End of group DAC_Exported_Types */

/** @defgroup DAC_Exported_Constants DAC Exported Constants
  * @{
  */

/** @defgroup DAC_Synchronous_Mode DAC Synchronous Mode
  * @{
  */

#define DAC_SYNC_MODE_Independent             (((uint32_t)0x0) << DAC_CFG_SYNC_A_Pos)       /*!< The independent DAC1, DAC2 operation mode. */
#define DAC_SYNC_MODE_Synchronous             (((uint32_t)0x1) << DAC_CFG_SYNC_A_Pos)       /*!< The synchronous DAC1, DAC2 operation mode. */

#define IS_DAC_SYNC_MODE(MODE) (((MODE) == DAC_SYNC_MODE_Independent) || \
                                ((MODE) == DAC_SYNC_MODE_Synchronous))

/** @} */ /* End of group DAC_Synchronous_Mode */

/** @defgroup DAC1_Ref_Source DAC1 Reference Voltage source
  * @{
  */

#define DAC1_AVCC                             (((uint32_t)0x0) << DAC_CFG_M_REF0_Pos)       /*!< Selects AVcc as Reference Voltage. */
#define DAC1_REF                              (((uint32_t)0x1) << DAC_CFG_M_REF0_Pos)       /*!< Selects DAC1_REF input as Reference Voltage. */

#define IS_DAC1_REF_CONFIG(CONFIG) (((CONFIG) == DAC1_AVCC) || \
                                    ((CONFIG) == DAC1_REF ))

/** @} */ /* End of group DAC1_Ref_Source */

/** @defgroup DAC2_Ref_Source DAC2 Reference Voltage source
  * @{
  */

#define DAC2_AVCC                             (((uint32_t)0x0) << DAC_CFG_M_REF1_Pos)       /*!< Selects AVcc as Reference Voltage. */
#define DAC2_REF                              (((uint32_t)0x1) << DAC_CFG_M_REF1_Pos)       /*!< Selects DAC2_REF input as Reference Voltage. */

#define IS_DAC2_REF_CONFIG(CONFIG) (((CONFIG) == DAC2_AVCC) || \
                                    ((CONFIG) == DAC2_REF ))

/** @} */ /* End of group DAC2_Ref_Source */

/** @defgroup DAC_Data DAC Data
  * @{
  */

#define DAC_LO_DATA_MSK                       ((uint32_t)0x0FFF)
#define DAC_HI_DATA_MSK                       (DAC_LO_DATA_MSK << 16)

#define IS_DAC_DATA(DATA) (((DATA) & ~(DAC_LO_DATA_MSK | DAC_HI_DATA_MSK)) == 0 )

/** @} */ /* End of group DAC_Data */

/** @} */ /* End of group DAC_Exported_Constants */

/** @defgroup DAC_Exported_Macros DAC Exported Macros
  * @{
  */

/** @} */ /* End of group DAC_Exported_Macros */

/** @defgroup DAC_Exported_Functions DAC Exported Functions
  * @{
  */

void DAC_DeInit(void);

void DAC_Init(uint32_t SyncMode, uint32_t DAC1_Ref, uint32_t DAC2_Ref);

void DAC1_Init(uint32_t DAC1_Ref);
void DAC2_Init(uint32_t DAC2_Ref);

void DAC1_Cmd(FunctionalState NewState);
void DAC2_Cmd(FunctionalState NewState);

void DAC1_SetData(uint32_t Data);
void DAC2_SetData(uint32_t Data);

uint32_t DAC1_GetData(void);
uint32_t DAC2_GetData(void);

/** @} */ /* End of group DAC_Exported_Functions */

/** @} */ /* End of group DAC */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32F9QX_DAC_H */

/*
*
* END OF FILE MDR32F9Qx_dac.h */

