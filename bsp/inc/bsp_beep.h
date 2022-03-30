////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_BEEP.H
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE LED
///           BSP LAYER.
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
#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_BSP
/// @brief LED BSP modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Types
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Constants
/// @{

#if defined(__MM32_MB020)
    #define BEEP_TIMER TIM3
    #define BEEP_PWMCH 2
    #define CH_REMAPEN 1
    #define CH_REMAPID 0
#elif defined(__MM32_MB032) || defined(__MM32_MB033)
    #define BEEP_TIMER TIM1
    #define BEEP_PWMCH 2
    #define CH_REMAPEN 1
    #define CH_REMAPID 1
#elif defined(__MM32_MB036) || defined(__MM32_MB039)
    #define BEEP_TIMER TIM1
    #define BEEP_PWMCH 1
    #define CH_REMAPEN 1
    #define CH_REMAPID 0
#else
    #define BEEP_TIMER TIM1
    #define BEEP_PWMCH 1
    #define CH_REMAPEN 0
    #define CH_REMAPID 0
#endif


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Variables
/// @{
#ifdef _BSP_BEEP_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif


#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Functions
/// @{



void BEEP_on(u32 val);
void BEEP_off(void);

void BEEP_once(void);
void BEEP_lang(void);

void BEEP_freq(u32 *val);

void BSP_BEEP_Configure(u32 freq);

/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__BSP_BEEP_H */
////////////////////////////////////////////////////////////////////////////////
