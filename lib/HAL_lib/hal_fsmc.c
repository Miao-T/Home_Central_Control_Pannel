////////////////////////////////////////////////////////////////////////////////
/// @file     hal_fsmc.c
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE FSMC FIRMWARE FUNCTIONS.
///                Interface with SRAM, PSRAM, NOR memories
///                Interrupts and flags management
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
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _HAL_FSMC_C_

// Files includes
#include "mm32_types.h"
#include "mm32.h"

#include "hal_fsmc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FSMC_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FSMC_Exported_Functions
/// @{

#if defined(__MM3U1)
void FSMC_NORSRAMDeInit(u32 FSMC_Bank)
{
}

////////////////////////////////////////////////////////////////////////////////
void FSMC_NORSRAMStructInit(FSMC_InitTypeDef* init_struct)
{
    init_struct->FSMC_Mode = 0;
    init_struct->FSMC_AddrDataMode = 0;

    init_struct->FSMC_TimingRegSelect = 0;
    init_struct->FSMC_MemSize = 0;
    init_struct->FSMC_MemType = 0;

    init_struct->FSMC_TimingStruct->FSMC_SMReadPipe = 0;
    init_struct->FSMC_TimingStruct->FSMC_ReadyMode = 0;
    init_struct->FSMC_TimingStruct->FSMC_WritePeriod = 0;
    init_struct->FSMC_TimingStruct->FSMC_WriteHoldTime = 0;
    init_struct->FSMC_TimingStruct->FSMC_AddrSetTime = 0;
    init_struct->FSMC_TimingStruct->FSMC_ReadPeriod = 0;
//    init_struct->FSMC_TimingStruct->FSMC_TimingMode=0;

    init_struct->FSMC_MemoryDataWidth = 0;
}

////////////////////////////////////////////////////////////////////////////////
void FSMC_NORSRAMInit(FSMC_InitTypeDef* init_struct)
{

    SYSCFG->CFGR = (u32)(init_struct->FSMC_Mode |
                   init_struct->FSMC_AddrDataMode);
#if 0
    FSMC->MR = (u32)(init_struct->FSMC_TimingRegSelect |
                   init_struct->FSMC_MemSize |
                   init_struct->FSMC_MemType);

    FSMC->TR0 = (u32)((init_struct->FSMC_TimingStruct->FSMC_SMReadPipe << 28)    |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_ReadyMode << 26)     |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_WritePeriod << 10)   |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_WriteHoldTime << 8)  |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_AddrSetTime << 6)    |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_ReadPeriod << 0 ));

    FSMC->CR = (u32)((init_struct->FSMC_MemoryDataWidth << 7) | 0x00000001);
#else
    FSMC->SMSKR0 = (u32)init_struct->FSMC_TimingRegSelect |
                   init_struct->FSMC_MemSize |
                   init_struct->FSMC_MemType;

    FSMC->SMTMGR_SET0 = (u32)(init_struct->FSMC_TimingStruct->FSMC_SMReadPipe << 28)    |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_ReadyMode << 26)     |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_WritePeriod << 10)   |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_WriteHoldTime << 8)  |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_AddrSetTime << 6)    |
                        (u32)(init_struct->FSMC_TimingStruct->FSMC_ReadPeriod << 0 ) ;//   |
    // (u32)(init_struct->FSMC_TimingStruct->FSMC_TimingMode  ) ;

    FSMC->SMCTLR = (u32)(init_struct->FSMC_MemoryDataWidth << 7) | 0x00000001;
#endif
}

////////////////////////////////////////////////////////////////////////////////
void FSMC_NORSRAMCmd(u32 FSMC_Bank, FunctionalState NewState)
{
    if (NewState != DISABLE) {
        // Enable the selected NOR/SRAM Bank

    }
    else {
        // Disable the selected NOR/SRAM Bank

    }
}

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif // __HAL_FSMC_C 
////////////////////////////////////////////////////////////////////////////////
