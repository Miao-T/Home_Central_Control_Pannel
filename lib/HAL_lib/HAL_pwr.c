////////////////////////////////////////////////////////////////////////////////
/// @file     HAL_PWR.C
/// @author   MY Zhang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE PWR FIRMWARE FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define __HAL_PWR_C_

// Files includes  -------------------------------------------------------------
#include "hal_pwr.h"
#include "hal_rcc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Deinitializes the PWR peripheral registers to their default reset
/// values.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_DeInit(void)
{
    exRCC_APB1PeriphReset(RCC_APB1ENR_PWR);
}

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1)|| defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables access to the RTC and backup registers.
/// @param  state: new state of the access to the RTC and backup
///         registers. This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_BackupAccessCmd(FunctionalState state)
{
#if defined(PWR_CR_DBP)
    (state) ? (PWR->CR |= PWR_CR_DBP) : (PWR->CR &= ~PWR_CR_DBP);
#endif
#if defined(RCC_BDCR_DBP)
    //(state) ? (RCC->BDCR |= RCC_BDCR_DBP) : (RCC->BDCR &= ~RCC_BDCR_DBP);
#endif
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the Power Voltage Detector(PVD).
/// @param  state: new state of the PVD.
///         This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_PVDCmd(FunctionalState state)
{
#if defined(PWR_CR_PVDE)
    (state) ? (PWR->CR |= PWR_CR_PVDE) : (PWR->CR &= ~PWR_CR_PVDE);
#endif
#if defined(SYSCFG_PDETCSR_PVDE)
    (state) ? (SYSCFG->PDETCSR |= SYSCFG_PDETCSR_PVDE) : (SYSCFG->PDETCSR &= ~SYSCFG_PDETCSR_PVDE);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the voltage threshold detected by the Power Voltage
///         Detector(PVD).
/// @param  PWR_PVDLevel: specifies the PVD detection level
///         This parameter can be one of the following values:
/// @arg    PWR_PVDLevel_1V8: PVD detection level set to 2.8V
/// @arg    PWR_PVDLevel_2V1: PVD detection level set to 2.1V
/// @arg    PWR_PVDLevel_2V4: PVD detection level set to 2.4V
/// @arg    PWR_PVDLevel_2V7: PVD detection level set to 2.7V
/// @arg    PWR_PVDLevel_3V0: PVD detection level set to 3.0V
/// @arg    PWR_PVDLevel_3V3: PVD detection level set to 3.3V
/// @arg    PWR_PVDLevel_3V6: PVD detection level set to 3.6V
/// @arg    PWR_PVDLevel_3V9: PVD detection level set to 3.9V
/// @arg    PWR_PVDLevel_4V2: PVD detection level set to 4.2V
/// @arg    PWR_PVDLevel_4V5: PVD detection level set to 4.5V
/// @arg    PWR_PVDLevel_4V8: PVD detection level set to 4.8V
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_PVDLevelConfig(u32 PWR_PVDLevel)
{
#if defined(PWR_CR_PLS)
    PWR->CR = PWR->CR & (~PWR_CR_PLS) | PWR_PVDLevel;
#endif
#if defined(SYSCFG_PDETCSR_PLS)
    SYSCFG->PDETCSR = SYSCFG->PDETCSR & (~SYSCFG_PDETCSR_PLS) | PWR_PVDLevel;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the WakeUp Pin functionality.
/// @param  state: new state of the WakeUp Pin functionality.
///         This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_WakeUpPinCmd(FunctionalState state)
{
#if defined(PWR_CSR_EWUP)
    (state != DISABLE) ? (PWR->CSR |= PWR_CSR_EWUP) : (PWR->CSR &= ~PWR_CSR_EWUP);
#endif
#if defined(PWR_CSR_EWUP)
    (state != DISABLE) ? (PWR->CR2 |= PWR_CR2_EWUP1 | PWR_CR2_EWUP2 | PWR_CR2_EWUP3 | PWR_CR2_EWUP4 | PWR_CR2_EWUP5 | PWR_CR2_EWUP6) : (PWR->CR2 &= ~(PWR_CR2_EWUP1 | PWR_CR2_EWUP2 | PWR_CR2_EWUP3 | PWR_CR2_EWUP4 | PWR_CR2_EWUP5 | PWR_CR2_EWUP6));
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enters STOP mode.
/// @param  PWR_Regulator: specifies the regulator state in STOP mode.
///         This parameter can be one of the following values:
/// @arg    PWR_Regulator_ON: STOP mode with regulator ON
/// @arg    PWR_Regulator_LowPower: STOP mode with regulator in low power mode.
/// @param  PWR_STOPEntry: specifies if STOP mode in entered with WFI or  WFE
///         instruction.
///         This parameter can be one of the following values:
/// @arg    PWR_STOPEntry_WFI: enter STOP mode with WFI instruction
/// @arg    PWR_STOPEntry_WFE: enter STOP mode with WFE instruction
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_EnterSTOPMode(u32 PWR_Regulator, u8 PWR_STOPEntry)
{
    PWR->CR |= PWR_Regulator;

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    (PWR_STOPEntry == PWR_STOPEntry_WFI) ? __WFI() : __WFE();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enters STANDBY mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_EnterSTANDBYMode(void)
{
#if defined(PWR_CR_CWUF)
    PWR->CR |= PWR_CR_CWUF | PWR_CR_PDDS;
#endif
#if defined(PWR_SCR_CWUF1)
    PWR->CR |= PWR_CR_PDDS;
    PWR->SCR |= PWR_SCR_CWUF1 | PWR_SCR_CWUF2 | PWR_SCR_CWUF3 | PWR_SCR_CWUF4 | PWR_SCR_CWUF5 | PWR_SCR_CWUF6;
#endif
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

#if defined(__CC_ARM)
    __force_stores();
#endif
    __WFI();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified PWR flag is set or not.
/// @param  flag: specifies the flag to check.
///         This parameter can be one of the following values:
/// @arg    PWR_FLAG_WU: Wake Up flag
/// @arg    PWR_FLAG_SB: StandBy flag
/// @arg    PWR_FLAG_PVDO: PVD Output
/// @retval The new state of PWR_FLAG (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
FlagStatus PWR_GetFlagStatus(u32 flag)
{
    return (FlagStatus)(PWR->CSR & flag);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clears the PWR's pending flags.
/// @param  flag: specifies the flag to clear.
///         This parameter can be one of the following values:
/// @arg    PWR_FLAG_WU: Wake Up flag
/// @arg    PWR_FLAG_SB: StandBy flag
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_ClearFlag(u32 flag)
{
    PWR->CR |= flag << 2;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enters STANDBY mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exPWR_EnterLowPowerMode(emPWR_LP_Mode_Typedef lp_Mode, emPWR_Wait_Mode_Typedef wait_Mode)
{
	if (lp_Mode != LP_SLEEP_MODE) {
#if defined(PWR_CR_CWUF)
		PWR->CR |= lp_Mode | PWR_CR_CWUF;
#endif
#if defined(PWR_SCR_CWUF1)
        PWR->CR |= lp_Mode;
        PWR->SCR |= PWR_SCR_CWUF1 | PWR_SCR_CWUF2 | PWR_SCR_CWUF3 | PWR_SCR_CWUF4 | PWR_SCR_CWUF5 | PWR_SCR_CWUF6;
#endif
		SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    }
	if (wait_Mode == LP_WFE) {
		__WFE();		// sleep & stop & standby
		return;
	}
    else {
	__WFI();		// sleep & stop & standby
    }
}

/// @}

/// @}

/// @}
