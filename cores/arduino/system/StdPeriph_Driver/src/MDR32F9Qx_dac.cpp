//Source: https://github.com/eldarkg/emdr1986x-std-per-lib

/**
  * FILE MDR32F9Qx_dac.c
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

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_dac.h"


#define ASSERT_INFO_FILE_ID FILEID__MDR32F9X_DAC_C

/** @addtogroup __MDR32F9Qx_StdPeriph_Driver MDR32F9Qx Standard Peripherial Driver
  * @{
  */

/** @defgroup DAC DAC
  * @{
  */

/** @defgroup DAC_Private_Functions DAC Private Functions
  * @{
  */

/**
  * @brief  Deinitializes the DAC peripheral registers to their default reset values.
  * @param  None.
  * @retval None.
  */
void DAC_DeInit(void)
{
  MDR_DAC->DAC1_DATA = 0;
  MDR_DAC->DAC2_DATA = 0;
  MDR_DAC->CFG = 0;
}

/**
  * @brief  Initializes the DAC peripheral.
  * @param  SyncMode: specifies the DAC1, DAC2 operation mode - synchronous or independent.
  *         This parameter can be one of the following values:
  *           @arg DAC_SYNC_MODE_Independent:   the independent DAC1, DAC2 operation mode;
  *           @arg DAC_SYNC_MODE_Synchronous:   the synchronous DAC1, DAC2 operation mode.
  * @param  DAC1_Ref: selects the DAC1 Reference Voltage source.
  *         This parameter can be one of the following values:
  *           @arg DAC1_AVCC:                   selects AVcc as Reference Voltage;
  *           @arg DAC1_REF :                   selects DAC1_REF input as Reference Voltage.
  * @param  DAC2_Ref: selects the DAC2 Reference Voltage source.
  *         This parameter can be one of the following values:
  *           @arg DAC2_AVCC:                   selects AVcc as Reference Voltage;
  *           @arg DAC2_REF :                   selects DAC2_REF input as Reference Voltage.
  * @retval None
  */
void DAC_Init(uint32_t SyncMode, uint32_t DAC1_Ref, uint32_t DAC2_Ref)
{
  uint32_t tmpreg_CFG;

  /* Check the parameters */
  assert_param(IS_DAC_SYNC_MODE(SyncMode));
  assert_param(IS_DAC1_REF_CONFIG(DAC1_Ref));
  assert_param(IS_DAC2_REF_CONFIG(DAC2_Ref));

  tmpreg_CFG = MDR_DAC->CFG;
  tmpreg_CFG &= ~(DAC_CFG_SYNC_A | DAC_CFG_M_REF0 | DAC_CFG_M_REF1);
  tmpreg_CFG += SyncMode + DAC1_Ref + DAC2_Ref;
  MDR_DAC->CFG = tmpreg_CFG;
}

/**
  * @brief  Initializes the DAC1 peripheral.
  * @param  DAC1_Ref: selects the DAC1 Reference Voltage source.
  *         This parameter can be one of the following values:
  *           @arg DAC1_AVCC:                   selects AVcc as Reference Voltage;
  *           @arg DAC1_REF :                   selects DAC1_REF input as Reference Voltage.
  * @retval None
  */
void DAC1_Init(uint32_t DAC1_Ref)
{
  uint32_t tmpreg_CFG;

  /* Check the parameters */
  assert_param(IS_DAC1_REF_CONFIG(DAC1_Ref));

  tmpreg_CFG = MDR_DAC->CFG;
  tmpreg_CFG &= ~DAC_CFG_M_REF0;
  tmpreg_CFG += DAC1_Ref;
  MDR_DAC->CFG = tmpreg_CFG;
}

/**
  * @brief  Initializes the DAC2 peripheral.
  * @param  DAC2_Ref: selects the DAC2 Reference Voltage source.
  *         This parameter can be one of the following values:
  *           @arg DAC2_AVCC:                   selects AVcc as Reference Voltage;
  *           @arg DAC2_REF :                   selects DAC2_REF input as Reference Voltage.
  * @retval None
  */
void DAC2_Init(uint32_t DAC2_Ref)
{
  uint32_t tmpreg_CFG;

  /* Check the parameters */
  assert_param(IS_DAC2_REF_CONFIG(DAC2_Ref));

  tmpreg_CFG = MDR_DAC->CFG;
  tmpreg_CFG &= ~DAC_CFG_M_REF1;
  tmpreg_CFG += DAC2_Ref;
  MDR_DAC->CFG = tmpreg_CFG;
}

/**
  * @brief  Enables or disables the DAC1 peripheral.
  * @param  NewState: new state of the DAC1 peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DAC1_Cmd(FunctionalState NewState)
{
  uint32_t tmpreg_CFG;

  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  tmpreg_CFG = MDR_DAC->CFG;

  /* Form new value */
  if (NewState != DISABLE)
  {
    /* Enable DAC1 by setting the Cfg_ON_DAC0 bit in the DAC_CFG register */
    tmpreg_CFG |= DAC_CFG_ON_DAC0;
  }
  else
  {
    /* Disable DAC1 by resetting the Cfg_ON_DAC0 bit in the DAC_CFG register */
    tmpreg_CFG &= ~DAC_CFG_ON_DAC0;
  }

  /* Configure DAC_CFG register with new value */
  MDR_DAC->CFG = tmpreg_CFG;
}

/**
  * @brief  Enables or disables the DAC1 peripheral.
  * @param  NewState: new state of the DAC1 peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DAC2_Cmd(FunctionalState NewState)
{
  uint32_t tmpreg_CFG;

  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  tmpreg_CFG = MDR_DAC->CFG;

  /* Form new value */
  if (NewState != DISABLE)
  {
    /* Enable DAC2 by setting the Cfg_ON_DAC1 bit in the DAC_CFG register */
    tmpreg_CFG |= DAC_CFG_ON_DAC1;
  }
  else
  {
    /* Disable DAC2 by resetting the Cfg_ON_DAC1 bit in the DAC_CFG register */
    tmpreg_CFG &= ~DAC_CFG_ON_DAC1;
  }

  /* Configure DAC_CFG register with new value */
  MDR_DAC->CFG = tmpreg_CFG;
}

/**
  * @brief  Sets the DAC1 (and DAC2 in Synchronous mode) output data.
  * @param  Data: specifies the DAC output data.
  * @retval None
  */
void DAC1_SetData(uint32_t Data)
{
  /* Check the parameters */
  assert_param(IS_DAC_DATA(Data));

  MDR_DAC->DAC1_DATA = Data;
}

/**
  * @brief  Sets the DAC2 (and DAC1 in Synchronous mode) output data.
  * @param  Data: specifies the DAC output data.
  * @retval None
  */
void DAC2_SetData(uint32_t Data)
{
  /* Check the parameters */
  assert_param(IS_DAC_DATA(Data));

  MDR_DAC->DAC2_DATA = Data;
}

/**
  * @brief  Returns the DAC1 output data.
  * @param  None.
  * @retval Counter Register value.
  */
uint32_t DAC1_GetData(void)
{
  return MDR_DAC->DAC1_DATA;
}

/**
  * @brief  Returns the DAC2 output data.
  * @param  None.
  * @retval Counter Register value.
  */
uint32_t DAC2_GetData(void)
{
  return MDR_DAC->DAC2_DATA;
}

/** @} */ /* End of group DAC_Private_Functions */

/** @} */ /* End of group DAC */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Driver */

/*
*
* END OF FILE MDR32F9Qx_dac.c */

