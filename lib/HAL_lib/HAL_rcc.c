////////////////////////////////////////////////////////////////////////////////
/// @file     HAL_RCC.C
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE RCC FIRMWARE FUNCTIONS.
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
#define _HAL_RCC_C_

// Files includes  -------------------------------------------------------------
#include "hal_rcc.h"
#include "common.h"
u8 tbPresc[] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RCC_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RCC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Resets the RCC clock configuration to default state.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_DeInit()
{
    SET_BIT(RCC->CR, RCC_CR_HSION);
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1)
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_PLLDIV | RCC_CR_PLLMUL);
#endif
#if defined(__MM3U1)
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON );
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLMUL | RCC_PLLCFGR_PLLDIV);
#endif
#if defined(__MM0Q1) || defined(__MM0T1)
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
#endif
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    CLEAR_REG(RCC->CFGR);
    CLEAR_REG(RCC->CIR);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the External High Speed oscillator (HSE).
/// @param  state: specifies the new state of HSE.
///   This parameter can be one of the following values:
/// @arg RCC_HSE_OFF: HSE oscillator OFF
/// @arg RCC_HSE_ON: HSE oscillator ON
/// @arg RCC_HSE_Bypass: HSE oscillator bypassed with external
///   clock
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_HSEConfig(RCCHSE_TypeDef state)
{
    RCC->CR &= ~(RCC_CR_HSEBYP | RCC_CR_HSEON);
    switch (state) {
        case RCC_HSE_Bypass: RCC->CR |= RCC_CR_HSEBYP;
        case RCC_HSE_ON: RCC->CR |= RCC_CR_HSEON;
        default: break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified RCC flag is set or not.
/// @param  flag: specifies the flag to check.
///   This parameter can be one of the following values:
/// @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
/// @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
/// @arg RCC_FLAG_PLLRDY: PLL clock ready
/// @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
/// @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
/// @arg RCC_FLAG_PINRST: Pin reset
/// @arg RCC_FLAG_PORRST: POR/PDR reset
/// @arg RCC_FLAG_SFTRST: Software reset
/// @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
/// @arg RCC_FLAG_WWDGRST: Window Watchdog reset
/// @arg RCC_FLAG_LPWRRST: Low Power reset
/// @retval The new state of flag (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
FlagStatus RCC_GetFlagStatus(RCC_FLAG_TypeDef flag)
{
#if defined(__MM3N1) || defined(__MM3O1) || defined(__MM0S1) || defined(__MM3U1)
    return ((((flag >> 5) == CR_REG_INDEX) ? RCC->CR : (((flag >> 5) == BDCR_REG_INDEX) ? RCC->BDCR : RCC->CSR)) &
            (1 << (flag & 0x1F)))
               ? SET : RESET;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0T1)
    return ((((flag >> 5) == CR_REG_INDEX) ? RCC->CR : (((flag >> 5) == BDCR_REG_INDEX) ? 0 : RCC->CSR)) &
            (1 << (flag & 0x1F)))
               ? SET : RESET;
#endif
}

#if defined(__MM3O1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Adjusts the Internal High Speed oscillator (HSI) calibration value.
/// @param HSICalibrationValue: specifies the calibration trimming value.
///   This parameter must be a number between 0 and 0x1F.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue)
{

    RCC->CR &= RCC_CR_HSITRIM;
    RCC->CR |= (HSICalibrationValue << 3);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Waits for HSE start-up.
/// @param  None.
/// @retval  An ErrorStatus enumuration value:
/// - SUCCESS: HSE oscillator is stable and ready to use
/// - ERROR: HSE oscillator not yet ready
////////////////////////////////////////////////////////////////////////////////
ErrorStatus RCC_WaitForHSEStartUp(void)
{
    u32 StartUpCounter = 0;

    FlagStatus HSEStatus;

    do {
        HSEStatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
        StartUpCounter++;
    } while ((HSEStatus == RESET) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    return (ErrorStatus)(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET) ? SUCCESS : ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Waits for flag start-up.
/// @param  flag: specifies the flag to check.
///   This parameter can be one of the following values:
/// @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
/// @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
/// @arg RCC_FLAG_PLLRDY: PLL clock ready
/// @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
/// @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
/// @arg RCC_FLAG_PINRST: Pin reset
/// @arg RCC_FLAG_PORRST: POR/PDR reset
/// @arg RCC_FLAG_SFTRST: Software reset
/// @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
/// @arg RCC_FLAG_WWDGRST: Window Watchdog reset
/// __MM3O1
/// @arg RCC_FLAG_LPWRST :Low power reset
/// @retval  An ErrorStatus enumuration value:
/// - SUCCESS: HSE oscillator is stable and ready to use
/// - ERROR: HSE oscillator not yet ready
////////////////////////////////////////////////////////////////////////////////
ErrorStatus RCC_WaitForFlagStartUp(RCC_FLAG_TypeDef flag)
{
    u32 StartUpCounter = 0;

    while (RCC_GetFlagStatus(flag) == RESET) {
        if (StartUpCounter++ > HSE_STARTUP_TIMEOUT) {
            return ERROR;
        }
    }
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the Internal High Speed oscillator (HSI).
/// @param  state: new state of the HSI.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_HSICmd(FunctionalState state)
{
    MODIFY_REG(RCC->CR, RCC_CR_HSION, (state << RCC_CR_HSION_Pos));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the system clock (SYSCLK).
/// @param  sysClkSrc: specifies the clock source used as system
///   clock. This parameter can be one of the following values:
/// @arg RCC_HSI: specifies HSI as system clock
/// @arg RCC_HSE: specifies HSE as system clock
/// @arg RCC_PLL: specifies PLL as system clock
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_SYSCLKConfig(SYSCLK_TypeDef sysClkSrc)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, (sysClkSrc << RCC_CFGR_SW_Pos));
}

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the PLL clock source and DM DN factor.
///   This function must be used only when the PLL is disabled.
/// @param  plldn: specifies the PLL multiplication factor.
///   This parameter can be RCC_PLLMul_x where x:[31:26]
/// @param  plldm: specifies the PLL Divsior factor.
///   This parameter can be RCC_Divsior_x where x:[22:20]
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_PLLDMDNConfig(u32 plldn, u32 plldm)
{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1)
    MODIFY_REG(RCC->CR, (RCC_CR_PLLMUL | RCC_CR_PLLDIV), ((plldn << RCC_CR_PLLMUL_Pos) | (plldm << RCC_CR_PLLDIV_Pos)));
#endif
#if defined(__MM3U1)
    MODIFY_REG(RCC->PLLCFGR, (RCC_PLLCFGR_PLLMUL | RCC_PLLCFGR_PLLDIV), ((plldn << RCC_PLLCFGR_PLLMUL_Pos) | (plldm << RCC_PLLCFGR_PLLDIV_Pos)));
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the PLL.
///   The PLL can not be disabled if it is used as system clock.
/// @param  state: new state of the PLL.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_PLLCmd(FunctionalState state)
{
    MODIFY_REG(RCC->CR, RCC_CR_PLLON, (state << RCC_CR_PLLON_Pos));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the PLL clock source and multiplication factor.
///   This function must be used only when the PLL is disabled.
/// @param  pllSrc: specifies the PLL entry clock source.
///   This parameter can be one of the following values:
/// @arg RCC_HSI_Div4: HSI oscillator clock divided
///   by 4 selected as PLL clock entry
/// @arg RCC_HSE_Div1: HSE oscillator clock selected
///   as PLL clock entry
/// @arg RCC_HSE_Div2: HSE oscillator clock divided
///   by 2 selected as PLL clock entry
/// @param  pllMul: specifies the PLL multiplication factor.
///   This parameter can be RCC_PLLMul_x where x:[31:26][22:20]
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_PLLConfig(RCC_PLLSource_TypeDef pllSrc, RCC_PLLMul_TypeDef pllMul)
{
#if defined(RCC_CFGR_PLLSRC)
    MODIFY_REG(RCC->CFGR, (RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC), pllSrc);
#endif
#if defined(RCC_PLLCFGR_PLLSRC)
    MODIFY_REG(RCC->PLLCFGR, (RCC_PLLCFGR_PLLXTPRE | RCC_PLLCFGR_PLLSRC), pllSrc);
#endif

    u8 DNDM_Item[] = {0x07, 0x03, 0x05, 0x01, 0x07, 0x01, 0x09, 0x01,  // Frclk*8/4 ; Frclk*6/2 ; Frclk*8/2 ; Frclk*10/2;
                      0x0B, 0x01, 0x0D, 0x01, 0x0F, 0x01, 0x11, 0x01,  // Frclk*12/2; Frclk*14/2; Frclk*16/2; Frclk*18/2;
                      0x13, 0x01, 0x15, 0x01, 0x17, 0x01, 0x19, 0x01,  // Frclk*20/2; Frclk*22/2; Frclk*24/2; Frclk*26/2;
                      0x1B, 0x01, 0x1D, 0x01, 0x1F, 0x01};             // Frclk*28/2; Frclk*30/2;
                                                                       // Frclk*32/2;
#if defined(__MM3U1)
    MODIFY_REG(RCC->PLLCFGR, (RCC_PLLCFGR_PLLXTPRE | RCC_PLLCFGR_PLLSRC), pllSrc);
#endif
    RCC_PLLDMDNConfig((u32)DNDM_Item[pllMul >> 17], (u32)DNDM_Item[(pllMul >> 17) + 1]);
}
#endif

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the USB clock (USBCLK).
/// @param  usbClkSrc: specifies the USB clock source.
///   This clock is derived from the PLL output.
///   This parameter can be one of the following values:
/// @arg RCC_USBCLKSource_PLLCLK_Div1: PLL clock selected as USB clock source
/// @arg RCC_USBCLKSource_PLLCLK_Div2: PLL clock divided by 2 selected as USB
/// clock source
/// @arg RCC_USBCLKSource_PLLCLK_Div3: PLL clock divided by 3 selected as USB
/// clock source
/// @arg RCC_USBCLKSource_PLLCLK_Div4: PLL clock divided by 4 selected as USB
/// clock source
/// @arg RCC_USBCLKSource_PLLCLK_Div5: PLL clock divided by 5 selected as USB
/// clock source
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_USBCLKConfig(RCC_USBCLKSOURCE_TypeDef usbClkSrc)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_USBPRE, (usbClkSrc << RCC_CFGR_USBPRE_Pos));
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the clock source used as system clock.
/// @param  None.
/// @retval The clock source used as system clock. The returned value can
///   be one of the following:
/// - 0x00: HSI/6 used as system clock
/// - 0x04: HSE used as system clock
/// - 0x08: PLL used as system clock
////////////////////////////////////////////////////////////////////////////////
u8 RCC_GetSYSCLKSource(void)
{
    return ((u8)READ_BIT(RCC->CFGR, RCC_CFGR_SWS));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the AHB clock (HCLK).
/// @param  sysClk: defines the AHB clock divider. This clock is derived
///                    from the system clock (SYSCLK).
///   This parameter can be one of the following values:
/// @arg RCC_SYSCLK_Div1: AHB clock = SYSCLK
/// @arg RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
/// @arg RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
/// @arg RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
/// @arg RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
/// @arg RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
/// @arg RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
/// @arg RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
/// @arg RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_HCLKConfig(RCC_AHB_CLK_TypeDef sysClk)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, sysClk);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the Low Speed APB clock (PCLK1).
/// @param  HCLK: defines the APB1 clock divider. This clock is derived from
///                  the AHB clock (HCLK).
///   This parameter can be one of the following values:
/// @arg RCC_HCLK_Div1: APB1 clock = HCLK
/// @arg RCC_HCLK_Div2: APB1 clock = HCLK/2
/// @arg RCC_HCLK_Div4: APB1 clock = HCLK/4
/// @arg RCC_HCLK_Div8: APB1 clock = HCLK/8
/// @arg RCC_HCLK_Div16: APB1 clock = HCLK/16
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_PCLK1Config(RCC_APB1_APB2_CLK_TypeDef HCLK)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, HCLK);
}

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1)  || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the High Speed APB clock (PCLK2).
/// @param  HCLK: defines the APB2 clock divider. This clock is derived from
///                  the AHB clock (HCLK).
///   This parameter can be one of the following values:
/// @arg RCC_HCLK_Div1: APB2 clock = HCLK
/// @arg RCC_HCLK_Div2: APB2 clock = HCLK/2
/// @arg RCC_HCLK_Div4: APB2 clock = HCLK/4
/// @arg RCC_HCLK_Div8: APB2 clock = HCLK/8
/// @arg RCC_HCLK_Div16: APB2 clock = HCLK/16
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_PCLK2Config(RCC_APB1_APB2_CLK_TypeDef HCLK)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (HCLK << 3));
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the ADC clock (ADCCLK).
/// @param  PCLK2: defines the ADC clock divider. This clock is derived from
///                   the APB2 clock (PCLK2).
///   This parameter can be one of the following values:
/// @arg RCC_PCLK2_Div2: ADC clock = PCLK2/2
/// @arg RCC_PCLK2_Div4: ADC clock = PCLK2/4
/// @arg RCC_PCLK2_Div6: ADC clock = PCLK2/6
/// @arg RCC_PCLK2_Div8: ADC clock = PCLK2/8
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_ADCCLKConfig(RCC_ADCCLKSOURCE_TypeDef PCLK2)
{
    MODIFY_REG(RCC->CFGR, ADC_CFGR_PRE, PCLK2);
}

#if defined(__MM3N1) || defined(__MM3O1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the External Low Speed oscillator (LSE).
/// @param  state: specifies the new state of the LSE.
///   This parameter can be one of the following values:
/// @arg RCC_LSE_OFF: LSE oscillator OFF
/// @arg RCC_LSE_ON: LSE oscillator ON
/// @arg RCC_LSE_Bypass: LSE oscillator bypassed with external
///   clock
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_LSEConfig(RCC_LSE_TypeDef state)
{
    RCC->BDCR &= ~(RCC_BDCR_LSEBYP | RCC_BDCR_LSEON);

    switch (state) {
        case RCC_LSE_Bypass: RCC->BDCR |= RCC_BDCR_LSEBYP;

        case RCC_LSE_ON: RCC->BDCR |= RCC_BDCR_LSEON;

        default: break;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the RTC clock (RTCCLK).
///   Once the RTC clock is selected it can be changed unless the
///   Backup domain is reset.
/// @param  rtcClkSrc: specifies the RTC clock source.
///   This parameter can be one of the following values:
/// @arg RCC_RTCCLKSource_LSE: LSE selected as RTC clock
/// @arg RCC_RTCCLKSource_LSI: LSI selected as RTC clock
/// @arg RCC_RTCCLKSource_HSE_Div128: HSE clock divided by 128
///   selected as RTC clock
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_RTCCLKConfig(RCC_RTCCLKSOURCE_TypeDef rtcClkSrc)
{
    MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCSRC, rtcClkSrc);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the RTC clock.
///   This function must be used only after the RTC clock was
///   selected using the RCC_RTCCLKConfig function.
/// @param  state: new state of the RTC clock.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_RTCCLKCmd(FunctionalState state)
{
    MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCEN, (state << RCC_BDCR_RTCEN_Pos));
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the Internal Low Speed oscillator (LSI).
///   LSI can not be disabled if the IWDG is running.
/// @param  state: new state of the LSI.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_LSICmd(FunctionalState state)
{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1)
    MODIFY_REG(RCC->CSR, RCC_CSR_LSION, (state << RCC_CSR_LSION_Pos));
#endif
#if defined(__MM3U1)
    MODIFY_REG(RCC->CSR, RCC_CSR_LSION | RCC_CSR_LSIOENLV, RCC_CSR_LSIOENLV | (state << RCC_CSR_LSION_Pos));
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the clock frequency of different on chip clocks.
/// @param  None.
/// @retval sysClk : System clock frequency
////////////////////////////////////////////////////////////////////////////////
u32 RCC_GetSysClockFreq(void)
{
#if defined(__MM3U1)
        u32 result;
    u32 clock, mul, div;
    switch (RCC->CFGR & RCC_CFGR_SWS) {
        case RCC_CFGR_SWS_LSI:
            result = 40000U;//LSI_VALUE;
            break;

        case RCC_CFGR_SWS_HSE:
            result = 8000000U;//HSE_VALUE;
            break;

        case RCC_CFGR_SWS_PLL:
            clock = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC) ? (READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLXTPRE) ? (HSE_VALUE >> 1) : 8000000U)
                    : 8000000U;//HSI_VALUE_PLL_ON;
            mul = ((RCC->PLLCFGR & (u32)RCC_PLLCFGR_PLLMUL) >> RCC_PLLCFGR_PLLMUL_Pos) + 1;
            div = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLDIV) >> RCC_PLLCFGR_PLLDIV_Pos) + 1;

            result = clock * mul / div;
            break;
        default:
            result =  8000000U;//HSI_VALUE;
            break;
    }
    return result;
#else
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1)
    u32 clock, mul, div;
#endif
    switch (RCC->CFGR & RCC_CFGR_SWS) {
#if defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM0T1)
		case RCC_CFGR_SWS_LSI:
			return LSI_VALUE;
#endif

		case RCC_CFGR_SWS_HSE:
			return HSE_VALUE;

#if defined(__MM0Q1)
		case RCC_CFGR_SWS_HSI:
			return (RCC->CR & RCC_CR_HSI_72M) ? HSI_72MHz : HSI_48MHz;
#endif
#if defined(__MM0T1)
		case RCC_CFGR_SWS_HSI:
			return HSI_48MHz;
#endif

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1)
		case RCC_CFGR_SWS_PLL:
			clock = READ_BIT(RCC->CFGR, RCC_CFGR_PLLSRC) ? (READ_BIT(RCC->CFGR, RCC_CFGR_PLLXTPRE) ? (HSE_VALUE >> 1) : HSE_VALUE)
				: HSI_VALUE_PLL_ON;
			mul = ((RCC->CR & (u32)RCC_CR_PLLMUL) >> RCC_CR_PLLMUL_Pos) + 1;
			div = ((RCC->CR & RCC_CR_PLLDIV) >> RCC_CR_PLLDIV_Pos) + 1;

			return  clock * mul / div;
#endif

		default:
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1) || defined(__MM0T1)
			return HSI_DIV6;
#endif
#if defined(__MM0Q1)
			return (RCC->CR & RCC_CR_HSI_72M) ? HSI_72MHz_DIV6 : HSI_DIV6;
#endif
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the HCLK frequency of different on chip clocks.
/// @param  None.
/// @retval HCLK frequency
////////////////////////////////////////////////////////////////////////////////
u32 RCC_GetHCLKFreq(void)
{
    return (RCC_GetSysClockFreq() >> tbPresc[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos]);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the PCLK1 frequency of different on chip clocks.
/// @param  None.
/// @retval PCLK1 frequency
////////////////////////////////////////////////////////////////////////////////
u32 RCC_GetPCLK1Freq(void)
{
    return (RCC_GetHCLKFreq() >> tbPresc[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]);
}

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the PCLK2 frequency of different on chip clocks.
/// @param  None.
/// @retval PCLK2 frequency
////////////////////////////////////////////////////////////////////////////////
u32 RCC_GetPCLK2Freq(void)
{
    return (RCC_GetHCLKFreq() >> tbPresc[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos]);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Returns the frequency of different on chip clocks.
/// @param  clk: pointer to a RCC_ClocksTypeDef structure which
///   will hold the clocks frequency.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_GetClocksFreq(RCC_ClocksTypeDef* clk)
{
    u8 tbADCPresc[] = {2, 4, 6, 8};

    clk->SYSCLK_Frequency = RCC_GetSysClockFreq();
    clk->HCLK_Frequency   = RCC_GetHCLKFreq();
    clk->PCLK1_Frequency  = RCC_GetPCLK1Freq();
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
    clk->PCLK2_Frequency  = RCC_GetPCLK2Freq();
#endif

    clk->ADCCLK_Frequency = clk->PCLK2_Frequency / tbADCPresc[(RCC->CFGR & ADC_CFGR_PRE) >> ADC_CFGR_PRE_Pos];
}


#if defined(__MM3O1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
void RCC_AHB1PeriphClockCmd(u32 RCC_AHB1Periph, FunctionalState state)
{
    (state) ? (RCC->AHB1ENR |= RCC_AHB1Periph) : (RCC->AHB1ENR &= ~RCC_AHB1Periph);
}

////////////////////////////////////////////////////////////////////////////////
void RCC_AHB2PeriphClockCmd(u32 RCC_AHB2Periph, FunctionalState state)
{
    (state) ? (RCC->AHB2ENR |= RCC_AHB2Periph) : (RCC->AHB2ENR &= ~RCC_AHB2Periph);
}

////////////////////////////////////////////////////////////////////////////////
void RCC_AHB3PeriphClockCmd(u32 RCC_AHB3Periph, FunctionalState state)
{
    (state) ? (RCC->AHB3ENR |= RCC_AHB3Periph) : (RCC->AHB2ENR &= ~RCC_AHB3Periph);
}
#endif

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1)  || defined(__MM0T1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the AHB peripheral clock.
/// @param  AHBPeriph: specifies the AHB peripheral to gates its clock.
///   This parameter can be any combination of the following values:
/// __MM3N1
/// @arg RCC_AHB1ENR_DMA1, RCC_AHB1ENR_SRAM, RCC_AHB1ENR_FLITF,
///      RCC_AHB1ENR_CRC, RCC_AHB1ENR_AES
/// __MM0N1
/// @arg RCC_AHB1ENR_DMA1, RCC_AHB1ENR_SRAM, RCC_AHB1ENR_FLITF,
///      RCC_AHB1ENR_CRC, RCC_AHB1ENR_AES, RCC_AHB1ENR_GPIOA,
///      RCC_AHB1ENR_GPIOB, RCC_AHB1ENR_GPIOC, RCC_AHB1ENR_GPIOD,
///      RCC_AHB1ENR_GPIOE
/// __MM0P1
/// @arg RCC_AHB1ENR_DMA1, RCC_AHB1ENR_SRAM, RCC_AHB1ENR_FLITF,
///      RCC_AHB1ENR_CRC, RCC_AHB1ENR_GPIOA, RCC_AHB1ENR_GPIOB,
///      RCC_AHB1ENR_GPIOC, RCC_AHB1ENR_GPIOD, RCC_AHB1ENR_HWSQRT,
///      RCC_AHB1ENR_HWDIV
/// __MM0Q1
/// @arg RCC_AHB1ENR_DMA1, RCC_AHB1ENR_SRAM, RCC_AHB1ENR_FLITF,
///      RCC_AHB1ENR_CRC, RCC_AHB1ENR_GPIOA, RCC_AHB1ENR_GPIOB,
///      RCC_AHB1ENR_GPIOC, RCC_AHB1ENR_GPIOD, RCC_AHB1ENR_HWDIV
/// @param  state: new state of the specified peripheral clock.
///   This parameter can be: ENABLE or DISABLE.
/// __MM0S1
/// @arg RCC_AHB1ENR_DMA1, RCC_AHB1ENR_SRAM, RCC_AHB1ENR_FLASH,
///      RCC_AHB1ENR_CRC, RCC_AHB1ENR_AES, RCC_AHB1ENR_GPIOA,
///      RCC_AHB1ENR_GPIOB, RCC_AHB1ENR_GPIOC, RCC_AHB1ENR_GPIOD,
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_AHBPeriphClockCmd(u32 AHBPeriph, FunctionalState state)
{
    (state) ? (RCC->AHB1ENR |= AHBPeriph) : (RCC->AHB1ENR &= ~AHBPeriph);
}
#endif

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the High Speed APB (APB2) peripheral clock.
/// @param  APB2Periph: specifies the APB2 peripheral to gates its
///   clock.
///   This parameter can be any combination of the following values:
/// __MM3N1
/// @arg RCC_APB2ENR_EXTI, RCC_APB2ENR_GPIOA, RCC_APB2ENR_GPIOB,
/// 	 RCC_APB2ENR_GPIOC, RCC_APB2ENR_GPIOD, RCC_APB2ENR_GPIOE,
/// 	 RCC_APB2ENR_ADC1, RCC_APB2ENR_ADC2, RCC_APB2ENR_TIM1,
/// 	 RCC_APB2ENR_SPI1, RCC_APB2ENR_UART1, RCC_APB2ENR_COMP
/// __MM0N1
/// @arg RCC_APB2ENR_EXTI, RCC_APB2ENR_ADC1, RCC_APB2ENR_TIM1,
/// 	 RCC_APB2ENR_SPI1, RCC_APB2ENR_UART1, RCC_APB2ENR_COMP,
/// 	 RCC_APB2ENR_TIM14, RCC_APB2ENR_TIM16, RCC_APB2ENR_TIM17,
/// 	 RCC_APB2ENR_DBGMCU
/// __MM3O1
/// @arg RCC_APB2ENR_TIM1, RCC_APB2ENR_TIM8,RCC_APB2ENR_UART1,
///      RCC_APB2ENR_UART6, RCC_APB2ENR_ADC1, RCC_APB2ENR_SPI1,
///      RCC_APB2ENR_EXTI, RCC_APB2ENR_COMP
/// __MM0P1
/// @arg RCC_APB2ENR_EXTI, RCC_APB2ENR_ADC1, RCC_APB2ENR_ADC2,
///      RCC_APB2ENR_TIM1, RCC_APB2ENR_SPI1, RCC_APB2ENR_TIM8,
///      RCC_APB2ENR_UART1, RCC_APB2ENR_COMP, RCC_APB2ENR_TIM14,
///      RCC_APB2ENR_TIM16, RCC_APB2ENR_TIM17, RCC_APB2ENR_DBGMCU,
///      RCC_APB2RSTR_PWM
/// __MM0Q1
/// @arg RCC_APB2ENR_EXTI, RCC_APB2ENR_ADC1, RCC_APB2ENR_TIM1,
/// 	 RCC_APB2ENR_SPI1, RCC_APB2ENR_UART1, RCC_APB2ENR_COMP,
/// 	 RCC_APB2ENR_TIM14, RCC_APB2ENR_TIM16, RCC_APB2ENR_TIM17,
/// 	 RCC_APB2ENR_DBGMCU, RCC_APB2ENR_PWM
/// __MM0S1
/// @arg RCC_APB2ENR_EXTI, RCC_APB2ENR_ADC1, RCC_APB2ENR_TIM1,
/// 	 RCC_APB2ENR_SPI1, RCC_APB2ENR_UART1, RCC_APB2ENR_COMP,
/// 	 RCC_APB2ENR_TIM14, RCC_APB2ENR_TIM16, RCC_APB2ENR_TIM17,
/// 	 RCC_APB2ENR_DBGMCU
/// @param  state: new state of the specified peripheral clock.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_APB2PeriphClockCmd(u32 APB2Periph, FunctionalState state)
{
    (state) ? (RCC->APB2ENR |= APB2Periph) : (RCC->APB2ENR &= ~APB2Periph);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the Low Speed APB (APB1) peripheral clock.
/// @param  APB1Periph: specifies the APB1 peripheral to gates its
///   clock.
///   This parameter can be any combination of the following values:
/// __MM3N1
/// @arg RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, RCC_APB1ENR_TIM4,
///   	 RCC_APB1ENR_WWDG, RCC_APB1ENR_SPI2, RCC_APB1ENR_UART2,
///		 RCC_APB1ENR_UART3, RCC_APB1ENR_I2C1, RCC_APB1ENR_I2C2,
///   	 RCC_APB1ENR_USB, RCC_APB1ENR_CAN, RCC_APB1ENR_BKP,
///   	 RCC_APB1ENR_PWR, RCC_APB1ENR_DAC,
/// __MM0N1
/// @arg RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, RCC_APB1ENR_WWDG,
///		 RCC_APB1ENR_SPI2, RCC_APB1ENR_UART2, RCC_APB1ENR_I2C1,
///   	 RCC_APB1ENR_USB, RCC_APB1ENR_CAN, RCC_APB1ENR_CRS,
///   	 RCC_APB1ENR_PWR
/// __MM3O1
/// @arg RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, RCC_APB1ENR_TIM4,
///   	 RCC_APB1ENR_TIM5, RCC_APB1ENR_TIM6, RCC_APB1ENR_TIM7,
///		 RCC_APB1ENR_WWDG, RCC_APB1ENR_SPI2, RCC_APB1ENR_SPI3,
///      RCC_APB1ENR_UART2, RCC_APB1ENR_UART3, RCC_APB1ENR_UART4,
///      RCC_APB1ENR_UART5, RCC_APB1ENR_I2C1, RCC_APB1ENR_I2C2,
///   	 RCC_APB1ENR_CRS, RCC_APB1ENR_CAN1, RCC_APB1ENR_BKP,
///   	 RCC_APB1ENR_PWR, RCC_APB1ENR_UART7, RCC_APB1ENR_UART8
/// __MM0P1
/// @arg RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, RCC_APB1ENR_WWDG,
///		 RCC_APB1ENR_SPI2, RCC_APB1ENR_UART2, RCC_APB1ENR_I2C1,
///   	 RCC_APB1ENR_PWR
/// __MM0Q1
/// @arg RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, RCC_APB1ENR_WWDG,
///		 RCC_APB1ENR_SPI2, RCC_APB1ENR_UART2, RCC_APB1ENR_I2C1,
///   	 RCC_APB1ENR_PWR
/// __MM0S1
/// @arg RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, RCC_APB1ENR_WWDG,
///		 RCC_APB1ENR_SPI2, RCC_APB1ENR_UART2, RCC_APB1ENR_I2C1,
///   	 RCC_APB1ENR_USB, RCC_APB1ENR_CAN, RCC_APB1ENR_PWR
/// @param  state: new state of the specified peripheral clock.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_APB1PeriphClockCmd(u32 APB1Periph, FunctionalState state)
{
    (state) ? (RCC->APB1ENR |= APB1Periph) : (RCC->APB1ENR &= ~APB1Periph);
}

#if defined(__MM3O1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases AHB3 peripheral reset.
/// @brief  Forces or releases AHB3 peripheral reset.
/// @param RCC_AHB3Periph: specifies the AHB3 peripheral to reset.
///   This parameter can be any combination of the following values:
/// @arg  RCC_AHB3RSTR_FSMC
/// @param NewState: new state of the specified peripheral reset.
///   This parameter can be: ENABLE or DISABLE.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void RCC_AHB3PeriphResetCmd(u32 AHB3Periph, FunctionalState state)
{
        (state) ? (RCC->AHB3RSTR |= AHB3Periph) : (RCC->AHB3RSTR &= ~AHB3Periph);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases AHB2 peripheral reset.
/// @param RCC_AHB2Periph: specifies the AHB2 peripheral to reset.
///   This parameter can be any combination of the following values:
/// @arg RCC_AHB2RSTR_AES
/// @arg RCC_AHB2RSTR_RNG
/// @arg RCC_AHB2RSTR_USBFS
/// @param NewState: new state of the specified peripheral reset.
///   This parameter can be: ENABLE or DISABLE.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void RCC_AHB2PeriphResetCmd(u32 AHB2Periph, FunctionalState state)
{
    (state) ? (RCC->AHB2RSTR |= AHB2Periph) : (RCC->AHB2RSTR &= ~AHB2Periph);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases AHB1 peripheral reset.
/// @param RCC_AHB1Periph: specifies the AHB1 peripheral to reset.
///   This parameter can be any combination of the following values:
///  @arg RCC_AHB1RSTR_GPIOA
///  @arg RCC_AHB1RSTR_GPIOB
///  @arg RCC_AHB1RSTR_GPIOC
///  @arg RCC_AHB1RSTR_GPIOD
///  @arg RCC_AHB1RSTR_SDIO
///  @arg RCC_AHB1RSTR_CRC
///  @arg RCC_AHB1RSTR_FLASH
///  @arg RCC_AHB1RSTR_SRAM
///  @arg RCC_AHB1RSTR_DMA1
///  @arg RCC_AHB1RSTR_DMA2
///  @arg RCC_AHB1RSTR_ETHMAC
/// @param NewState: new state of the specified peripheral reset.
///   This parameter can be: ENABLE or DISABLE.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////

void RCC_AHB1PeriphResetCmd(u32 AHB1Periph, FunctionalState state)
{
    (state) ? (RCC->AHB1RSTR |= AHB1Periph) : (RCC->AHB1RSTR &= ~AHB1Periph);
}
#endif

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases High Speed APB (APB2) peripheral reset.
/// @param  APB2Periph: specifies the APB2 peripheral to reset.
///   This parameter can be any combination of the following values:
/// __MM3N1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_GPIOA, RCC_APB2RSTR_GPIOB,
/// 	 RCC_APB2RSTR_GPIOC, RCC_APB2RSTR_GPIOD, RCC_APB2RSTR_GPIOE,
/// 	 RCC_APB2RSTR_ADC1, RCC_APB2RSTR_ADC2, RCC_APB2RSTR_TIM1,
/// 	 RCC_APB2RSTR_SPI1, RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP
/// __MM0N1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_ADC1, RCC_APB2RSTR_TIM1,
/// 	 RCC_APB2RSTR_SPI1, RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP,
/// 	 RCC_APB2RSTR_TIM14, RCC_APB2RSTR_TIM16, RCC_APB2RSTR_TIM17,
/// 	 RCC_APB2RSTR_DBGMCU
/// __MM3O1
/// @arg RCC_APB2RSTR_TIM1, RCC_APB2RSTR_TIM8, RCC_APB2RSTR_UART1,
///      RCC_APB2RSTR_UART6,RCC_APB2RSTR_ADC1, RCC_APB2RSTR_SPI1,
///      RCC_APB2RSTR_EXTI, RCC_APB2RSTR_COMP
/// __MM0P1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_ADC1, RCC_APB2RSTR_ADC2,
///      RCC_APB2RSTR_TIM1, RCC_APB2RSTR_SPI1, RCC_APB2RSTR_TIM8,
///      RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP, RCC_APB2RSTR_TIM14,
///      RCC_APB2RSTR_TIM16, RCC_APB2RSTR_TIM17, RCC_APB2RSTR_DBGMCU,
///      RCC_APB2RSTR_PWM
/// __MM0Q1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_ADC1, RCC_APB2RSTR_TIM1,
/// 	 RCC_APB2RSTR_SPI1, RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP,
/// 	 RCC_APB2RSTR_TIM14, RCC_APB2RSTR_TIM16, RCC_APB2RSTR_TIM17,
/// 	 RCC_APB2RSTR_DBGMCU, RCC_APB2RSTR_PWM
/// __MM0S1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_ADC1, RCC_APB2RSTR_TIM1,
/// 	 RCC_APB2RSTR_SPI1, RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP,
/// 	 RCC_APB2RSTR_TIM14, RCC_APB2RSTR_TIM16, RCC_APB2RSTR_TIM17,
/// 	 RCC_APB2RSTR_DBGMCU
/// @param  state: new state of the specified peripheral reset.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_APB2PeriphResetCmd(u32 APB2Periph, FunctionalState state)
{
    (state) ? (RCC->APB2RSTR |= APB2Periph) : (RCC->APB2RSTR &= ~APB2Periph);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
/// @param  APB1Periph: specifies the APB1 peripheral to reset.
///   This parameter can be any combination of the following values:
/// __MM3N1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_TIM4,
///   	 RCC_APB1RSTR_WWDG, RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2,
///		 RCC_APB1RSTR_UART3, RCC_APB1RSTR_I2C1, RCC_APB1RSTR_I2C2,
///   	 RCC_APB1RSTR_USB, RCC_APB1RSTR_CAN, RCC_APB1RSTR_BKP,
///   	 RCC_APB1RSTR_PWR, RCC_APB1RSTR_DAC, RCC_APB1RSTR_ALL
/// __MM0N1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_WWDG,
///		 RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2, RCC_APB1RSTR_I2C1,
///   	 RCC_APB1RSTR_USB, RCC_APB1RSTR_CAN, RCC_APB1RSTR_CRS,
///   	 RCC_APB1RSTR_PWR
/// __MM3O1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_TIM4,
///   	 RCC_APB1RSTR_TIM5, RCC_APB1RSTR_TIM6, RCC_APB1RSTR_TIM7,
///		 RCC_APB1RSTR_WWDG, RCC_APB1RSTR_SPI2, RCC_APB1RSTR_SPI3,
///      RCC_APB1RSTR_UART2,RCC_APB1RSTR_UART3,RCC_APB1RSTR_UART4,
///      RCC_APB1RSTR_UART5,RCC_APB1RSTR_I2C1, RCC_APB1RSTR_I2C2,
///   	 RCC_APB1RSTR_CRS,  RCC_APB1RSTR_CAN,  RCC_APB1RSTR_BKP,
///   	 RCC_APB1RSTR_PWR,  RCC_APB1RSTR_UART7,RCC_APB1RSTR_UART8
/// __MM0P1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_WWDG,
///		 RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2, RCC_APB1RSTR_I2C1,
///   	 RCC_APB1RSTR_PWR
/// __MM0Q1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_WWDG,
///		 RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2, RCC_APB1RSTR_I2C1,
///   	 RCC_APB1RSTR_PWR
/// __MM0S1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_WWDG,
///		 RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2,RCC_APB1RSTR_I2C1,
///   	 RCC_APB1RSTR_USB,  RCC_APB1RSTR_CAN,  RCC_APB1RSTR_PWR
/// @param  state: new state of the specified peripheral clock.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_APB1PeriphResetCmd(u32 APB1Periph, FunctionalState state)
{
    (state) ? (RCC->APB1RSTR |= APB1Periph) : (RCC->APB1RSTR &= ~APB1Periph);
}

#if defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM0T1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases Low Speed AHB peripheral reset.
/// @param  AHBPeriph: specifies the AHB peripheral to reset.
///   This parameter can be any combination of the following values:
/// __MM0P1
/// @arg RCC_AHB1RSTR_DMA1, RCC_AHB1RSTR_SRAM, RCC_AHB1RSTR_FLITF,
///      RCC_AHB1RSTR_CRC, RCC_AHB1RSTR_GPIOA, RCC_AHB1RSTR_GPIOB,
///      RCC_AHB1RSTR_GPIOC, RCC_AHB1RSTR_GPIOD, RCC_AHB1RSTR_HWSQRT,
///      RCC_AHB1RSTR_HWDIV
/// __MM0Q1
/// @arg RCC_AHB1RSTR_DMA1, RCC_AHB1RSTR_SRAM, RCC_AHB1RSTR_FLITF,
///      RCC_AHB1RSTR_CRC, RCC_AHB1RSTR_GPIOA, RCC_AHB1RSTR_GPIOB,
///      RCC_AHB1RSTR_GPIOC, RCC_AHB1RSTR_GPIOD, RCC_AHB1RSTR_HWDIV
/// __MM0S1
///      RCC_AHB1RSTR_DMA1, RCC_AHB1RSTR_SRAM, RCC_AHB1RSTR_FLASH,
///      RCC_AHB1RSTR_CRC,RCC_AHB1RSTR_GPIOA, RCC_AHB1RSTR_GPIOB,
///      RCC_AHB1RSTR_GPIOC, RCC_AHB1RSTR_GPIOD
/// @param  state: new state of the specified peripheral clock.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_AHBPeriphResetCmd(u32 AHBPeriph, FunctionalState state)
{
    (state) ? (RCC->AHB1RSTR |= AHBPeriph) : (RCC->AHB1RSTR &= ~AHBPeriph);
}
#endif

#if defined(__MM3N1) || defined(__MM3O1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases the Backup domain reset.
/// @param  state: new state of the Backup domain reset.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_BackupResetCmd(FunctionalState state)
{
    MODIFY_REG(RCC->BDCR, RCC_BDCR_BDRST, (state << RCC_BDCR_BDRST_Pos));
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the Clock Security System.
/// @param  state: new state of the Clock Security System..
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_ClockSecuritySystemCmd(FunctionalState state)
{
    MODIFY_REG(RCC->CR, RCC_CR_CSSON, (state << RCC_CR_CSSON_Pos));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Selects the clock source to output on MCO pin.
/// @param  mcoSrc: specifies the clock source to output.
///   This parameter can be one of the following values:
/// @arg RCC_MCO_NoClock: No clock selected
/// @arg RCC_MCO_LSI: LSI oscillator clock selected
/// @arg RCC_MCO_LSE: LSE oscillator clock selected
/// @arg RCC_MCO_SYSCLK: System clock selected
/// @arg RCC_MCO_HSI: HSI oscillator clock selected
/// @arg RCC_MCO_HSE: HSE oscillator clock selected
/// @arg RCC_MCO_PLLCLK_Div2: PLL clock divided by 2 selected
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_MCOConfig(RCC_MCO_TypeDef mcoSrc)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO, mcoSrc);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clears the RCC reset flags.
///   The reset flags are: RCC_FLAG_PINRST, RCC_FLAG_PORRST,
///   RCC_FLAG_SFTRST, RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST,
///   RCC_FLAG_LPWRRST
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_ClearFlag(void)
{
    SET_BIT(RCC->CSR, RCC_CSR_RMVF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified RCC interrupts.
/// @param  it: specifies the RCC interrupt sources to be enabled or
/// disabled.
///   This parameter can be any combination of the following values:
/// @arg RCC_IT_LSIRDY: LSI ready interrupt
/// @arg RCC_IT_LSERDY: LSE ready interrupt
/// @arg RCC_IT_HSIRDY: HSI ready interrupt
/// @arg RCC_IT_HSERDY: HSE ready interrupt
/// @arg RCC_IT_PLLRDY: PLL ready interrupt
/// @param  state: new state of the specified RCC interrupts.
///   This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_ITConfig(RCC_IT_TypeDef it, FunctionalState state)
{
    (state) ? SET_BIT(RCC->CIR, it << RCC_CIR_LSIRDYIE_Pos) : CLEAR_BIT(RCC->CIR, it << RCC_CIR_LSIRDYIE_Pos);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified RCC interrupt has occurred or not.
/// @param  it: specifies the RCC interrupt source to check.
///   This parameter can be one of the following values:
/// @arg RCC_IT_LSIRDY: LSI ready interrupt
/// @arg RCC_IT_LSERDY: LSE ready interrupt
/// @arg RCC_IT_HSIRDY: HSI ready interrupt
/// @arg RCC_IT_HSERDY: HSE ready interrupt
/// @arg RCC_IT_PLLRDY: PLL ready interrupt
/// @arg RCC_IT_CSS: Clock Security System interrupt
/// @retval The new state of it (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
ITStatus RCC_GetITStatus(RCC_IT_TypeDef it)
{
    return (ITStatus)READ_BIT(RCC->CIR, (it << RCC_CIR_LSIRDYF_Pos));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clears the RCC�?interrupt pending bits.
/// @param  it: specifies the interrupt pending bit to clear.
///   This parameter can be any combination of the following values:
/// @arg RCC_IT_LSIRDY: LSI ready interrupt
/// @arg RCC_IT_LSERDY: LSE ready interrupt
/// @arg RCC_IT_HSIRDY: HSI ready interrupt
/// @arg RCC_IT_HSERDY: HSE ready interrupt
/// @arg RCC_IT_PLLRDY: PLL ready interrupt
/// @arg RCC_IT_CSS: Clock Security System interrupt
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_ClearITPendingBit(u8 it)
{
    SET_BIT(RCC->CIR, (it << RCC_CIR_LSIRDYC_Pos));
}

////////////////////////////////////////////////////////////////////////////////
//
//	New Function Interface
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
/// @param  RCC_APB1Periph: specifies the APB1 peripheral to reset.
///   This parameter can be any combination of the following values:
/// __MM3N1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_TIM4,
///   	 RCC_APB1RSTR_WWDG, RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2,
///		 RCC_APB1RSTR_UART3, RCC_APB1RSTR_I2C1, RCC_APB1RSTR_I2C2,
///   	 RCC_APB1RSTR_USB, RCC_APB1RSTR_CAN, RCC_APB1RSTR_BKP,
///   	 RCC_APB1RSTR_PWR, RCC_APB1RSTR_DAC
/// __MM0N1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_WWDG,
///		 RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2, RCC_APB1RSTR_I2C1,
///   	 RCC_APB1RSTR_USB, RCC_APB1RSTR_CAN, RCC_APB1RSTR_CRS,
///   	 RCC_APB1RSTR_PWR
/// __MM0P1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_WWDG,
///		 RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2, RCC_APB1RSTR_I2C1,
///   	 RCC_APB1RSTR_PWR
/// __MM0Q1
/// @arg RCC_APB1RSTR_TIM2, RCC_APB1RSTR_TIM3, RCC_APB1RSTR_WWDG,
///		 RCC_APB1RSTR_SPI2, RCC_APB1RSTR_UART2, RCC_APB1RSTR_I2C1,
///   	 RCC_APB1RSTR_PWR
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exRCC_APB1PeriphReset(u32 RCC_APB1Periph)
{
    RCC->APB1RSTR |= RCC_APB1Periph;
    RCC->APB1RSTR &= ~RCC_APB1Periph;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exRCC_BackupReset()
{
#if defined(__MM3N1) || defined(__MM3O1) || defined(__MM0S1)
    RCC->BDCR |=   RCC_BDCR_BDRST;
    RCC->BDCR &=  ~RCC_BDCR_BDRST;
#endif
}

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Forces or releases High Speed APB (APB2) peripheral reset.
/// @param  RCC_APB2Periph: specifies the APB2 peripheral to reset.
///   This parameter can be any combination of the following values:
/// __MM3N1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_GPIOA, RCC_APB2RSTR_GPIOB,
/// 	 RCC_APB2RSTR_GPIOC, RCC_APB2RSTR_GPIOD, RCC_APB2RSTR_GPIOE,
/// 	 RCC_APB2RSTR_ADC1, RCC_APB2RSTR_ADC2, RCC_APB2RSTR_TIM1,
/// 	 RCC_APB2RSTR_SPI1, RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP
/// __MM0N1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_ADC1, RCC_APB2RSTR_TIM1,
/// 	 RCC_APB2RSTR_SPI1, RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP,
/// 	 RCC_APB2RSTR_TIM14, RCC_APB2RSTR_TIM16, RCC_APB2RSTR_TIM17,
/// 	 RCC_APB2RSTR_DBGMCU
/// __MM0P1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_ADC1, RCC_APB2RSTR_ADC2,
///      RCC_APB2RSTR_TIM1, RCC_APB2RSTR_SPI1, RCC_APB2RSTR_TIM8,
///      RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP, RCC_APB2RSTR_TIM14,
///      RCC_APB2RSTR_TIM16, RCC_APB2RSTR_TIM17, RCC_APB2RSTR_DBGMCU,
///      RCC_APB2RSTR_PWM
/// __MM0Q1
/// @arg RCC_APB2RSTR_EXTI, RCC_APB2RSTR_ADC1, RCC_APB2RSTR_TIM1,
/// 	 RCC_APB2RSTR_SPI1, RCC_APB2RSTR_UART1, RCC_APB2RSTR_COMP,
/// 	 RCC_APB2RSTR_TIM14, RCC_APB2RSTR_TIM16, RCC_APB2RSTR_TIM17,
/// 	 RCC_APB2RSTR_DBGMCU, RCC_APB2RSTR_PWM
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exRCC_APB2PeriphReset(u32 RCC_APB2Periph)
{
    RCC->APB2RSTR |= RCC_APB2Periph;
    RCC->APB2RSTR &= ~RCC_APB2Periph;
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Disable systick
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exRCC_SystickDisable()
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable systick
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exRCC_SystickEnable(u32 sysTickPeriod)
{
	SysTick_Config(RCC_GetHCLKFreq() / 1000000 * sysTickPeriod);
}

#define RCC_LATENCY_TB		16
#define RCC_MUL_TB			12
#define RCC_DIV_TB			8
#define RCC_SW_TB			4
#define RCC_SRC_TB			0

void RCC_ADC_ClockCmd(ADC_TypeDef* peripheral, FunctionalState state)
{
    switch (*(u32*)&peripheral) {

        case ADC1_BASE:
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM3U1)
            (state) ? (RCC->APB2ENR |= RCC_APB2ENR_ADC1) : (RCC->APB2ENR &= ~RCC_APB2ENR_ADC1);
#endif
            break;
#if defined(__MM3N1) || defined(__MMT3270)
        case ADC2_BASE:
            (state) ? (RCC->APB2ENR |= RCC_APB2ENR_ADC2) : (RCC->APB2ENR &= ~RCC_APB2ENR_ADC2);
            break;
#endif
#if defined(__MM3U1)
        case ADC3_BASE:
            (state) ? (RCC->APB2ENR |= RCC_APB2ENR_ADC3) : (RCC->APB2ENR &= ~RCC_APB2ENR_ADC3);
            break;
#endif
        default:
            break;
    }

}
#if defined(__MM3U1)

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified BKP peripheral Clock.
/// @param  peripheral:select the BKP peripheral.
/// @param  state: new state of the BKP peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_BKP_ClockCmd(BKP_TypeDef* peripheral, FunctionalState state)
{
    if(BKP == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_BKP) : (RCC->APB1ENR &= ~RCC_APB1ENR_BKP);
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_PWR) : (RCC->APB1ENR &= ~RCC_APB1ENR_PWR);
    }
}
#endif
#if defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified CAN peripheral Clock.
/// @param  peripheral:select the CAN peripheral.
/// @param  state: new state of the CAN peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_CAN_ClockCmd(CAN_TypeDef* peripheral, FunctionalState state)
{
    if(CAN1 == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_CAN) : (RCC->APB1ENR &= ~RCC_APB1ENR_CAN);
    }
}
#endif
#if defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified COMP peripheral Clock.
/// @param  peripheral:select the COMP peripheral.
/// @param  state: new state of the COMP peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_COMP_ClockCmd(COMP_TypeDef* peripheral, FunctionalState state)
{
    if(COMP == peripheral) {
        (state) ? (RCC->APB2ENR |= RCC_APB2ENR_COMP) : (RCC->APB2ENR &= ~RCC_APB2ENR_COMP);
    }
}
#endif
#if defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified CRC peripheral Clock.
/// @param  peripheral:select the CRC peripheral.
/// @param  state: new state of the CRC peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_CRC_ClockCmd(CRC_TypeDef* peripheral, FunctionalState state)
{
    if(CRC == peripheral) {
        (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_CRC) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_CRC);
    }
}
#endif
#if defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified DAC peripheral Clock.
/// @param  peripheral:select the DAC peripheral.
/// @param  state: new state of the DAC peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_DAC_ClockCmd(DAC_TypeDef* peripheral, FunctionalState state)
{
    if(DAC == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_DAC) : (RCC->APB1ENR &= ~RCC_APB1ENR_DAC);
    }
}
#endif
#if defined(__MM3U1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified DMA peripheral Clock.
/// @param  peripheral:select the DMA peripheral.
/// @param  state: new state of the DMA peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_DMA_ClockCmd(DMA_TypeDef* peripheral, FunctionalState state)
{
    if(DMA1 == peripheral) {
        (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_DMA1) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA1);
    }
#if defined(DMA2)
    if(DMA2 == peripheral) {
        (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_DMA2) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA2);
    }
#endif
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enables or disables the specified GPIO peripheral Clock.
/// @param  peripheral:select the GPIO peripheral.
/// @param  state: new state of the GPIO peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_GPIO_ClockCmd(GPIO_TypeDef* peripheral, FunctionalState state)
{
    switch (*(u32*)&peripheral) {
#if defined(__MM3N1)
        case (u32)GPIOA:
            (state) ? (RCC->APB2ENR |= RCC_APB2ENR_GPIOA) : (RCC->APB2ENR &= ~RCC_APB2ENR_GPIOA);
            break;
        case (u32)GPIOB:
            (state) ? (RCC->APB2ENR |= RCC_APB2ENR_GPIOB) : (RCC->APB2ENR &= ~RCC_APB2ENR_GPIOB);
            break;
        case (u32)GPIOC:
            (state) ? (RCC->APB2ENR |= RCC_APB2ENR_GPIOC) : (RCC->APB2ENR &= ~RCC_APB2ENR_GPIOC);
            break;
        case (u32)GPIOD:
            (state) ? (RCC->APB2ENR |= RCC_APB2ENR_GPIOD) : (RCC->APB2ENR &= ~RCC_APB2ENR_GPIOD);
            break;
#endif
#if defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM0T1) || defined(__MM3U1)
        case (u32)GPIOA:
            (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOA) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOA);
            break;
        case (u32)GPIOB:
            (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOB) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOB);
            break;
#endif
#if defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
        case (u32)GPIOC:
            (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOC) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOC);
            break;
        case (u32)GPIOD:
            (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOD) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOD);
            break;
#endif
#if defined(__MM3O1) || defined(__MM3U1)
        case (u32)GPIOE:
            (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOE) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOE);
            break;
#endif
#if defined(__MM3U1)
        case (u32)GPIOF:
            (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOF) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOF);
            break;
        case (u32)GPIOG:
            (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOG) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOG);
            break;
        case (u32)GPIOH:
            (state) ? (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOH) : (RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOH);
            break;
#endif
        default:
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Deinitializes the uart peripheral registers to their
///         default reset values.
/// @param  peripheral: Select the UART or the UART peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_UART_ClockCmd(UART_TypeDef* peripheral, FunctionalState state)
{
    if(UART2 == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_UART2) : (RCC->APB1ENR &= ~RCC_APB1ENR_UART2);//exRCC_APB1PeriphReset(RCC_APB1ENR_UART2);
    }
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
    if(UART1 == peripheral) {
        (state) ? (RCC->APB2ENR |= RCC_APB2ENR_UART1) : (RCC->APB2ENR &= ~RCC_APB2ENR_UART1);//exRCC_APB2PeriphReset(RCC_APB2ENR_UART1);
    }
#endif
#if defined(__MM0T1)
    if(UART1 == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_UART1) : (RCC->APB1ENR &= ~RCC_APB1ENR_UART1);//exRCC_APB1PeriphReset(RCC_APB1ENR_UART1);
    }
#endif
#if defined(__MM3N1) || defined(__MM3O1)||defined(__MM3U1)
    if(UART3 == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_UART3) : (RCC->APB1ENR &= ~RCC_APB1ENR_UART3);//exRCC_APB1PeriphReset(RCC_APB1ENR_UART3);
    }
#endif
#if defined(__MM3O1)||defined(__MM3U1)
    if(UART4 == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_UART4) : (RCC->APB1ENR &= ~RCC_APB1ENR_UART4);//exRCC_APB1PeriphReset(RCC_APB1ENR_UART4);
    }
    if(UART5 == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_UART5) : (RCC->APB1ENR &= ~RCC_APB1ENR_UART5);//exRCC_APB1PeriphReset(RCC_APB1ENR_UART5);
    }
    if(UART6 == peripheral) {
        (state) ? (RCC->APB2ENR |= RCC_APB2ENR_UART6) : (RCC->APB2ENR &= ~RCC_APB2ENR_UART6);//exRCC_APB2PeriphReset(RCC_APB2ENR_UART6);
    }
    if(UART7 == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_UART7) : (RCC->APB1ENR &= ~RCC_APB1ENR_UART7);//exRCC_APB1PeriphReset(RCC_APB1ENR_UART7);
    }
    if(UART8 == peripheral) {
        (state) ? (RCC->APB1ENR |= RCC_APB1ENR_UART8) : (RCC->APB1ENR &= ~RCC_APB1ENR_UART8);//exRCC_APB1PeriphReset(RCC_APB1ENR_UART8);
    }
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the External High Speed oscillator (HSE).
/// @param  para
/// @retval An ErrorStatus enumuration value:
/// - SUCCESS: HSE oscillator is stable and ready to use
/// - ERROR: HSE oscillator not yet ready
////////////////////////////////////////////////////////////////////////////////
ErrorStatus exRCC_Init(RCCInitStruct_TypeDef* para)
{
    RCC_DeInit();

    // Protection
    if ((((int)((para->RCC_SystemClock & 0xF0000) >> RCC_LATENCY_TB) < 0x0) || \
			((int)((para->RCC_SystemClock & 0xF0000) >> RCC_LATENCY_TB) > 3)) != 0) {
        para->RCC_SystemClock = SYSCLK_HSI_6d;									// default clock
		RCC->CR |= RCC_CR_HSION;
		if (!RCC_WaitForFlagStartUp(RCC_FLAG_HSIRDY))  return ERROR;
	}
	else {
		// Flash
		FLASH->ACR |= FLASH_ACR_PRFTBE;
		FLASH->ACR &= ~FLASH_ACR_LATENCY;
        FLASH->ACR |= (para->RCC_SystemClock & 0xF0000) >> RCC_LATENCY_TB;

//------------------------------------------------------------------------------
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1)
		// Set PLL
        if (((para->RCC_SystemClock & 0x000F0) >> RCC_SW_TB) == 2) {
			RCC->CR |= RCC_CR_PLLON;
			if (!RCC_WaitForFlagStartUp(RCC_FLAG_PLLRDY)) return ERROR;
		}
        RCC->CR |= (((para->RCC_SystemClock & 0x0F000) >> RCC_MUL_TB) << RCC_CR_PLLMUL_Pos) & RCC_CR_PLLMUL;  	// RCC_CR_PLLMUL
		RCC->CR |= (((para->RCC_SystemClock & 0x00F00) >> RCC_DIV_TB) << RCC_CR_PLLDIV_Pos) & RCC_CR_PLLDIV;  	// RCC_CR_PLLDIV
#endif

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0S1)
		RCC->CFGR |= (para->RCC_PrescaleUSB << RCC_CFGR_USBPRE_Pos) & RCC_CFGR_USBPRE;                               					// RCC_CR_USB
#endif

#if defined(__MM0Q1)
    // Null
#endif

//------------------------------------------------------------------------------
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0S1)
		// Set Oscillator
        if (((para->RCC_SystemClock & 0x0000F) >> RCC_SRC_TB) == 0) {
			RCC->CR |= RCC_CR_HSION;
			if (!RCC_WaitForFlagStartUp(RCC_FLAG_HSIRDY)) return ERROR;
			RCC->CFGR &= ~RCC_CFGR_PLLSRC;
		}
		else {
			RCC->CR |= RCC_CR_HSEON;
			if (!RCC_WaitForFlagStartUp(RCC_FLAG_HSERDY)) return ERROR;
			RCC->CFGR |= RCC_CFGR_PLLSRC;
		}
#endif

#if defined(__MM0Q1) || defined(__MM0T1)
 		if (((para->RCC_SystemClock & 0x0000F) >> RCC_SRC_TB) == 1) {
			RCC->CR |= RCC_CR_HSEON;
			if (!RCC_WaitForFlagStartUp(RCC_FLAG_HSERDY)) return ERROR;
		}
        else if (((para->RCC_SystemClock & 0x0000F) >> RCC_SRC_TB) == 2) {
			RCC->CSR |= RCC_CSR_LSION;
			if (!RCC_WaitForFlagStartUp(RCC_FLAG_LSIRDY)) return ERROR;
		}
		else {
#if defined(__MM0Q1)
            ((para->RCC_SystemClock & 0xF0000) >> RCC_LATENCY_TB == 1) ?
				(RCC->CR &= ~RCC_CR_HSI_72M) : (RCC->CR |=  RCC_CR_HSI_72M);
#endif
			RCC->CR |= RCC_CR_HSION;
			if (!RCC_WaitForFlagStartUp(RCC_FLAG_HSIRDY)) return ERROR;
		}
#endif
	}
    //------------------------------------------------------------------------------
    // AHB, APB1, APB2
    RCC->CFGR |= (para->RCC_PrescaleAHB  << RCC_CFGR_HPRE_Pos)  & RCC_CFGR_HPRE;
    RCC->CFGR |= (para->RCC_PrescaleAPB1 << RCC_CFGR_PPRE1_Pos) & RCC_CFGR_PPRE1;
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1)
    RCC->CFGR |= (para->RCC_PrescaleAPB2 << RCC_CFGR_PPRE2_Pos) & RCC_CFGR_PPRE2;
#endif

    // Clock Switch to
      RCC->CFGR |= (((para->RCC_SystemClock & 0x000F0) >> RCC_SW_TB) << RCC_CFGR_SW_Pos) & RCC_CFGR_SW;

    while (((RCC->CFGR & RCC_CFGR_SWS) >> 2) != ((para->RCC_SystemClock & 0x000F0) >> RCC_SW_TB));

    if (para->SysTickEN == ENABLE)
        SysTick_Config(RCC_GetHCLKFreq() / 1000000 * para->SysTickPeriod);
    else
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    return SUCCESS;
}

#if defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  External crystal drive current calibration value.If the crystal oscillator is abnormal,
///   the drive current can be adjusted to match the crystal oscillator.
/// @param  RCC_Config: external crystal drive current calibration value.
///   This parameter can be any combination of the following values:
/// @arg RCC_CONFIG_OSC_ITRIM_2mA, RCC_CONFIG_OSC_ITRIM_4mA,
/// 	 RCC_CONFIG_OSC_ITRIM_6mA, RCC_CONFIG_OSC_ITRIM_8mA.
////////////////////////////////////////////////////////////////////////////////
void exRCC_Set_OSC_ITRIM_Config(u32 val)
{

  RCC->CONFIG &= ~RCC_CONFIG_ITRIM;
  RCC->CONFIG |= val;
}
#endif

/// @}

/// @}

/// @}
