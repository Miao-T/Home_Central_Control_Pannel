////////////////////////////////////////////////////////////////////////////////
/// @file     HAL_COMP.H
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE COMP
///           FIRMWARE LIBRARY.
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
#ifndef __HAL_COMP_H
#define __HAL_COMP_H

// Files includes  -------------------------------------------------------------
#include "mm32_types.h"
#include "MM32.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_HAL
/// @brief COMP HAL modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief COMP_InvertingInput
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
typedef enum {
#if defined(__MM0N1)
    emCOMP_InvertingInput_1_4VREFINT  	= COMP_CSR_INM_VREFINT_1_4, 	///< Vrefint 1/4 as COMP inverting input
    emCOMP_InvertingInput_1_2VREFINT  	= COMP_CSR_INM_VREFINT_1_2, 	///< Vrefint 1/2 as COMP inverting input
    emCOMP_InvertingInput_3_4VREFINT  	= COMP_CSR_INM_VREFINT_3_4, 	///< Vrefint 3/4 as COMP inverting input
    emCOMP_InvertingInput_VREFINT 		= COMP_CSR_INM_VREFINT,  		///< Vrefint 1 as COMP inverting input
    emCOMP_InvertingInput_DAC1 			= COMP_CSR_INM_INM4,  			///< INM4 as COMP inverting input
    emCOMP_InvertingInput_DAC2 			= COMP_CSR_INM_INM5,  			///< INM5 as COMP inverting input
    emCOMP_InvertingInput_IO1 			= COMP_CSR_INM_INM6,  			///< INM6 as COMP inverting input
    emCOMP_InvertingInput_IO2 			= COMP_CSR_INM_INM7  			///< INM7 as COMP inverting input
#endif
#if defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
    emCOMP_InvertingInput_IO0 			= COMP_CSR_INM_INM0,  				///< INM0 as COMP inverting input
    emCOMP_InvertingInput_IO1 			= COMP_CSR_INM_INM1,  				///< INM1 as COMP inverting input
    emCOMP_InvertingInput_IO2 			= COMP_CSR_INM_INM2,  				///< INM2 as COMP inverting input
    emCOMP_InvertingInput_CRV 			= COMP_CSR_INM_INM3,  				///< INM3 as COMP inverting input
#endif
} EM_COMP_InvertingInput;
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief COMP_NonInvertingInput
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
typedef enum {
    emCOMP_NonInvertingInput_IO1 		= COMP_CSR_INP_INP0,  			///< INP0 as COMP non-inverting input
    emCOMP_NonInvertingInput_IO2 		= COMP_CSR_INP_INP1,  			///< INP1 as COMP non-inverting input
    emCOMP_NonInvertingInput_IO3 		= COMP_CSR_INP_INP2,  			///< INP2 as COMP non-inverting input
    emCOMP_NonInvertingInput_IO4 		= COMP_CSR_INP_INP3,  			///< INP3 as COMP non-inverting input
#if defined(__MM0N1)
    emCOMP_NonInvertingInput_IO5 		= COMP_CSR_INP_INP4,  			///< INP4 as COMP non-inverting input
    emCOMP_NonInvertingInput_IO6 		= COMP_CSR_INP_INP5,  			///< INP5 as COMP non-inverting input
    emCOMP_NonInvertingInput_IO7 		= COMP_CSR_INP_INP6,  			///< INP6 as COMP non-inverting input
    emCOMP_NonInvertingInput_IO8 		= COMP_CSR_INP_INP7  			///< INP7 as COMP non-inverting input
#endif
} EM_COMP_NonInvertingInput;
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief COMP_Output
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
typedef enum {
	emCOMP_Output_None = 0x00000000, ///< No output
	emCOMP_Output_TIM1BKIN 		= COMP_CSR_OUTSEL_TIM1_BRAKE, ///< Timer1 brake input
#if defined(__MM0P1)
    emCOMP_Output_TIM8BKIN 		= COMP_CSR_OUTSEL_TIM8_BRAKE, ///< Timer8 brake input
#endif
	emCOMP_Output_TIM1OCREFCLR 	= COMP_CSR_OUTSEL_TIM1_OCREFCLR, ///< Timer1 ocrefclear input
	emCOMP_Output_TIM1IC1 		= COMP_CSR_OUTSEL_TIM1_CAPTURE1, ///< Timer1 input capture 1
	emCOMP_Output_TIM2IC4 		= COMP_CSR_OUTSEL_TIM2_CAPTURE4, ///< Timer2 input capture 4
	emCOMP_Output_TIM2OCREFCLR 	= COMP_CSR_OUTSEL_TIM2_OCREFCLR, ///< Timer2 ocrefclear input
	emCOMP_Output_TIM3IC1 		= COMP_CSR_OUTSEL_TIM3_CAPTURE1, ///< Timer3 input capture 1
	emCOMP_Output_TIM3OCREFCLR 	= COMP_CSR_OUTSEL_TIM3_OCREFCLR, ///< Timer3 ocrefclear input
#if defined(__MM0P1)
    emCOMP_Output_TIM8OCREFCLR 	= COMP_CSR_OUTSEL_TIM8_OCREFCLR  ///< Timer8 ocrefclear input
#endif
} EM_COMP_Output;
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief COMP_OutputPoloarity
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    emCOMP_NonInverted                = 0x00000000,  											///< COMP non-inverting output
    COMP_OutputPol_NonInverted        = 0x00000000,
    emCOMP_Inverted                   = 0x00008000,   											///< COMP inverting output
    COMP_OutputPol_Inverted           = 0x00008000
} EM_COMP_OutputPol;

////////////////////////////////////////////////////////////////////////////////
/// @brief COMP_Hysteresis
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
typedef enum {
    emCOMP_Hysteresis_No = COMP_CSR_HYST_NONE,  										///< Hysteresis Voltage: 0mV
    emCOMP_Hysteresis_Low    = COMP_CSR_HYST_LOW,  								///< Hysteresis Voltage: 15mV
    emCOMP_Hysteresis_Medium = COMP_CSR_HYST_MID,  								///< Hysteresis Voltage: 30mV
    emCOMP_Hysteresis_High   = COMP_CSR_HYST_HIGH   								///< Hysteresis Voltage: 90mV
} EM_COMP_Hysteresis;
#endif
#if defined(__MM3U1)
typedef enum {
    COMP_Filter_0_Period                = COMP_CSR_OFLT_0,                      ///< filter is ((u32)0x00000000)
    COMP_Filter_2_Period                = COMP_CSR_OFLT_1,                      ///< filter is ((u32)0x00040000)
    COMP_Filter_4_Period                = COMP_CSR_OFLT_2,                      ///< filter is ((u32)0x00080000)
    COMP_Filter_8_Period                = COMP_CSR_OFLT_3,                      ///< filter is ((u32)0x000C0000)
    COMP_Filter_16_Period               = COMP_CSR_OFLT_4,                      ///< filter is ((u32)0x00100000)
    COMP_Filter_32_Period               = COMP_CSR_OFLT_5,                      ///< filter is ((u32)0x00140000)
    COMP_Filter_64_Period               = COMP_CSR_OFLT_6,                      ///< filter is ((u32)0x00180000)
    COMP_Filter_128_Period              = COMP_CSR_OFLT_7,                      ///< filter is ((u32)0x001C0000)
} EM_COMP_FILT;
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief COMP_Mode
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
typedef enum {
    emCOMP_Mode_HighSpeed = COMP_CSR_MODE_HIGHRATE,  								///< Comparator high rate mode
    emCOMP_Mode_MediumSpeed = COMP_CSR_MODE_MEDIUMRATE,  							///< Comparator medium rate mode
    emCOMP_Mode_LowPower = COMP_CSR_MODE_LOWPOWER,  								///< Comparator low power mode
    emCOMP_Mode_UltraLowPower = COMP_CSR_MODE_LOWESTPOWER 						///< Comparator lowest power mode
} EM_COMP_Mode;
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief COMP_OutputLevel
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
typedef enum {
    emCOMP_OutputLevel_High = COMP_CSR_OUTSEL,  ///<  High output
    emCOMP_OutputLevel_Low  = 0x00000000        ///<  Low output
} EM_COMP_OutputLevel;
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  COMP Init structure definition
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
typedef struct {
    u32 Invert;                                                             ///< Selects the inverting input of the comparator.
    u32 NonInvert;                                                          ///< Selects the non inverting input of the comparator.
    u32 Output;                                                             ///< Selects the output redirection of the comparator.
    u32 BlankingSrce;                                                       ///< Selects the output blanking source of the comparator.
    u32 OutputPol;                                                          ///< Selects the output polarity of the comparator.
    u32 Hysteresis;                                                         ///< Selects the hysteresis voltage of the comparator.
    u32 Mode;                                                               ///< Selects the operating mode of the comparator and allows
#if defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
    u32 OFLT;                                                               ///< to adjust the speed/consumption.
#endif
} COMP_InitTypeDef;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
typedef struct {

    FunctionalState COMP_Poll_En;                                                ///< Selects the inverting input of the comparator.

    u32 COMP_Poll_Ch;                                                            ///< Selects the non inverting input of the comparator.
    u32 COMP_Poll_Fixn;                                                          ///< Selects the output redirection of the comparator.
    u32 COMP_Poll_Period;                                                        ///< Selects the output polarity of the comparator.
    u32 COMP_Poll_Pout;                                                          ///< Selects the hysteresis voltage of the comparator.

} COMP_POLL_InitTypeDef;
#endif
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_Exported_Constants
/// @{
//
//#if defined(__MM0N1) || defined(__MM0S1)
//    #define COMP1 				((u32)0x00000000)  								///< Select comparator 1
//    #define COMP2 				((u32)0x00000004)  								///< Select comparator 2
//#endif
//#if defined(__MM0P1)
////    #define COMP1 				((u32)0x00000000)  								///< Select comparator 4
////    #define COMP2 				((u32)0x00000004)  								///< Select comparator 5
////    #define COMP3 				((u32)0x00000008)  								///< Select comparator 1
////    #define COMP4 				((u32)0x0000000C)  								///< Select comparator 2
////    #define COMP5 				((u32)0x00000014)  								///< Select comparator 3
//
//    #define COMP1 				((u32)0x00000008)  								///< Select comparator 4
//    #define COMP2 				((u32)0x0000000c)  								///< Select comparator 5
//    #define COMP3 				((u32)0x00000014)  								///< Select comparator 1
//    #define COMP4 				((u32)0x00000000)  								///< Select comparator 2
//    #define COMP5 				((u32)0x00000004)  								///< Select comparator 3
//#endif
//#if defined(__MM0Q1)
//    #define COMP1 				((u32)0x00000000)  								///< Select comparator 1
//#endif
//
//#if defined(__MM3U1)
//    #define COMP1 				((u32)0x0000000c)  								///< Select comparator 1
//    #define COMP2 				((u32)0x00000010)  								///< Select comparator 2
//#endif

#define COMP_BlankingSrce_None 	((u32)0x00000000)
#define COMP_CSR_CLEAR_MASK 	((u32)0x00000003)

#define COMP_CSR_COMPSW1 		((u32)0x00000002)

/// @}

////////////////////////////////////////////////////////////////////////////////
///@defgroup COMP_Exported_Variables
/// @{
#ifdef _HAL_COMP_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif

#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_Exported_Functions
/// @{

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1) || defined(__MM3U1)
void COMP_DeInit(u32 selection);
void COMP_Init(u32 selection, COMP_InitTypeDef *COMP_InitStruct);
void COMP_StructInit(COMP_InitTypeDef *COMP_InitStruct);
void COMP_Cmd(u32 selection, FunctionalState state);
void COMP_SwitchCmd(u32 selection, FunctionalState state);
void COMP_LockConfig(u32 selection);

u32 COMP_GetOutputLevel(u32 selection);
void COMP_SetCrv(u8 crv_select, u8 crv_level);
#if defined(__MM0P1) || defined(__MM0Q1) || defined(__MM0S1)
void exCOMP_CrvCmd(FunctionalState state);
void exCOMP_SwitchCrv(u32 crv);
void exCOMP_CrvSrc(u32 src);
#endif

#endif

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__HAL_COMP_H */
////////////////////////////////////////////////////////////////////////////////
