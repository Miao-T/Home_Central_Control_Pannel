////////////////////////////////////////////////////////////////////////////////
/// @file     HAL_RTC.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE RTC FIRMWARE FUNCTIONS.
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
#define _HAL_RTC_C_

// Files includes  -------------------------------------------------------------
#include "hal_rtc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_Exported_Functions
/// @{

#if defined(__MM3N1) || defined(__MM0S1) || defined(__MM3U1)

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified RTC interrupts.
/// @param  it: specifies the RTC interrupts sources to be enabled or
///         disabled.
///         This parameter can be any combination of the following values:
/// @arg    RTC_IT_OW: Overflow interrupt
/// @arg    RTC_IT_ALR: Alarm interrupt
/// @arg    RTC_IT_SEC: Second interrupt
/// @param  state: new state of the specified RTC interrupts.
///         This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_ITConfig(RTC_IT_TypeDef it, FunctionalState state)
{
    (state == ENABLE) ? (RTC->CR |= it) : (RTC->CR &= (u16)~it);
	RTC_WaitForLastTask();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enters the RTC configuration mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_EnterConfigMode(void)
{
#if defined(PWR_CR_DBP)
    PWR->CR  &= ~PWR_CR_DBP;
#endif
#if defined(RCC_BDCR_DBP)
    RCC->BDCR  &= ~RCC_BDCR_DBP;
#endif
    RTC->CSR |= RTC_CSR_CNF;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Exits from the RTC configuration mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_ExitConfigMode(void)
{
    RTC->CSR &= ~RTC_CSR_CNF;
	while (!(RTC->CSR & RTC_CSR_RTOFF));
#if defined(PWR_CR_DBP)
    PWR->CR  &= ~PWR_CR_DBP;
#endif
#if defined(RCC_BDCR_DBP)
    RCC->BDCR  &= ~RCC_BDCR_DBP;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the RTC counter value.
/// @param  None.
/// @retval RTC counter value.
////////////////////////////////////////////////////////////////////////////////
u32 RTC_GetCounter(void)
{
    u32 dat = RTC->CNTH << 16;
    return (RTC->CNTL | dat);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the RTC counter value.
/// @param  count: RTC counter new value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_SetCounter(u32 count)
{
	RTC->CSR |= RTC_CSR_CNF;
	RTC->CNTH = count >> 16;
    RTC->CNTL = count;
	RTC->CSR &= ~RTC_CSR_CNF;
	while (!(RTC->CSR & RTC_CSR_RTOFF));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the RTC prescaler value.
/// @param  prescaler: RTC prescaler new value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_SetPrescaler(u32 prescaler)
{
	RTC->CSR |= RTC_CSR_CNF;
	RTC->PRLH = (prescaler & 0x000F0000) >> 16;
	RTC->PRLL = (prescaler & 0x0000FFFF);
	RTC->CSR &= ~RTC_CSR_CNF;
	while (!(RTC->CSR & RTC_CSR_RTOFF));
}

////////////////////////////////////////////////////////////////////////////////
// @brief  Sets the RTC alarm value.
// @param  alarm: RTC alarm new value.
// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_SetAlarm(u32 alarm)
{
	RTC->CSR |= RTC_CSR_CNF;
	RTC->ALRH = alarm >> 16;
	RTC->ALRL = alarm;
	RTC->CSR &= ~RTC_CSR_CNF;
	while (!(RTC->CSR & RTC_CSR_RTOFF));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the RTC divider value.
/// @param  None.
/// @retval RTC Divider value.
////////////////////////////////////////////////////////////////////////////////
u32 RTC_GetDivider(void)
{
	u32 dat = ((u32)(RTC->DIVH & RTC_DIVH_DIV) << 16);
    return (RTC->DIVL | dat);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Waits until last write operation on RTC registers has finished.
/// @note   This function must be called before any write to RTC registers.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_WaitForLastTask(void)
{
    while (!(RTC->CSR & RTC_CSR_RTOFF));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Waits until the RTC registers (RTC_CNT, RTC_ALR and RTC_PRL)
///         are synchronized with RTC APB clock.
/// @note   This function must be called before any read operation after an APB
///         reset or an APB clock stop.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_WaitForSynchro(void)
{
    RTC->CSR &= ~RTC_CSR_RSF;
    while (!(RTC->CSR & RTC_CSR_RSF));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified RTC flag is set or not.
/// @param  flag: specifies the flag to check.
///         This parameter can be one the following values:
/// @arg    RTC_FLAG_RTOFF: RTC Operation OFF flag
/// @arg    RTC_FLAG_RSF: Registers Synchronized flag
/// @arg    RTC_FLAG_OW: Overflow flag
/// @arg    RTC_FLAG_ALR: Alarm flag
/// @arg    RTC_FLAG_SEC: Second flag
/// @retval The state of RTC_FLAG (SET or RESET).
/////////////////////////////////////////////////////////////////////////////////
FlagStatus RTC_GetFlagStatus(RTC_FLAG_TypeDef flag)
{
	return  (FlagStatus)(RTC->CSR & flag);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clears the RTC's pending flags.
/// @param  flag: specifies the flag to clear.
///   		This parameter can be any combination of the following values:
/// @arg 	RTC_FLAG_RSF: Registers Synchronized flag. This flag is cleared only
///         after an APB reset or an APB Clock stop.
/// @arg 	RTC_FLAG_OW: Overflow flag
/// @arg 	RTC_FLAG_ALR: Alarm flag
/// @arg 	RTC_FLAG_SEC: Second flag
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_ClearFlag(RTC_FLAG_TypeDef flag)
{
    RTC->CSR &= ~flag;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified RTC interrupt has occurred or not.
/// @param  it: specifies the RTC interrupts sources to check.
///   		This parameter can be one of the following values:
/// @arg 	RTC_IT_OW: Overflow interrupt
/// @arg 	RTC_IT_ALR: Alarm interrupt
/// @arg 	RTC_IT_SEC: Second interrupt
/// @retval The state of the RTC_IT (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
ITStatus RTC_GetITStatus(RTC_IT_TypeDef it)
{
	return  (ITStatus)(RTC->CSR & it);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clears the RTC's interrupt pending bits.
/// @param  it: specifies the interrupt pending bit to clear.
///         This parameter can be any combination of the following values:
/// @arg    RTC_IT_OW: Overflow interrupt
/// @arg    RTC_IT_ALR: Alarm interrupt
/// @arg    RTC_IT_SEC: Second interrupt
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_ClearITPendingBit(RTC_IT_TypeDef it)
{
//	RTC->CSR |= RTC_CSR_CNF;
    RTC->CSR &= ~it;
//	RTC->CSR &= ~RTC_CSR_CNF;
}

#endif

/// @}

/// @}

/// @}
