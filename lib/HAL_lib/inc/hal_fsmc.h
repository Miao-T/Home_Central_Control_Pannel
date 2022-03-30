////////////////////////////////////////////////////////////////////////////////
/// @file     hal_fsmc.h
/// @author   AE TEAM
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SDIO
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
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#ifndef __HAL_FSMC_H
#define __HAL_FSMC_H

// Files includes
#include "mm32_types.h"
#include "common.h"
#include "mm32.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup FSMC_HAL
/// @brief FSMC HAL modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup FSMC_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief FSMC_interrupts_define
////////////////////////////////////////////////////////////////////////////////

#if defined(__MM3U1)
//register set0 register set1 register set2
#define FSMC_TimingReg_Set0    ((uint32_t)0x00000000)
#define FSMC_TimingReg_Set1    ((uint32_t)0x00000100)
#define FSMC_TimingReg_Set2    ((uint32_t)0x00000200)

#define FSMC_MemSize_None   ((uint32_t)0x00000000)
#define FSMC_MemSize_64KB   ((uint32_t)0x00000001)
#define FSMC_MemSize_128KB  ((uint32_t)0x00000002)
#define FSMC_MemSize_256KB  ((uint32_t)0x00000002)
#define FSMC_MemSize_512KB  ((uint32_t)0x00000004)
#define FSMC_MemSize_1MB    ((uint32_t)0x00000005)
#define FSMC_MemSize_2MB    ((uint32_t)0x00000006)
#define FSMC_MemSize_4MB    ((uint32_t)0x00000007)
#define FSMC_MemSize_8MB    ((uint32_t)0x00000008)
#define FSMC_MemSize_16MB   ((uint32_t)0x00000009)
#define FSMC_MemSize_32MB   ((uint32_t)0x0000000A)
#define FSMC_MemSize_64MB   ((uint32_t)0x0000000B)
#define FSMC_MemSize_128MB  ((uint32_t)0x0000000C)
#define FSMC_MemSize_256MB  ((uint32_t)0x0000000D)
#define FSMC_MemSize_512MB  ((uint32_t)0x0000000E)
#define FSMC_MemSize_1GB    ((uint32_t)0x0000000F)
#define FSMC_MemSize_2GB    ((uint32_t)0x00000010)
#define FSMC_MemSize_4GB    ((uint32_t)0x00000011)

typedef enum {
    FSMC_DatWidth_set0 = 0,
    FSMC_DatWidth_set1,
    FSMC_DatWidth_set2
} FSMCDataWidthSet_TypeDef;

typedef struct {
    uint32_t FSMC_SMReadPipe;   
    uint32_t FSMC_ReadyMode;    
    uint32_t FSMC_WritePeriod;  
    uint32_t FSMC_WriteHoldTime;
    uint32_t FSMC_AddrSetTime;  
    uint32_t FSMC_ReadPeriod;   
    //uint32_t FSMC_TimingMode;
} FSMC_TimingInitTypeDef;

typedef struct {
    uint32_t FSMC_Mode;
    //uint32_t FSMC_DataAddressMux;
    uint32_t FSMC_MemoryDataWidth;
    uint32_t FSMC_TimingRegSelect;
    uint32_t FSMC_MemSize;
    uint32_t FSMC_MemType;
    uint32_t FSMC_AddrDataMode;
    FSMC_TimingInitTypeDef* FSMC_TimingStruct;
} FSMC_InitTypeDef;

#define FSMC_DataWidth_16bits       ((uint16_t)0x0000)  /* 16bits */
#define FSMC_DataWidth_32bits       ((uint16_t)0x0001)  /* 32bits */
#define FSMC_DataWidth_64bits       ((uint16_t)0x0002)  /* 64bits */
#define FSMC_DataWidth_128bits      ((uint16_t)0x0003)  /* 128bits */
#define FSMC_DataWidth_8bits        ((uint16_t)0x0004)  /* 8bits */

#define FSMC_MemType_SDRAM          ((uint32_t)0x0<<5)
#define FSMC_MemType_NorSRAM        ((uint32_t)0x1<<5)
#define FSMC_MemType_FLASH          ((uint32_t)0x2<<5)
#define FSMC_MemType_RESERVED       ((uint32_t)0x3<<5)
//SYSCFG_CFGR1
#define FSMC_Mode_6800              ((uint32_t)0x40000000)
#define FSMC_Mode_8080              ((uint32_t)0x20000000)
#define FSMC_Mode_NorFlash          ((uint32_t)0x00000000)

#define FSMC_AddrDataMUX            ((uint32_t)0x00000000)
#define FSMC_AddrDataDeMUX          ((uint32_t)0x10000000)

//#define FSMC_Synchronization        ((uint32_t)0x08000000)
//#define FSMC_Asynchronization       ((uint32_t)0x00000000)

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup GPIO_Exported_Variables
/// @{

#ifdef _HAL_FSMC_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif


#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup GPIO_Exported_Functions
/// @{

void FSMC_NORSRAMDeInit(u32 FSMC_Bank);
void FSMC_NORSRAMStructInit(FSMC_InitTypeDef* init_struct);
void FSMC_NORSRAMInit(FSMC_InitTypeDef* init_struct);
void FSMC_NORSRAMCmd(u32 FSMC_Bank, FunctionalState NewState);

#endif
/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif // __HAL_FSMC_H 
////////////////////////////////////////////////////////////////////////////////
