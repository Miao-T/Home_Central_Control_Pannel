////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_BEEP.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE LED BSP LAYER FUNCTIONS.
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
#define _BSP_BEEP_C_

// Files includes  -------------------------------------------------------------
#include "hal_gpio.h"
#include "hal_nvic.h"
#include "hal_rcc.h"
#include "hal_tim.h"

#include "bsp.h"
#include "bsp_beep.h"
#include "bsp_tim.h"
#include "common.h"

/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BEEP_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BEEP_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize BEEP.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_BEEP_Configure(u32 freq)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;
    u16 arr = 2000000 / freq - 1;
    
    (TIM1 == BEEP_TIMER) ? COMMON_EnableIpClock(emCLOCK_TIM1) : 
        COMMON_EnableIpClock(emCLOCK_TIM3);
    
    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = 59;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(BEEP_TIMER, &TIM_TimeBaseStructure);
    
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode          = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState     = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse           = arr >> 1;
    TIM_OCInitStructure.TIM_OCPolarity      = TIM_OCPolarity_High;
    
    if (2 == BEEP_PWMCH) {
        TIM_OC2Init(BEEP_TIMER, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(BEEP_TIMER, TIM_OCPreload_Enable);
        BSP_TIM_CH2_GPIO_Configure(BEEP_TIMER, CH_REMAPEN, CH_REMAPID, 1);
    } else {
        TIM_OC1Init(BEEP_TIMER, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(BEEP_TIMER, TIM_OCPreload_Enable);
        BSP_TIM_CH1_GPIO_Configure(BEEP_TIMER, CH_REMAPEN, CH_REMAPID, 1);
    };
    
    TIM_CtrlPWMOutputs(BEEP_TIMER, ENABLE);
    TIM_ARRPreloadConfig(BEEP_TIMER, ENABLE);
    TIM_Cmd(BEEP_TIMER, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
void BEEP_on(u32 val)
{
    TIM_SetAutoreload(BEEP_TIMER, val << 1);
        
    if (2 == BEEP_PWMCH) {
        TIM_SetCompare2(BEEP_TIMER, val);
    }
    else
        TIM_SetCompare1(BEEP_TIMER, val);
}

////////////////////////////////////////////////////////////////////////////////
void BEEP_off(void)
{
    if (2 == BEEP_PWMCH) {
        TIM_SetCompare2(BEEP_TIMER, 0);
    }
    else
        TIM_SetCompare1(BEEP_TIMER, 0);
}

////////////////////////////////////////////////////////////////////////////////
void BEEP_freq(u32 *val)
{
    if (*val < 200) {
        *val = 200;
    }
    
    TIM_SetAutoreload(BEEP_TIMER, *val);
}

/// @}

/// @}

/// @}

