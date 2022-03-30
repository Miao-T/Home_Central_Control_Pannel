////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_UART.H
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE UART
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

#ifndef __BSP_FATFS_H
#define __BSP_FATFS_H

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART_BSP
/// @brief UART BSP modules
/// @{
///

#if defined(__USE_FATFS)

#include "ff.h"
#include "diskio.h"
#include "ffconf.h"

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART_Exported_Types
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART_Exported_Variables
/// @{

/* Disk IO Driver structure definition */
typedef struct {
    DSTATUS (*disk_initialize) (BYTE);                                          /* Initialize Disk Drive */
    DSTATUS (*disk_status)     (BYTE);                                          /* Get Disk Status */
    DRESULT (*disk_read)       (BYTE, BYTE*, DWORD, UINT);                      /* Read Sector(s) */
#if _USE_WRITE == 1
    DRESULT (*disk_write)      (BYTE, const BYTE*, DWORD, UINT);                /* Write Sector(s) when _USE_WRITE = 0 */
#endif
#if _USE_IOCTL == 1
    DRESULT (*disk_ioctl)      (BYTE, BYTE, void*);                             /* I/O control operation when _USE_IOCTL = 1 */
#endif
} Diskio_drvTypeDef;

/* Global Disk IO Drivers structure definition */
typedef struct {
    uint8_t                 is_initialized[FF_VOLUMES];
    const Diskio_drvTypeDef* drv[FF_VOLUMES];
    uint8_t                 lun[FF_VOLUMES];
    volatile uint8_t        nbr;
} Disk_drvTypeDef;

////////////////////////////////////////////////////////////////////////////////
#ifdef _BSP_FATFS_C_
#define GLOBAL

GLOBAL Disk_drvTypeDef disk = {{0}, {0}, {0}, 0};

#else
#define GLOBAL extern
#endif

GLOBAL Disk_drvTypeDef disk;
GLOBAL uint8_t retSD;
GLOBAL char SDPath[4];

#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART_Exported_Functions
/// @{

uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef* drv, char* path);
uint8_t FATFS_UnLinkDriver(char* path);
uint8_t FATFS_LinkDriverEx(const Diskio_drvTypeDef* drv, char* path, BYTE lun);
uint8_t FATFS_UnLinkDriverEx(char* path, BYTE lun);
uint8_t FATFS_GetAttachedDriversNbr(void);

void BSP_FATFS_Configure(void);

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__BSP_UART_H */
////////////////////////////////////////////////////////////////////////////////

#endif