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
#ifndef __BSP_SDIO_H
#define __BSP_SDIO_H

#include "hal_sdio.h"

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

//SD_ReadDisk/SD_WriteDisk Function buffer. When the address of the data cache of these two functions is not 4-byte aligned,
// This array is needed to ensure that the address of the data cache is 4-byte aligned.


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Variables
/// @{

#ifdef _BSP_SDIO_C_
#define GLOBAL

GLOBAL u8 CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1;
GLOBAL u32 RCA = 0;
GLOBAL u8 DeviceMode = SD_DMA_MODE;
GLOBAL u8 StopCondition = 0;
GLOBAL u16 ob_cnt;
volatile GLOBAL u8 TransferEnd = 0;
volatile GLOBAL SD_Error TransferError = SD_OK;



#else
#define GLOBAL extern
#endif

GLOBAL u8 CardType;
GLOBAL SD_CardInfo SDCardInfo;
GLOBAL u32 CSD_Tab[4];
GLOBAL u32 CID_Tab[4];
GLOBAL u32 RCA; //relative address (RCA) data
////////////////////////////////////////////////////////////////


#if defined ( __CC_ARM )
__align(4)
GLOBAL u8 SDIO_DATA_BUFFER[512];
#elif defined ( __ICCARM__ )
GLOBAL u8 SDIO_DATA_BUFFER[512];
#endif

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Functions
/// @{

void BSP_SDIO_Configure();
void SDIO_ConfigInit(void);

SD_Error SD_EnableWideBusOperation(u32 wmode);
SD_Error SDEnWideBus(u8 enx);
SD_Error SD_Init(void);
SD_Error SD_PowerON(void);
SD_Error SD_InitializeCards(void);
SD_Error SD_GetCardInfo(SD_CardInfo* cardinfo);
SD_Error SD_SelectDeselect(u32 addr);
SD_Error SD_SendStatus(u32* pcardstatus);
SD_Error SD_ReadOneBlockPolling(u8* buf, long long  addr, u16 blksize);
SD_Error SD_WriteOneBlockPolling(u8* buf, long long addr, u16 blksize);
SD_Error SD_ReadBlocks(u8* buf, long long  addr, u16 blksize, u32 nblks);
SD_Error SD_WriteBlocks(u8* buf, long long addr, u16 blksize);
SD_Error SD_ReadMulBlocks(u8* buf, long long addr, u16 blksize, u32 nblks);
SD_Error SD_WriteMulBlocks(u8* buf, long long addr, u16 blksize, u32 nblks);
SD_Error SD_ProcessIRQSrc(void);
SD_Error IsCardProgramming(u8* pstatus);
SDCardState SD_GetState(void);
void SD_DMA_Config(u32* mbuf, u32 bufsize, u8 dir);
u8 SD_ReadDisk(u8* buf, u32 sector, u32 cnt);
u8 SD_WriteDisk(u8* buf, u32 sector, u32 cnt);


/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__BSP_BEEP_H */
////////////////////////////////////////////////////////////////////////////////
