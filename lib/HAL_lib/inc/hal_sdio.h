////////////////////////////////////////////////////////////////////////////////
/// @file     hal_gpio.h
/// @author   AE TEAM
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE GPIO
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
#ifndef __HAL_SDIO_H
#define __HAL_SDIO_H

// Files includes
#include "mm32_types.h"
#include "MM32.h"

#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup GPIO_HAL
/// @brief GPIO HAL modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup GPIO_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Output Maximum frequency selection
////////////////////////////////////////////////////////////////////////////////
#define SDIO_FLAG_CCRCFAIL                  ((uint32_t)0x00000001)
#define SDIO_FLAG_DCRCFAIL                  ((uint32_t)0x00000002)
#define SDIO_FLAG_CTIMEOUT                  ((uint32_t)0x00000004)
#define SDIO_FLAG_DTIMEOUT                  ((uint32_t)0x00000008)
#define SDIO_FLAG_TXUNDERR                  ((uint32_t)0x00000010)
#define SDIO_FLAG_RXOVERR                   ((uint32_t)0x00000020)
#define SDIO_FLAG_CMDREND                   ((uint32_t)0x00000040)
#define SDIO_FLAG_CMDSENT                   ((uint32_t)0x00000080)
#define SDIO_FLAG_DATAEND                   ((uint32_t)0x00000100)
#define SDIO_FLAG_STBITERR                  ((uint32_t)0x00000200)
#define SDIO_FLAG_DBCKEND                   ((uint32_t)0x00000400)
#define SDIO_FLAG_CMDACT                    ((uint32_t)0x00000800)
#define SDIO_FLAG_TXACT                     ((uint32_t)0x00001000)
#define SDIO_FLAG_RXACT                     ((uint32_t)0x00002000)
#define SDIO_FLAG_TXFIFOHE                  ((uint32_t)0x00004000)
#define SDIO_FLAG_RXFIFOHF                  ((uint32_t)0x00008000)
#define SDIO_FLAG_TXFIFOF                   ((uint32_t)0x00010000)
#define SDIO_FLAG_RXFIFOF                   ((uint32_t)0x00020000)
#define SDIO_FLAG_TXFIFOE                   ((uint32_t)0x00040000)
#define SDIO_FLAG_RXFIFOE                   ((uint32_t)0x00080000)
#define SDIO_FLAG_TXDAVL                    ((uint32_t)0x00100000)
#define SDIO_FLAG_RXDAVL                    ((uint32_t)0x00200000)
#define SDIO_FLAG_SDIOIT                    ((uint32_t)0x00400000)
#define SDIO_FLAG_CEATAEND                  ((uint32_t)0x00800000)



////////////////////////////////////////////////////////////////////////////////////////////////////
//SDIO working mode define ,SDIO working mode definition, set through the SD_SetDevice Mode function.
#define SD_POLLING_MODE     0                                                                      /// Query mode. In this mode, it is recommended to increase the setting of SDIO_TRANSFER_CLK_DIV if there are problems with reading and writing.
#define SD_DMA_MODE         1                                                                      /// In DMA mode, it is recommended to increase the setting of SDIO_TRANSFER_CLK_DIV if there are problems with reading and writing.   

////////////////////////////////////////////////////////////////////////////////
/// @brief  SDIO Various error enumeration definitions
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    SD_CMD_CRC_FAIL,                                                                        ///< Command response received (but CRC check failed)
    SD_DATA_CRC_FAIL,                                                                       ///< Data bock sent/received (CRC check Failed)
    SD_CMD_RSP_TIMEOUT,                                                                     ///< Command response timeout
    SD_DATA_TIMEOUT,                                                                        ///< Data time out
    SD_TX_UNDERRUN,                                                                         ///< Transmit FIFO under-run
    SD_RX_OVERRUN,                                                                          ///< Receive FIFO over-run
    SD_START_BIT_ERR,                                                                       ///< Start bit not detected on all data signals in widE bus mode
    SD_CMD_OUT_OF_RANGE,                                                                    ///< CMD's argument was out of range.
    SD_ADDR_MISALIGNED,                                                                     ///< Misaligned address
    SD_BLOCK_LEN_ERR,                                                                       ///< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length
    SD_ERASE_SEQ_ERR,                                                                       ///< An error in the sequence of erase command occurs.
    SD_BAD_ERASE_PARAM,                                                                     ///< An Invalid selection for erase groups
    SD_WRITE_PROT_VIOLATION,                                                                ///< Attempt to program a write protect block
    SD_LOCK_UNLOCK_FAILED,                                                                  ///< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card
    SD_COM_CRC_FAILED,                                                                      ///< CRC check of the previous command failed
    SD_ILLEGAL_CMD,                                                                         ///< Command is not legal for the card state
    SD_CARD_ECC_FAILED,                                                                     ///< Card internal ECC was applied but failed to correct the data
    SD_CC_ERROR,                                                                            ///< Internal card controller error
    SD_GENERAL_UNKNOWN_ERROR,                                                               ///< General or Unknown error
    SD_STREAM_READ_UNDERRUN,                                                                ///< The card could not sustain data transfer in stream read operation.
    SD_STREAM_WRITE_OVERRUN,                                                                ///< The card could not sustain data programming in stream mode
    SD_CID_CSD_OVERWRITE,                                                                   ///< CID/CSD overwrite error
    SD_WP_ERASE_SKIP,                                                                       ///< only partial address space was erased
    SD_CARD_ECC_DISABLED,                                                                   ///< Command has been executed without using internal ECC
    SD_ERASE_RESET,                                                                         ///< Erase sequence was cleared before executing because an out of erase sequence command was received
    SD_AKE_SEQ_ERROR,                                                                       ///< Error in sequence of authentication.
    SD_INVALID_VOLTRANGE,                                                                   ///< SD invalid voltage range,
    SD_ADDR_OUT_OF_RANGE,                                                                   ///< SD addresses are out of range,
    SD_SWITCH_ERROR,                                                                        ///< SD switch error,
    SD_SDIO_DISABLED,                                                                       ///< SD SDIO disability,
    SD_SDIO_FUNCTION_BUSY,                                                                  ///< SD SDIO function busy,
    SD_SDIO_FUNCTION_FAILED,                                                                ///< SD SDIO failed,
    SD_SDIO_UNKNOWN_FUNCTION,                                                               ///< SDIO unknown function,
    SD_INTERNAL_ERROR,                                                                      ///< SD internal error,
    SD_NOT_CONFIGURED,                                                                      ///< SD is not configured,
    SD_REQUEST_PENDING,                                                                     ///< The SD request waits,
    SD_REQUEST_NOT_APPLICABLE,                                                              ///< The SD requirement does not apply,
    SD_INVALID_PARAMETER,                                                                   ///< Invalid SD parameter,
    SD_UNSUPPORTED_FEATURE,                                                                 ///< Features not supported by SD,
    SD_UNSUPPORTED_HW,                                                                      ///< HW not supported by SD,
    SD_ERROR,                                                                               ///< SD error
    SD_OK = 0,///< SD OK
    SD_NoInsert
} SD_Error;
////////////////////////////////////////////////////////////////////////////////
/// @brief  SD card CSD register data
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    u8  CSDStruct;                                                                                ///< CSD structure
    u8  SysSpecVersion;                                                                           ///< System specification version
    u8  Reserved1;                                                                                ///< Reserved
    u8  TAAC;                                                                                     ///< Data read access-time 1
    u8  NSAC;                                                                                     ///< Data read access-time 2 in CLK cycles
    u8  MaxBusClkFrec;                                                                            ///< Max. bus clock frequency
    u16 CardComdClasses;                                                                          ///< Card command classes
    u8  RdBlockLen;                                                                               ///< Max. read data block length
    u8  PartBlockRead;                                                                            ///< Partial blocks for read allowed
    u8  WrBlockMisalign;                                                                          ///< Write block misalignment
    u8  RdBlockMisalign;                                                                          ///< Read block misalignment
    u8  DSRImpl;                                                                                  ///< DSR implemented
    u8  Reserved2;                                                                                ///< Reserved
    u32 DeviceSize;                                                                               ///< Device Size
    u8  MaxRdCurrentVDDMin;                                                                       ///< Max. read current @ VDD min
    u8  MaxRdCurrentVDDMax;                                                                       ///< Max. read current @ VDD max
    u8  MaxWrCurrentVDDMin;                                                                       ///< Max. write current @ VDD min
    u8  MaxWrCurrentVDDMax;                                                                       ///< Max. write current @ VDD max
    u8  DeviceSizeMul;                                                                            ///< Device size multiplier
    u8  EraseGrSize;                                                                              ///< Erase group size
    u8  EraseGrMul;                                                                               ///< Erase group size multiplier
    u8  WrProtectGrSize;                                                                          ///< Write protect group size
    u8  WrProtectGrEnable;                                                                        ///< Write protect group enable
    u8  ManDeflECC;                                                                               ///< Manufacturer default ECC
    u8  WrSpeedFact;                                                                              ///< Write speed factor
    u8  MaxWrBlockLen;                                                                            ///< Max. write data block length
    u8  WriteBlockPaPartial;                                                                      ///< Partial blocks for write allowed
    u8  Reserved3;                                                                                ///< Reserded
    u8  ContentProtectAppli;                                                                      ///< Content protection application
    u8  FileFormatGrouop;                                                                         ///< File format group
    u8  CopyFlag;                                                                                 ///< Copy flag (OTP)
    u8  PermWrProtect;                                                                            ///< Permanent write protection
    u8  TempWrProtect;                                                                            ///< Temporary write protection
    u8  FileFormat;                                                                               ///< File Format
    u8  ECC;                                                                                      ///< ECC code
    u8  CSD_CRC;                                                                                  ///< CSD CRC
    u8  Reserved4;                                                                                ///< always 1
} SD_CSD;

////////////////////////////////////////////////////////////////////////////////
/// @brief  SD card CID register data
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    u8  ManufacturerID;                                                                           ///< ManufacturerID
    u16 OEM_AppliID;                                                                              ///< OEM/Application ID
    u32 ProdName1;                                                                                ///< Product Name part1
    u8  ProdName2;                                                                                ///< Product Name part2
    u8  ProdRev;                                                                                  ///< Product Revision
    u32 ProdSN;                                                                                   ///< Product Serial Number
    u8  Reserved1;                                                                                ///< Reserved1
    u16 ManufactDate;                                                                             ///< Manufacturing Date
    u8  CID_CRC;                                                                                  ///< CID CRC
    u8  Reserved2;                                                                                ///< always 1
} SD_CID;
////////////////////////////////////////////////////////////////////////////////
/// @brief  SD state
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    SD_CARD_READY                  = ((uint32_t)0x00000001),
    SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
    SD_CARD_STANDBY                = ((uint32_t)0x00000003),
    SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
    SD_CARD_SENDING                = ((uint32_t)0x00000005),
    SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
    SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
    SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
    SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
} SDCardState;

////////////////////////////////////////////////////////////////////////////////
/// @brief  SD message ,include CSD,CID data
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    SD_CSD SD_csd;
    SD_CID SD_cid;
    long long CardCapacity;
    u32 CardBlockSize;
    u16 RCA;
    u8 CardType;
} SD_CardInfo;
////////////////////////////////////////////////////////////////////////////////
/// @brief  SDIO init
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    uint8_t SDIO_MDEN;
    uint8_t SDIO_DATWT;
    uint8_t SDIO_SelPTSM;
    uint8_t SDIO_CLKSP;
    uint8_t SDIO_OUTM;
    uint8_t SDIO_SelSM;
    uint8_t SDIO_OPMSel;
} SDIO_InitTypeDef;

////////////////////////////////////////////////////////////////////////////////
#define SD_CMD_GO_IDLE_STATE                       ((u8)0)
#define SD_CMD_SEND_OP_COND                        ((u8)1)
#define SD_CMD_ALL_SEND_CID                        ((u8)2)
#define SD_CMD_SET_REL_ADDR                        ((u8)3)
#define SD_CMD_SET_DSR                             ((u8)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((u8)5)
#define SD_CMD_HS_SWITCH                           ((u8)6)
#define SD_CMD_SEL_DESEL_CARD                      ((u8)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((u8)8)
#define SD_CMD_SEND_CSD                            ((u8)9)
#define SD_CMD_SEND_CID                            ((u8)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((u8)11)
#define SD_CMD_STOP_TRANSMISSION                   ((u8)12)
#define SD_CMD_SEND_STATUS                         ((u8)13)
#define SD_CMD_HS_BUSTEST_READ                     ((u8)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((u8)15)
#define SD_CMD_SET_BLOCKLEN                        ((u8)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((u8)17)
#define SD_CMD_READ_MULT_BLOCK                     ((u8)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((u8)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((u8)20)
#define SD_CMD_SET_BLOCK_COUNT                     ((u8)23)
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((u8)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((u8)25)
#define SD_CMD_PROG_CID                            ((u8)26)
#define SD_CMD_PROG_CSD                            ((u8)27)
#define SD_CMD_SET_WRITE_PROT                      ((u8)28)
#define SD_CMD_CLR_WRITE_PROT                      ((u8)29)
#define SD_CMD_SEND_WRITE_PROT                     ((u8)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((u8)32)
#define SD_CMD_SD_ERASE_GRP_END                    ((u8)33)
#define SD_CMD_ERASE_GRP_START                     ((u8)35)
#define SD_CMD_ERASE_GRP_END                       ((u8)36)
#define SD_CMD_ERASE                               ((u8)38)
#define SD_CMD_FAST_IO                             ((u8)39)
#define SD_CMD_GO_IRQ_STATE                        ((u8)40)
#define SD_CMD_LOCK_UNLOCK                         ((u8)42)
#define SD_CMD_APP_CMD                             ((u8)55)
#define SD_CMD_GEN_CMD                             ((u8)56)
#define SD_CMD_NO_CMD                              ((u8)64)
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Following commands are SD Card Specific commands.
/// @{
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((u8)6)
#define SD_CMD_SD_APP_STAUS                        ((u8)13)
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((u8)22)
#define SD_CMD_SD_APP_OP_COND                      ((u8)41)
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((u8)42)
#define SD_CMD_SD_APP_SEND_SCR                     ((u8)51)
#define SD_CMD_SDIO_RW_DIRECT                      ((u8)52)
#define SD_CMD_SDIO_RW_EXTENDED                    ((u8)53)

#define SD_CMD_SD_APP_GET_MKB                      ((u8)43)
#define SD_CMD_SD_APP_GET_MID                      ((u8)44)
#define SD_CMD_SD_APP_SET_CER_RN1                  ((u8)45)
#define SD_CMD_SD_APP_GET_CER_RN2                  ((u8)46)
#define SD_CMD_SD_APP_SET_CER_RES2                 ((u8)47)
#define SD_CMD_SD_APP_GET_CER_RES1                 ((u8)48)
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((u8)18)
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((u8)25)
#define SD_CMD_SD_APP_SECURE_ERASE                 ((u8)38)
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((u8)49)
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((u8)48)
/// @}
////////////////////////////////////////////////////////////////////////////////
/// @defgroup SD support define.
/// @{
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((u32)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((u32)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((u32)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((u32)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((u32)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((u32)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((u32)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((u32)0x00000007)
/// @}

#ifndef NULL
#define NULL 0
#endif
#define SDIO_STATIC_FLAGS               ((u32)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((u32)0x00010000)
#define SDIO_DATATIMEOUT                ((u32)0xFFFFFFFF)
#define SDIO_FIFO_Address               ((u32)0x40018080)

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Mask for errors Card Status R1 (OCR Register)
/// @{
#define SD_OCR_ADDR_OUT_OF_RANGE        ((u32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((u32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((u32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((u32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((u32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((u32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((u32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((u32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((u32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((u32)0x00200000)
#define SD_OCR_CC_ERROR                 ((u32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((u32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((u32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((u32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((u32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((u32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((u32)0x00004000)
#define SD_OCR_ERASE_RESET              ((u32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((u32)0x00000008)
#define SD_OCR_ERRORBITS                ((u32)0xFDFFE008)
/// @}
////////////////////////////////////////////////////////////////////////////////
/// @defgroup Masks for R6 Response
/// @{
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((u32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((u32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((u32)0x00008000)
/// @}
#define SD_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define SD_HIGH_CAPACITY                ((u32)0x40000000)
#define SD_STD_CAPACITY                 ((u32)0x00000000)
#define SD_CHECK_PATTERN                ((u32)0x000001AA)
#define SD_VOLTAGE_WINDOW_MMC           ((u32)0x80FF8000)

#define SD_MAX_VOLT_TRIAL               ((u32)0x0000FFFF)
#define SD_ALLZERO                      ((u32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((u32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((u32)0x00010000)
#define SD_CARD_LOCKED                  ((u32)0x02000000)
#define SD_CARD_PROGRAMMING             ((u32)0x00000007)
#define SD_CARD_RECEIVING               ((u32)0x00000006)
#define SD_DATATIMEOUT                  ((u32)0xFFFFFFFF)
#define SD_0TO7BITS                     ((u32)0x000000FF)
#define SD_8TO15BITS                    ((u32)0x0000FF00)
#define SD_16TO23BITS                   ((u32)0x00FF0000)
#define SD_24TO31BITS                   ((u32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((u32)0x01FFFFFF)

#define SD_HALFFIFO                     ((u32)0x00000008)
#define SD_HALFFIFOBYTES                ((u32)0x00000020)

#define SD_CCCC_LOCK_UNLOCK             ((u32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((u32)0x00000040)
#define SD_CCCC_ERASE                   ((u32)0x00000020)


#define SDIO_SEND_IF_COND               ((u32)0x00000008)

#define SDIO_Response_No                    ((u32)0x00)
#define SDIO_Response_Short                 ((u32)0x01)
#define SDIO_Response_Long                  ((u32)0x03)

#define SDIO_DataBlockSize_1b               ((u32)0x00000000)
#define SDIO_DataBlockSize_2b               ((u32)0x00000001)
#define SDIO_DataBlockSize_4b               ((u32)0x00000002)
#define SDIO_DataBlockSize_8b               ((u32)0x00000003)
#define SDIO_DataBlockSize_16b              ((u32)0x00000004)
#define SDIO_DataBlockSize_32b              ((u32)0x00000005)
#define SDIO_DataBlockSize_64b              ((u32)0x00000006)
#define SDIO_DataBlockSize_128b             ((u32)0x00000007)
#define SDIO_DataBlockSize_256b             ((u32)0x00000008)
#define SDIO_DataBlockSize_512b             ((u32)0x00000009)
#define SDIO_DataBlockSize_1024b            ((u32)0x0000000A)
#define SDIO_DataBlockSize_2048b            ((u32)0x0000000B)
#define SDIO_DataBlockSize_4096b            ((u32)0x0000000C)
#define SDIO_DataBlockSize_8192b            ((u32)0x0000000D)
#define SDIO_DataBlockSize_16384b           ((u32)0x0000000E)

//Define the data block length when the block data transfer mode is selected:
//0000: (0 decimal) lock length  = 2^0 = 1 byte
//0001: (1 decimal) lock length  = 2^1 = 2 bytes
//0010: (2 decimal) lock length  = 2^2 = 4 bytes
//0011: (3 decimal) lock length  = 2^3 = 8 bytes
//0100: (4 decimal) lock length  = 2^4 = 16 bytes
//0101: (5 decimal) lock length  = 2^5 = 32 bytes
//0110: (6 decimal) lock length  = 2^6 = 64 bytes
//0111: (7 decimal) lock length  = 2^7 = 128 bytes
//1000: (8 decimal) lock length  = 2^8 = 256 bytes
//1001: (9 decimal) lock length  = 2^9 = 512 bytes
//1010: (10 decimal) lock length = 2^10 = 1024 bytes
//1011: (11 decimal) lock length = 2^11 = 2048 bytes
//1100: (12 decimal) lock length = 2^12 = 4096 bytes
//1101: (13 decimal) lock length = 2^13 = 8192 bytes
//1110: (14 decimal) lock length = 2^14 = 16384 bytes
//1111: (15 decimal) reserved

#define SDIO_TransferDir_ToCard             ((u32)0x00000000)
#define SDIO_TransferDir_ToSDIO             ((u32)0x00000002)

#define SDIO_Wait_No                        ((u32)0x00000000) // SDIO No Wait, TimeOut is enabled 
#define SDIO_Wait_IT                        ((u32)0x00000100) //SDIO Wait Interrupt Request 
#define SDIO_Wait_Pend                      ((u32)0x00000200) // SDIO Wait End of transfer 


////////////////////////////////////////////////////////////////////////////////
/// @defgroup I2C_Exported_Functions
/// @{
void SDIO_DeInit(void);
void SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct);
void SDIO_ClockSet(uint32_t value);
void SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct);
void SDIO_ITConfig(uint32_t SDIO_IT, FunctionalState state);
void SDIO_CRCConfig(uint32_t SDIO_CRC, FunctionalState state);
void SDIO_Clock_Set(u8 clkdiv);
void SDIO_Send_Cmd(uint8_t cmdindex, uint8_t waitrsp, uint32_t arg);
SD_Error SD_PowerOFF(void);
SD_Error CmdError(void);
SD_Error CmdResp2Error(void);
SD_Error CmdResp3Error(void);
SD_Error CmdResp6Error(u8 cmd, u16* prca);
SD_Error CmdResp7Error(void);
SD_Error CmdResp1Error(u8 cmd);
void SDIO_Send_Data_Cfg(u32 datatimeout, u32 datalen, u8 blksize, u8 dir);
void SDIO_ClearITPendingBit(uint32_t SDIO_IT);
FlagStatus SDIO_GetFlagStatus(uint32_t SDIO_FLAG);
uint32_t SDIO_GetTimeOutCounter(void);
uint32_t SDIO_ReadData(void);
void SDIO_WriteData(u32 tempbuff);
void SDIO_DMACmd(FunctionalState state);
/// @}


#ifdef __cplusplus
}
#endif


/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
