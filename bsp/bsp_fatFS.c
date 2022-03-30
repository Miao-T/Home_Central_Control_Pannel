////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_FATFS.C
/// @author   CHENDO
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UART BSP LAYER FUNCTIONS.
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
#define _BSP_FATFS_C_

// Files includes  -------------------------------------------------------------
#include "bsp.h"
#include "bsp_fatFS.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FATFSBSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FATFS_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FF_GEN_DRV
/// @{
#if defined(__USE_FATFS)

#include "diskio.h"
#include "ff.h"
#include "stdint.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief  Links a compatible diskio driver/lun id and increments the number of active
///         linked drivers.
///  @note   The number of linked drivers (volumes) is up to 10 due to FatFs limits.
///  @param  drv: pointer to the disk IO Driver structure
///  @param  path: pointer to the logical drive path
///  @param  lun : only used for USB Key Disk to add multi-lun management
///         else the parameter must be equal to 0
///  @retval Returns 0 in case of success, otherwise 1.
////////////////////////////////////////////////////////////////////////////////
uint8_t FATFS_LinkDriverEx(const Diskio_drvTypeDef* drv, char* path, uint8_t lun)
{
    uint8_t ret = 1;
    uint8_t DiskNum = 0;
    if(disk.nbr < FF_VOLUMES) {
        disk.is_initialized[disk.nbr] = 0;
        disk.drv[disk.nbr] = drv;
        disk.lun[disk.nbr] = lun;
        DiskNum = disk.nbr++;
        path[0] = DiskNum + '0';
        path[1] = ':';
        path[2] = '/';
        path[3] = 0;
        ret = 0;
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Links a compatible diskio driver and increments the number of active
///         linked drivers.
///  @note   The number of linked drivers (volumes) is up to 10 due to FatFs limits
///  @param  drv: pointer to the disk IO Driver structure
///  @param  path: pointer to the logical drive path
///  @retval Returns 0 in case of success, otherwise 1.
////////////////////////////////////////////////////////////////////////////////
uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef* drv, char* path)
{
    return FATFS_LinkDriverEx(drv, path, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Unlinks a diskio driver and decrements the number of active linked
///         drivers.
///  @param  path: pointer to the logical drive path
///  @param  lun : not used
///  @retval Returns 0 in case of success, otherwise 1.
////////////////////////////////////////////////////////////////////////////////
uint8_t FATFS_UnLinkDriverEx(char* path, uint8_t lun)
{
    uint8_t DiskNum = 0;
    uint8_t ret = 1;
    if(disk.nbr >= 1) {
        DiskNum = path[0] - '0';
        if(disk.drv[DiskNum] != 0) {
            disk.drv[DiskNum] = 0;
            disk.lun[DiskNum] = 0;
            disk.nbr--;
            ret = 0;
        }
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Unlinks a diskio driver and decrements the number of active linked
///         drivers.
///  @param  path: pointer to the logical drive path
///  @retval Returns 0 in case of success, otherwise 1.
////////////////////////////////////////////////////////////////////////////////
uint8_t FATFS_UnLinkDriver(char* path)
{
    return FATFS_UnLinkDriverEx(path, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets number of linked drivers to the FatFs module.
///  @param  None
///  @retval Number of attached drivers.
////////////////////////////////////////////////////////////////////////////////
uint8_t FATFS_GetAttachedDriversNbr(void)
{
    return disk.nbr;
}










#include "bsp_sdio.h"
////////////////////////////////////////////////////////////////////////////////
#if defined(SDMMC_DATATIMEOUT)
#define SD_TIMEOUT SDMMC_DATATIMEOUT
#elif defined(SD_DATATIMEOUT)
#define SD_TIMEOUT SD_DATATIMEOUT
#else
#define SD_TIMEOUT 30 * 1000
#endif

#define SD_DEFAULT_BLOCK_SIZE 512


static volatile DSTATUS Stat = STA_NOINIT;

DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
#endif
#if _USE_IOCTL == 1
DRESULT SD_ioctl (BYTE, BYTE, void*);
#endif

const Diskio_drvTypeDef  SD_Driver = {
    SD_initialize,
    SD_status,
    SD_read,
#if  _USE_WRITE == 1
    SD_write,
#endif

#if  _USE_IOCTL == 1
    SD_ioctl,
#endif
};


//static DSTATUS SD_CheckStatus(BYTE lun)
//{
//  Stat = STA_NOINIT;

//  if(BSP_SD_GetCardState() == MSD_OK)
//  {
//    Stat &= ~STA_NOINIT;
//  }

//  return Stat;
//}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes a Drive
/// @param  lun : not used
/// @retval DSTATUS: Operation status
////////////////////////////////////////////////////////////////////////////////
DSTATUS SD_initialize(BYTE lun)
{
    Stat = STA_NOINIT;

    Stat = SD_Init();

    if(Stat)return  STA_NOINIT;
    else return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets Disk Status
/// @param  lun : not used
/// @retval DSTATUS: Operation status
////////////////////////////////////////////////////////////////////////////////
DSTATUS SD_status(BYTE lun)
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Reads Sector(s)
/// @param  lun : not used
/// @param  *buff: Data buffer to store read data
/// @param  sector: Sector address (LBA)
/// @param  count: Number of sectors to read (1..128)
/// @retval DRESULT: Operation result
////////////////////////////////////////////////////////////////////////////////

DRESULT SD_read(BYTE lun, BYTE* buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;

    if (!count)return RES_PARERR;
    res = (DRESULT)SD_ReadDisk(buff, sector, count);
    while(res) {
        SD_Init();
        res = (DRESULT)SD_ReadDisk(buff, sector, count);
        //printf("sd rd error:%d\r\n",res);
    }
    if(res == 0x00)return RES_OK;
    else return RES_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Writes Sector(s)
/// @param  lun : not used
/// @param  *buff: Data to be written
/// @param  sector: Sector address (LBA)
/// @param  count: Number of sectors to write (1..128)
/// @retval DRESULT: Operation result
////////////////////////////////////////////////////////////////////////////////
#if _USE_WRITE == 1

DRESULT SD_write(BYTE lun, const BYTE* buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    if (!count)return RES_PARERR;
    res = (DRESULT)SD_WriteDisk((u8*)buff, sector, count);
    while(res) {
        SD_Init();
        res = (DRESULT)SD_WriteDisk((u8*)buff, sector, count);
        //printf("sd wr error:%d\r\n",res);
    }
    if(res == 0x00)return RES_OK;
    else return RES_ERROR;
}
#endif


////////////////////////////////////////////////////////////////////////////////
/// @brief  I/O control operation
/// @param  lun : not used
/// @param  cmd: Control code
/// @param  *buff: Buffer to send/receive control data
/// @retval DRESULT: Operation result
////////////////////////////////////////////////////////////////////////////////
#if _USE_IOCTL == 1
DRESULT SD_ioctl(BYTE lun, BYTE cmd, void* buff)
{
    DRESULT res = RES_ERROR;
    switch(cmd) {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(DWORD*)buff = 512;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = SDCardInfo.CardBlockSize;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = SDCardInfo.CardCapacity / 512;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
    }
    return res;
}
#endif




















////////////////////////////////////////////////////////////////////////////////
DWORD get_fattime(void)
{
    /* USER CODE BEGIN get_fattime */
    return 0;
    /* USER CODE END get_fattime */
}

////////////////////////////////////////////////////////////////////////////////
void BSP_FATFS_Configure()
{
    retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
}

/// @}

/// @}

/// @}

#endif