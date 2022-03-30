////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_ETH.C
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
#define _BSP_SDIO_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_sdio.h"
#include "hal_nvic.h"

#include "bsp.h"
#include "common.h"

#include "bsp_sdio.h"


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
/// @brief  SDIO GPIO congfiguration
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SDIO_PIN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    COMMON_EnableIpClock(emCLOCK_GPIOC);
    COMMON_EnableIpClock(emCLOCK_GPIOD);
    COMMON_EnableIpClock(emCLOCK_EXTI);
    
 /* PC8: SDCard_DAT0; PC9: SDCard_DAT1; PC10: SDCard_DAT2; 
    PC11: SDCard_DAT3; PC12: SDCard_CLK; PD2: SDCard_CMD */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_12);                       //PD2   SDIO_CMD
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_12);                       //PC8   SDIO_D0
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_12);                       //PC9   SDIO_D1
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_12);                      //PC10  SDIO_D2
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_12);                      //PC11  SDIO_D3
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_12);                      //PC12  SDIO_CLK

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | \
        GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SDIO Detect Pin GPIO configuration
/// @param  PC7: SDIO_DET
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SDIO_Detect_Pin_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    COMMON_EnableIpClock(emCLOCK_GPIOC);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void SDIO_ConfigInit(void)
{
    SDIO_InitTypeDef SDIO_InitStruct;

    SDIO_PIN_GPIO_Config();
    SDIO_Detect_Pin_Config();

    SDIO_DeInit();
    COMMON_EnableIpClock(emCLOCK_SDIO);
    
    SDIO_ClockSet(0x2F);
    SDIO_StructInit(&SDIO_InitStruct);
    SDIO_InitStruct.SDIO_OPMSel = SDIO_MMC_CTRL_OPMSel;
    SDIO_InitStruct.SDIO_SelPTSM = SDIO_MMC_CTRL_SelSM;
    SDIO_InitStruct.SDIO_DATWT = SDIO_MMC_CTRL_DATWT;
    SDIO_Init(&SDIO_InitStruct);

    SDIO_CRCConfig(SDIO_MMC_CRCCTL_CMD_CRCEN | SDIO_MMC_CRCCTL_DAT_CRCEN, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}
////////////////////////////////////////////////////////////////////////////////
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}


////////////////////////////////////////////////////////////////////////////////
SD_Error SD_PowerON(void)
{
    u32 timeout = SDIO_CMD0TIMEOUT;
    u32 i, j, response, count, validvoltage, SDType;
    SD_Error errorstatus;
    validvoltage = 0;
    count = 0;
    errorstatus = SD_OK;
    
    for (i = 0; i < 74; i++) {
        SDIO_Send_Cmd(SD_CMD_GO_IDLE_STATE, SDIO_Response_No, 0);               //Send CMD0 into the IDLE STAGE mode command.
        errorstatus = CmdError();
        if (errorstatus == SD_OK)   break;
    }
    if (errorstatus)    return errorstatus;
    j = 50000;
    while(j--);
    
    SDIO_Send_Cmd(SDIO_SEND_IF_COND, SDIO_Response_Short, SD_CHECK_PATTERN);    //Send CMD8, short response, check SD card interface features.
    errorstatus = CmdResp7Error();                                              //Wait for R7 to respond
    if (errorstatus == SD_OK) {                                                 //R7 responds normally
        CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0;                              //SD 2.0
        SDType = SD_HIGH_CAPACITY;                                              //High-capacity card
    }
    j = 50000;
    while(j--);
    
    SDIO_Send_Cmd(SD_CMD_APP_CMD, SDIO_Response_Short, 0);                      //Send CMD55, short response
    errorstatus = CmdResp1Error(SD_CMD_APP_CMD);                                //Waiting for R1 response
    if (errorstatus == SD_OK) {                                                 //SD2.0/SD 1.1,Otherwise it's an MMC card
        while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL)) {
            SDIO_Send_Cmd(SD_CMD_APP_CMD, SDIO_Response_Short, 0);              //Send CMD55, short response
            errorstatus = CmdResp1Error(SD_CMD_APP_CMD);                        //Waiting for R1 response
            if (errorstatus != SD_OK)   return errorstatus;                     //Response error
            SDIO_Send_Cmd(SD_CMD_SD_APP_OP_COND, SDIO_Response_Short, SD_VOLTAGE_WINDOW_SD | SDType);   //Send ACMD41, short response
            errorstatus = CmdResp3Error();                                      //Waiting for R3 response
            if (errorstatus != SD_OK)   return errorstatus;                     //Response error
            response = SDIO->CMD_BUF3 << 24 | SDIO->CMD_BUF2 << 16 | SDIO->CMD_BUF1 << 8 | SDIO->CMD_BUF0;
            validvoltage = (((response >> 31) == 1) ? 1 : 0);                   //Determine whether the SD card is powered on
            count++;
            if (timeout-- <= 0)
                return SD_CMD_RSP_TIMEOUT;
        }
        if (count >= SD_MAX_VOLT_TRIAL) {
            errorstatus = SD_INVALID_VOLTRANGE;
            return errorstatus;
        }
        if (response &= SD_HIGH_CAPACITY)
            CardType = SDIO_HIGH_CAPACITY_SD_CARD;
    }
    else {
        u32 timeout = SDIO_CMD0TIMEOUT;
        /* MMC card, send CMD1 SDIO_SEND_OP_COND with the parameter 0x80FF8000 */
        while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL)) {
            if (timeout-- <= 0) {
                return SD_CMD_RSP_TIMEOUT;
            }
            SDIO_Send_Cmd(SD_CMD_SEND_OP_COND, SDIO_Response_Short, SD_VOLTAGE_WINDOW_MMC); //Send CMD1, short response
            errorstatus = CmdResp3Error();                                      //Waiting for R3 response
            if (errorstatus != SD_OK)   return errorstatus;                     //Response error
            response = SDIO->CMD_BUF3 << 24 | SDIO->CMD_BUF2 << 16 | SDIO->CMD_BUF1 << 8 | SDIO->CMD_BUF0;   //Get the response
            validvoltage = (((response >> 31) == 1) ? 1 : 0);
            count++;
        }
        if (count >= SD_MAX_VOLT_TRIAL) {
            errorstatus = SD_INVALID_VOLTRANGE;
            return errorstatus;
        }
        CardType = SDIO_MULTIMEDIA_CARD;
    }
    return errorstatus;
}

////////////////////////////////////////////////////////////////////////////////
SD_Error SD_InitializeCards(void)
{
    SD_Error errorstatus = SD_OK;
    u16 rca = 0x01;
    if (((SDIO->MMC_CARDSEL) & (SDIO_MMC_CARDSEL_ENPCLK | SDIO_MMC_CARDSEL_CTREN)) == 0) {
        return SD_REQUEST_NOT_APPLICABLE;                                       //Check the power status to make sure it is on
    }
    if (SDIO_SECURE_DIGITAL_IO_CARD != CardType) {                              //The SECURE_DIGITAL_IO_CARD
        SDIO_Send_Cmd(SD_CMD_ALL_SEND_CID, SDIO_Response_Long, 0);              //Send CMD2, get CID, long response
        errorstatus = CmdResp2Error();
        if (errorstatus != SD_OK) {
            return errorstatus;
        }
        CID_Tab[3] = SDIO->CMD_BUF2  << 24 | SDIO->CMD_BUF1  << 16 | SDIO->CMD_BUF0  << 8 | SDIO->CMD_BUF0;//The low 8 bits are CRC and reserved bits
        CID_Tab[2] = SDIO->CMD_BUF6  << 24 | SDIO->CMD_BUF5  << 16 | SDIO->CMD_BUF4  << 8 | SDIO->CMD_BUF3;
        CID_Tab[1] = SDIO->CMD_BUF10 << 24 | SDIO->CMD_BUF9  << 16 | SDIO->CMD_BUF8  << 8 | SDIO->CMD_BUF7;
        CID_Tab[0] = SDIO->CMD_BUF14 << 24 | SDIO->CMD_BUF13 << 16 | SDIO->CMD_BUF12 << 8 | SDIO->CMD_BUF11;
    }
    if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) { //Judgment card type
        SDIO_Send_Cmd(SD_CMD_SET_REL_ADDR, SDIO_Response_Short, 0);
        errorstatus = CmdResp6Error(SD_CMD_SET_REL_ADDR, &rca);
        if (errorstatus != SD_OK) {
            return errorstatus;
        }
    }
    if (SDIO_MULTIMEDIA_CARD == CardType) {
        SDIO_Send_Cmd(SD_CMD_SET_REL_ADDR, SDIO_Response_Short, (u32)(rca << 16));
        errorstatus = CmdResp2Error();
        if (errorstatus != SD_OK) {
            return errorstatus;
        }
    }
    if (SDIO_SECURE_DIGITAL_IO_CARD != CardType) {                              //not SECURE_DIGITAL_IO_CARD
        RCA = rca;
        SDIO_Send_Cmd(SD_CMD_SEND_CSD, SDIO_Response_Long, (u32)(rca << 16));   //Send CMD9+ card RCA, get CSD, long response
        errorstatus = CmdResp2Error();
        if (errorstatus != SD_OK) {
            return errorstatus;
        }
        CSD_Tab[3] = SDIO->CMD_BUF2 << 24  | SDIO->CMD_BUF1 << 16  | SDIO->CMD_BUF0 << 8  | SDIO->CMD_BUF0;
        CSD_Tab[2] = SDIO->CMD_BUF6 << 24  | SDIO->CMD_BUF5 << 16  | SDIO->CMD_BUF4 << 8  | SDIO->CMD_BUF3;
        CSD_Tab[1] = SDIO->CMD_BUF10 << 24 | SDIO->CMD_BUF9 << 16 | SDIO->CMD_BUF8 << 8  | SDIO->CMD_BUF7;
        CSD_Tab[0] = SDIO->CMD_BUF14 << 24 | SDIO->CMD_BUF13 << 16 | SDIO->CMD_BUF12 << 8 | SDIO->CMD_BUF11;
    }
    return SD_OK;//Card initialization succeeded
}

////////////////////////////////////////////////////////////////////////////////
SD_Error SD_Init(void)
{
    u8 clkdiv = 0;
    u32 clk, targetFreq;
    RCC_ClocksTypeDef bclk;
    SD_Error errorstatus = SD_NoInsert;
    
    //INTX_DISABLE();
    errorstatus = SD_PowerON();                 //SD Power On
    if (errorstatus != SD_OK)   return errorstatus;
    errorstatus = SD_InitializeCards();         //Initialize SD Card
    if (errorstatus != SD_OK)   return errorstatus;
    errorstatus = SD_GetCardInfo(&SDCardInfo);  //Get card information
    if (errorstatus != SD_OK)   return errorstatus;
    errorstatus = SD_SelectDeselect((u32)(SDCardInfo.RCA << 16)); //Select the SD card
    if (errorstatus != SD_OK)   return errorstatus;
    errorstatus = SD_EnableWideBusOperation(1); //4 bit width, if it is an MMC card, you cannot use 4 bit mode
    if ((errorstatus != SD_OK)) {
        if ((SDIO_MULTIMEDIA_CARD == CardType)) {
            /* V1.1/V2.0 card with the maximum setting of 48/4=12Mhz; For other cards such as SDHC, the maximum setting is 48/2=24Mhz */
            clkdiv = (SDCardInfo.CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1 || SDCardInfo.CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0) ? 0 : 1;
            targetFreq = (clkdiv != 0) ? 24000000 : 12000000;
            RCC_GetClocksFreq(&bclk);
            clk = (bclk.HCLK_Frequency / 2 / 2 / targetFreq - 1);
            SDIO_ClockSet(clk);
        }
        else
            __NOP();
    }
    else {
        clkdiv = (SDCardInfo.CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1 || SDCardInfo.CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0) ? 0 : 1;
        targetFreq = (clkdiv != 0) ? 24000000 : 12000000;
        RCC_GetClocksFreq(&bclk);
        clk = (bclk.HCLK_Frequency / 2 / 2 / targetFreq - 1);
        SDIO_ClockSet(clk);
    }
    INTX_ENABLE();
    return errorstatus;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Data width and mode configuration.
/// @param  enx : width.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
SD_Error SDEnWideBus(u8 enx)
{
    SD_Error errorstatus = SD_OK;
    u32 response;
    u8 arg = 0X00;
    if (enx)arg = 0X02;
    else arg = 0X00;
    response = SDIO->CMD_BUF3 << 24 | SDIO->CMD_BUF2 << 16 | SDIO->CMD_BUF1 << 8 | SDIO->CMD_BUF0;
    if (response & SD_CARD_LOCKED)return SD_LOCK_UNLOCK_FAILED;
    if (errorstatus != SD_OK)return errorstatus;
    {
        SDIO_Send_Cmd(SD_CMD_APP_CMD, SDIO_Response_Short, (u32)RCA << 16);
        errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
        if (errorstatus != SD_OK)return errorstatus;
        SDIO_Send_Cmd(SD_CMD_APP_SD_SET_BUSWIDTH, 1, arg);
        errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);
        return errorstatus;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Data width and mode configuration.
/// @param  wmode : width.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
SD_Error SD_EnableWideBusOperation(u32 wmode)
{
    SD_Error errorstatus = SD_OK;
    u16 clkcr = 0;
    if (SDIO_MULTIMEDIA_CARD == CardType)
        return SD_UNSUPPORTED_FEATURE;
    else
        if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
            if (wmode >= 2)
                return SD_UNSUPPORTED_FEATURE;
            else {
                errorstatus = SDEnWideBus(wmode);
                if (SD_OK == errorstatus) {
                    clkcr = SDIO->MMC_CTRL;
                    clkcr &= ~(SDIO_MMC_CTRL_DATWT);
                    clkcr |= (u16)wmode << SDIO_MMC_CTRL_DATWT_Pos;
                    SDIO->MMC_CTRL = (clkcr & (~(0x07 << 3)));
                }
            }
        }
    return errorstatus;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get card information
/// @param  cardinfo: Card information storage area.
/// @retval errorstatus: error statue.
////////////////////////////////////////////////////////////////////////////////
SD_Error SD_GetCardInfo(SD_CardInfo* cardinfo)
{
    SD_Error errorstatus = SD_OK;
    u8 tmp = 0;
    cardinfo->CardType = (u8)CardType;
    cardinfo->RCA = (u16)RCA;
    tmp = (u8)((CSD_Tab[0] & 0xFF000000) >> 24);
    cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
    cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;                        //This part of the 2.0 protocol has not been defined (for reservation) and should be defined by subsequent protocols
    cardinfo->SD_csd.Reserved1 = tmp & 0x03;                                    //Two reserved bits
    tmp = (u8)((CSD_Tab[0] & 0x00FF0000) >> 16);                                //The first byte
    cardinfo->SD_csd.TAAC = tmp;                                                //Data read time 1
    tmp = (u8)((CSD_Tab[0] & 0x0000FF00) >> 8);                                 //Second byte
    cardinfo->SD_csd.NSAC = tmp;                                                //Data read time 2
    tmp = (u8)(CSD_Tab[0] & 0x000000FF);                                        //The third byte
    cardinfo->SD_csd.MaxBusClkFrec = tmp;                                       //Transmission speed
    tmp = (u8)((CSD_Tab[1] & 0xFF000000) >> 24);                                //The fourth byte
    cardinfo->SD_csd.CardComdClasses = tmp << 4;                                //The card instruction class is four bits high
    tmp = (u8)((CSD_Tab[1] & 0x00FF0000) >> 16);                                //The fifth byte
    cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;                      //The card instruction class is four digits low
    cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;                                   //Maximum read data length
    tmp = (u8)((CSD_Tab[1] & 0x0000FF00) >> 8);                                 //The sixth byte
    cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;                         //Block reading is allowed
    cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;                       //Written piece of dislocation
    cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;                       //Read piece of dislocation
    cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
    cardinfo->SD_csd.Reserved2 = 0;                                             //reserved
    if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || \
            (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0) || \
            (SDIO_MULTIMEDIA_CARD == CardType)) {                                   // Standard 1.1/2.0 card /MMC card
        cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;                       //C_SIZE(12)
        tmp = (u8)(CSD_Tab[1] & 0x000000FF);                                    //The seventh byte
        cardinfo->SD_csd.DeviceSize |= (tmp) << 2;
        tmp = (u8)((CSD_Tab[2] & 0xFF000000) >> 24);                            //The eighth byte
        cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;
        cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
        cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);
        tmp = (u8)((CSD_Tab[2] & 0x00FF0000) >> 16);                            //The ninth byte
        cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
        cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
        cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;                     //C_SIZE_MULT
        tmp = (u8)((CSD_Tab[2] & 0x0000FF00) >> 8);                             //The tenth byte
        cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
        cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1);             //Calculated card capacity
        cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
        cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);           //block size
        cardinfo->CardCapacity *= cardinfo->CardBlockSize;
    }
    else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {                          //High-capacity card
        tmp = (u8)(CSD_Tab[1] & 0x000000FF);    //
        cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;                       //C_SIZE
        tmp = (u8)((CSD_Tab[2] & 0xFF000000) >> 24);
        cardinfo->SD_csd.DeviceSize |= (tmp << 8);
        tmp = (u8)((CSD_Tab[2] & 0x00FF0000) >> 16);
        cardinfo->SD_csd.DeviceSize |= (tmp);
        tmp = (u8)((CSD_Tab[2] & 0x0000FF00) >> 8);
        cardinfo->CardCapacity = (long long)(cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
        cardinfo->CardBlockSize = 512;                                          //The block size is fixed at 512 bytes
    }
    cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;
    tmp = (u8)(CSD_Tab[2] & 0x000000FF);                                        //The eleventh byte
    cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
    cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);
    tmp = (u8)((CSD_Tab[3] & 0xFF000000) >> 24);                                //The twelfth byte
    cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
    cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;
    tmp = (u8)((CSD_Tab[3] & 0x00FF0000) >> 16);                                //The thirteenth byte
    cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
    cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.Reserved3 = 0;
    cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);
    tmp = (u8)((CSD_Tab[3] & 0x0000FF00) >> 8);                                 //The fourteenth byte
    cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
    cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
    cardinfo->SD_csd.ECC = (tmp & 0x03);
    tmp = (u8)(CSD_Tab[3] & 0x000000FF);                                        //The fifteenth byte
    cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
    cardinfo->SD_csd.Reserved4 = 1;
    tmp = (u8)((CID_Tab[0] & 0xFF000000) >> 24);                                //The zero byte
    cardinfo->SD_cid.ManufacturerID = tmp;
    tmp = (u8)((CID_Tab[0] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.OEM_AppliID = tmp << 8;
    tmp = (u8)((CID_Tab[0] & 0x000000FF00) >> 8);
    cardinfo->SD_cid.OEM_AppliID |= tmp;
    tmp = (u8)(CID_Tab[0] & 0x000000FF);
    cardinfo->SD_cid.ProdName1 = tmp << 24;
    tmp = (u8)((CID_Tab[1] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdName1 |= tmp << 16;
    tmp = (u8)((CID_Tab[1] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.ProdName1 |= tmp << 8;
    tmp = (u8)((CID_Tab[1] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ProdName1 |= tmp;
    tmp = (u8)(CID_Tab[1] & 0x000000FF);
    cardinfo->SD_cid.ProdName2 = tmp;
    tmp = (u8)((CID_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdRev = tmp;
    tmp = (u8)((CID_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.ProdSN = tmp << 24;
    tmp = (u8)((CID_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ProdSN |= tmp << 16;
    tmp = (u8)(CID_Tab[2] & 0x000000FF);
    cardinfo->SD_cid.ProdSN |= tmp << 8;
    tmp = (u8)((CID_Tab[3] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdSN |= tmp;
    tmp = (u8)((CID_Tab[3] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
    cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;
    tmp = (u8)((CID_Tab[3] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ManufactDate |= tmp;
    tmp = (u8)(CID_Tab[3] & 0x000000FF);
    cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
    cardinfo->SD_cid.Reserved2 = 1;
    return errorstatus;
}

////////////////////////////////////////////////////////////////////////////////
SD_Error SD_SelectDeselect(u32 addr)
{
    SDIO_Send_Cmd(SD_CMD_SEL_DESEL_CARD, SDIO_Response_Short, addr);            //Send CMD7, select card, short response
    return CmdResp1Error(SD_CMD_SEL_DESEL_CARD);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   Read SD card
/// @param   buf: read data buffer
/// @param   sector: sector address
/// @param   cnt: number of sectors
/// @retval  error status; 0, normal; other, error code;
////////////////////////////////////////////////////////////////////////////////
u8 SD_ReadDisk(u8* buf, u32 sector, u32 cnt)
{
    u8 sta = SD_OK;
    long long lsector = sector;
    u32 n;
    if (CardType != SDIO_STD_CAPACITY_SD_CARD_V1_1)lsector <<= 9;
    if ((u32)buf % 4 != 0) {
        for (n = 0; n < cnt; n++) {
            sta = SD_ReadBlocks(SDIO_DATA_BUFFER, lsector + 512 * n, 512, 1);
            memcpy(buf, SDIO_DATA_BUFFER, 512);
            buf += 512;
        }
    }
    else {
        if(cnt == 1)sta = SD_ReadBlocks(buf, lsector, 512, 1);
        else sta = SD_ReadMulBlocks(buf, lsector, 512, cnt);
    }
    return sta;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   Write to SD card
/// @param   buf: write data buffer area
/// @param   sector: sector address
/// @param   cnt: number of sectors
/// @retval  error status; 0, normal; other, error code;
////////////////////////////////////////////////////////////////////////////////
u8 SD_WriteDisk(u8* buf, u32 sector, u32 cnt)
{
    u8 sta = SD_OK;
    u32 n;
    long long lsector = sector;
    if (CardType != SDIO_STD_CAPACITY_SD_CARD_V1_1)lsector <<= 9;
    if ((u32)buf % 4 != 0) {
        for (n = 0; n < cnt; n++) {
            memcpy(SDIO_DATA_BUFFER, buf, 512);
            sta = SD_WriteBlocks(SDIO_DATA_BUFFER, lsector + 512 * n, 512);
            buf += 512;
        }
    }
    else {
        if(cnt == 1)sta = SD_WriteBlocks(buf, lsector, 512);
        sta = SD_WriteMulBlocks(buf, lsector, 512, cnt);
    }
    return sta;
}

////////////////////////////////////////////////////////////////////////////////
void CMD_Send(unsigned int cmd, unsigned int arg)
{
    SDIO->CMD_BUF4 = 0x40 | cmd;
    SDIO->CMD_BUF3 = ((arg & 0xff000000) >> 24);
    SDIO->CMD_BUF2 = ((arg & 0xff0000) >> 16);
    SDIO->CMD_BUF1 = ((arg & 0xff00) >> 8);
    SDIO->CMD_BUF0 = (arg & 0xff);

    SDIO->MMC_IO |= 0x04;                                                       //auto only command transfer
    while(1) {
        if(SDIO->CLR_MMC_INT & 0x1) {                                           //judge which interrupt generation
            SDIO->CLR_MMC_INT = 0x1;                                            //write 1 clear interrup
            break;
        }
    }
}
SD_Error SD_ReadMulBlocks(u8* buf, long long addr, u16 blksize, u32 nblks)
{
    SD_Error errorstatus = SD_OK;
    u32 timeout = SDIO_DATATIMEOUT;
    u32* tempbuff = (u32*)buf;
    ob_cnt = 0;
    SDIO->BUF_CTL &= ~(SDIO_BUF_CTLL_DRM);
    if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
        blksize = 512;
        addr >>= 9;
    }
    CMD_Send(16, blksize);
    SDIO->MMC_IO = 0x0c;  //auto only response transfer (48bit)
    while(1) {
        if(SDIO->CLR_MMC_INT & 0x1) {            //judge which interrupt generation
            SDIO->CLR_MMC_INT = 0x1;   //write 1 clear interrup
            break;
        }
    }
    CMD_Send(18, addr); //18
    SDIO->MMC_IO = 0xcc;  //auto only response transfer (48bit)
    SDIO->MMC_BLOCKCNT          = nblks;
    SDIO->MMC_IO_MBCTL          |= 0x7;
    SDIO->MMC_TIMEOUTCNT          = 0xff;//
    while(1) {
        if(SDIO->CLR_MMC_INT & 0x1) {            //judge which interrupt generation
            SDIO->CLR_MMC_INT = 0x1;   //write 1 clear interrup
            break;
        }
    }
    if(DeviceMode == SD_POLLING_MODE) { //
        INTX_DISABLE();
        while(1) {
#define USE_BUFFER_FULL//USE_BUFFER_EMPTY//
#if defined(USE_BUFFER_EMPTY)
            if(!(SDIO->BUF_CTL & 0X02)) {
                *(vu32*)tempbuff = SDIO->DATA_BUF0;
                tempbuff++;
                ob_cnt++;
                timeout = 0X7FFFFF;
            }
            if(ob_cnt >= ((nblks * blksize) / 4)) {
                if(SDIO->CLR_MMC_INT & (0X01 << 4))
                    break;
            }
#endif
#if defined(USE_BUFFER_FULL)
            if((SDIO->BUF_CTL & 0X01)) {
                for(int j = 0; j < 128; j++) {
                    *(vu32*)tempbuff++ = SDIO->DATA_BUF0;
                }
                ob_cnt = ob_cnt + 128;
                timeout = 0X7FFFFF;
            }
            if(ob_cnt >= ((nblks * blksize) / 4)) {
                if(SDIO->CLR_MMC_INT & (0X01 << 4))
                    break;
            }
#endif
            if(timeout == 0) {
                return SD_DATA_TIMEOUT;
            }
        }
        if(nblks > 1) {
            if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
                SDIO_Send_Cmd(SD_CMD_STOP_TRANSMISSION, 1, 0);
                errorstatus = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
                if(errorstatus != SD_OK)return errorstatus;
            }
        }
        INTX_ENABLE();
        SDIO->CLR_MMC_INT = SDIO_CLR_MMC_INT_MASK;
    }
    else if(DeviceMode == SD_DMA_MODE) {
        TransferError = SD_OK;
        if(nblks > 1)StopCondition = 1;
        else StopCondition = 0;
        TransferEnd = 0;
        SDIO->BUF_CTL |= (1 << 14);
        SD_DMA_Config((u32*)buf, nblks * blksize, 0);
        if(timeout == 0)return SD_DATA_TIMEOUT;
        while((TransferEnd == 0) && (TransferError == SD_OK));
        if(TransferError != SD_OK)errorstatus = TransferError;
    }
    return errorstatus;
}

SD_Error SD_ReadBlocks(u8* buf, long long addr, u16 blksize, u32 nblks)
{
    SD_Error errorstatus = SD_OK;


    u32 timeout = SDIO_DATATIMEOUT;
    u32* tempbuff = (u32*)buf;
    ob_cnt = 0;
    SDIO->BUF_CTL &= ~(SDIO_BUF_CTLL_DRM);
    if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
        blksize = 512;
        addr >>= 9;
    }
    SDIO_Send_Cmd(SD_CMD_SET_BLOCKLEN, SDIO_Response_Short, blksize);
    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
    if(errorstatus != SD_OK)return errorstatus;
    SDIO_Send_Data_Cfg(SD_DATATIMEOUT, nblks * blksize, blksize, 1);
    if(nblks > 1) {
        SDIO_Send_Cmd(SD_CMD_READ_MULT_BLOCK, SDIO_Response_Short, addr);
        errorstatus = CmdResp1Error(SD_CMD_READ_MULT_BLOCK);
        if(errorstatus != SD_OK)return errorstatus;
    }
    else {
        SDIO_Send_Cmd(SD_CMD_READ_SINGLE_BLOCK, SDIO_Response_Short, addr);
        errorstatus = CmdResp1Error(SD_CMD_READ_SINGLE_BLOCK);
        if(errorstatus != SD_OK)return errorstatus;
    }
    if(DeviceMode == SD_POLLING_MODE) { //
        INTX_DISABLE();
        SDIO->MMC_BLOCKCNT = nblks;
        SDIO->MMC_IO          = SDIO_MMC_IO_MBCTL_SMBDTD;
        ob_cnt = 0;
        SDIO->MMC_IO          |= SDIO_MMC_IO_MBCTL_SPMBDTR;
        while(!(SDIO->CLR_MMC_INT & (SDIO_CLR_MMC_INT_DATEMC | SDIO_CLR_MMC_INT_CRNTMC | SDIO_CLR_MMC_INT_DATDMC))) {
            if(!(SDIO->BUF_CTL & SDIO_BUF_CTLL_DBE)) {
                *(vu32*)tempbuff = SDIO->DATA_BUF0;
                tempbuff++;
                ob_cnt++;
                timeout = 0X7FFFFF;
            }
            if(timeout == 0) {
                return SD_DATA_TIMEOUT;
            }
            timeout--;
        }
        if(SDIO->CLR_MMC_INT & SDIO_CLR_MMC_INT_CRNTMC) {
            SDIO->CLR_MMC_INT = SDIO_CLR_MMC_INT_CRNTMC;
            return SD_DATA_TIMEOUT;
        }
        else if(SDIO->CLR_MMC_INT & SDIO_CLR_MMC_INT_DATEMC) {
            SDIO->CLR_MMC_INT = SDIO_CLR_MMC_INT_DATEMC;
            return SD_DATA_CRC_FAIL;
        }
        while(!(SDIO->BUF_CTL & SDIO_CLR_MMC_INT_DATDMC)) {
            *(vu32*)tempbuff = SDIO->DATA_BUF0;
            tempbuff++;
            ob_cnt++;
        }
        if(nblks > 1) {
            if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
                SDIO_Send_Cmd(SD_CMD_STOP_TRANSMISSION, 1, 0);
                errorstatus = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
                if(errorstatus != SD_OK)return errorstatus;
            }
        }
        INTX_ENABLE();
        SDIO->CLR_MMC_INT = SDIO_CLR_MMC_INT_MASK;
    }
    else if(DeviceMode == SD_DMA_MODE) {
        TransferError = SD_OK;
        if(nblks > 1)StopCondition = 1;
        else StopCondition = 0;
        TransferEnd = 0;
        SDIO->BUF_CTL |= (1 << 14);
        SD_DMA_Config((u32*)buf, nblks * blksize, 0);
        if(timeout == 0)return SD_DATA_TIMEOUT;
        while((TransferEnd == 0) && (TransferError == SD_OK));
        if(TransferError != SD_OK)errorstatus = TransferError;
    }
    return errorstatus;
}

SD_Error SD_WriteBlocks(u8* buf, long long addr, u16 blksize)
{
    SD_Error errorstatus = SD_OK;
    u8  cardstate = 0;
    u32 timeout = 0, bytestransferred = 0;
    u32 cardstatus = 0;
    u32 tlen = blksize;
    u32* tempbuff = (u32*)buf;
    u32 i;
    if (buf == NULL)return SD_INVALID_PARAMETER;
    SDIO->BUF_CTL &= ~(1 << 14);
    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
        blksize = 512;
        addr >>= 9;
    }
    SDIO_Send_Cmd(SD_CMD_SET_BLOCKLEN, SDIO_Response_Short, blksize);
    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
    if (errorstatus != SD_OK)return errorstatus;

    SDIO_Send_Cmd(SD_CMD_SEND_STATUS, SDIO_Response_Short, (u32)RCA << 16);
    errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);
    if (errorstatus != SD_OK)return errorstatus;
    cardstatus = SDIO->CMD_BUF3 << 24 | SDIO->CMD_BUF2 << 16 | SDIO->CMD_BUF1 << 8 | SDIO->CMD_BUF0;
    timeout = SD_DATATIMEOUT;
    while (((cardstatus & 0x00000100) == 0) && (timeout > 0)) {
        timeout--;
        SDIO_Send_Cmd(SD_CMD_SEND_STATUS, SDIO_Response_Short, (u32)RCA << 16);
        errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);
        if (errorstatus != SD_OK)return errorstatus;
        cardstatus = SDIO->CMD_BUF3 << 24 | SDIO->CMD_BUF2 << 16 | SDIO->CMD_BUF1 << 8 | SDIO->CMD_BUF0;
    }
    if (timeout == 0)return SD_ERROR;
    SDIO_Send_Cmd(SD_CMD_WRITE_SINGLE_BLOCK, SDIO_Response_Short, addr);
    errorstatus = CmdResp1Error(SD_CMD_WRITE_SINGLE_BLOCK);

    if (errorstatus != SD_OK)return errorstatus;
    SDIO_Send_Data_Cfg(SD_DATATIMEOUT, 1 * blksize, 9, 0);
    timeout = SDIO_DATATIMEOUT;
    if (DeviceMode == SD_POLLING_MODE) {
        INTX_DISABLE();
        timeout = 0X7FFFFF;
        SDIO->MMC_IO          = 0x00;
        SDIO->BUF_CTL         = SDIO_BUF_CTLL_SBAD | SDIO_BUF_CTLL_DBFEN;
        SDIO->MMC_BLOCKCNT = 1;
        bytestransferred = 0;
        while (!(SDIO->CLR_MMC_INT & ((1 << 2) | (1 << 6) | (1 << 1)))) {
            if (!(SDIO->BUF_CTL & (1 << 0))) {                  //send buffer full
                if ((tlen - bytestransferred) > 0) {
                    SDIO->DATA_BUF0 = *(vu32*)tempbuff++;
                    bytestransferred += 4;
                }
                if (tlen == bytestransferred) {
                    SDIO->MMC_IO   = 0x1; //2019  SDIO->MMC_IO   = 0x1;
                    break;
                }
                timeout = 0X3FFFFFFF;
            }
            else {
                if (timeout == 0)return SD_DATA_TIMEOUT;
                timeout--;
            }
        }
        if (SDIO->CLR_MMC_INT & (1 << 6)) {
            SDIO->CLR_MMC_INT = 1 << 6;
            return SD_DATA_TIMEOUT;
        }
        else if (SDIO->CLR_MMC_INT & (1 << 2)) {
            SDIO->CLR_MMC_INT = 1 << 2;
            return SD_DATA_CRC_FAIL;
        }
        if (SDIO->CLR_MMC_INT & (1 << 6)) {
            SDIO->CLR_MMC_INT = 1 << 6;
            return SD_DATA_TIMEOUT;
        }
        else if (SDIO->CLR_MMC_INT & (1 << 2)) {
            SDIO->CLR_MMC_INT = 1 << 2;
            return SD_DATA_CRC_FAIL;
        }
        while (!(SDIO->BUF_CTL & (1 << 1)));
        SDIO->CLR_MMC_INT |= 0;
        i = 100;
        while (i--);
        SDIO->BUF_CTL         = 0x8800;    //disable dma, write sd card

        INTX_ENABLE();
        SDIO->CLR_MMC_INT = 0xFF;
    }
    else if(DeviceMode == SD_DMA_MODE) {

    }
    SDIO->CLR_MMC_INT = 0xFF;
    errorstatus = IsCardProgramming(&cardstate);
    while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING))) {
        errorstatus = IsCardProgramming(&cardstate);
    }
    return errorstatus;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The SD card reads a oneor more block
/// @param  buf:Read the data cache,addr:read address,blksize: block size,
///         nblks: The number of blocks to read,1: read one block
/// @retval errorstatus: error statue.
////////////////////////////////////////////////////////////////////////////////
SD_Error SD_WriteMulBlocks(u8* buf, long long addr, u16 blksize, u32 nblks)
{
    SD_Error errorstatus = SD_OK;
    u8  cardstate = 0;
    u32 timeout = 0, bytestransferred = 0;
    u32 tlen = nblks * blksize;
    u32* tempbuff = (u32*)buf;
    if (buf == NULL)return SD_INVALID_PARAMETER;
    SDIO->BUF_CTL &= ~(1 << 14);
    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
        blksize = 512;
        addr >>= 9;
    }
    SDIO_Send_Cmd(SD_CMD_SET_BLOCKLEN, SDIO_Response_Short, blksize);
    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
    if (errorstatus != SD_OK)return errorstatus;
    if (nblks > 1) {
        if (nblks * blksize > SD_MAX_DATA_LENGTH)return SD_INVALID_PARAMETER;
        if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {

            SDIO_Send_Cmd(SD_CMD_APP_CMD, SDIO_Response_Short, (u32)RCA << 16);
            errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
            if (errorstatus != SD_OK)return errorstatus;
            SDIO_Send_Cmd(SD_CMD_SET_BLOCK_COUNT, SDIO_Response_Short, nblks);
            errorstatus = CmdResp1Error(SD_CMD_SET_BLOCK_COUNT);
            if (errorstatus != SD_OK)return errorstatus;
        }
        SDIO_Send_Cmd(SD_CMD_WRITE_MULT_BLOCK, SDIO_Response_Short, addr);
        errorstatus = CmdResp1Error(SD_CMD_WRITE_MULT_BLOCK);
    }
    if (errorstatus != SD_OK)return errorstatus;
    SDIO_Send_Data_Cfg(SD_DATATIMEOUT, nblks * blksize, 9, 0);
    timeout = SDIO_DATATIMEOUT;
    if (DeviceMode == SD_POLLING_MODE) {
        INTX_DISABLE();
        timeout = 0X7FFFFF;
        SDIO->MMC_IO          = 0x00;
        SDIO->BUF_CTL         = SDIO_BUF_CTLL_SBAD | SDIO_BUF_CTLL_DBFEN;
        SDIO->MMC_BLOCKCNT = nblks;
        bytestransferred = 0;
        while (!(SDIO->CLR_MMC_INT & ((1 << 2) | (1 << 6) | (1 << 1)))) {
            if (!(SDIO->BUF_CTL & (1 << 0))) {                  //send buffer full
                if ((tlen - bytestransferred) > 0) {
                    SDIO->DATA_BUF0 = *(vu32*)tempbuff++;
                    bytestransferred += 4;
                }
                if (tlen == bytestransferred) {
                    SDIO->MMC_IO   = 0x1; //2019  SDIO->MMC_IO   = 0x1;
                    break;
                }
                timeout = 0X3FFFFFFF;
            }
            else {
                if (timeout == 0)return SD_DATA_TIMEOUT;
                timeout--;
            }
        }
        if (SDIO->CLR_MMC_INT & (1 << 6)) {
            SDIO->CLR_MMC_INT = 1 << 6;
            return SD_DATA_TIMEOUT;
        }
        else if (SDIO->CLR_MMC_INT & (1 << 2)) {
            SDIO->CLR_MMC_INT = 1 << 2;
            return SD_DATA_CRC_FAIL;
        }
        if (SDIO->CLR_MMC_INT & (1 << 6)) {
            SDIO->CLR_MMC_INT = 1 << 6;
            return SD_DATA_TIMEOUT;
        }
        else if (SDIO->CLR_MMC_INT & (1 << 2)) {
            SDIO->CLR_MMC_INT = 1 << 2;
            return SD_DATA_CRC_FAIL;
        }
//        while (!(SDIO->BUF_CTL & (1 << 1)));
//        SDIO->CLR_MMC_INT |= 0;
//        i = 100;
//        while (i--);
//        SDIO->BUF_CTL         = 0x8800;    //disable dma, write sd card
        if (nblks > 1) {
            if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
                SDIO_Send_Cmd(SD_CMD_STOP_TRANSMISSION, SDIO_Response_Short, 0);
                errorstatus = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
                if (errorstatus != SD_OK)return errorstatus;
            }
        }
        INTX_ENABLE();
        SDIO->CLR_MMC_INT = 0xFF;
    }
    else if(DeviceMode == SD_DMA_MODE) {

    }
    SDIO->CLR_MMC_INT = 0xFF;
    errorstatus = IsCardProgramming(&cardstate);
    while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING))) {
        errorstatus = IsCardProgramming(&cardstate);
    }
    return errorstatus;
}

////////////////////////////////////////////////////////////////////////////////
SD_Error SD_ProcessIRQSrc(void)
{
    if (SDIO->CLR_MMC_INT & 0x01) {
        if (StopCondition == 1) {
            SDIO_Send_Cmd(SD_CMD_STOP_TRANSMISSION, SDIO_Response_Short, 0);      //¡¤¡é?¨ªCMD12,?¨¢¨º?¡ä?¨º?
            TransferError = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
        }
        else TransferError = SD_OK;
        SDIO->CLR_MMC_INT = 1 << 0;
        SDIO->MMC_INT_MASK &= ~((1 << 0) | (1 << 1) | (1 << 6) | (1 << 7));
        TransferEnd = 1;
        return (TransferError);
    }
    if (SDIO->CLR_MMC_INT & (1 << 2)) {
        SDIO->CLR_MMC_INT = 1 << 2;
        SDIO->MMC_INT_MASK &= ~((1 << 0) | (1 << 1) | (1 << 6) | (1 << 7));
        TransferError = SD_DATA_CRC_FAIL;
        return (SD_DATA_CRC_FAIL);
    }
    if (SDIO->CLR_MMC_INT & (1 << 6)) {
        SDIO->CLR_MMC_INT = 1 << 6;
        SDIO->MMC_INT_MASK &= ~((1 << 0) | (1 << 1) | (1 << 6) | (1 << 7));
        TransferError = SD_DATA_TIMEOUT;
        return (SD_DATA_TIMEOUT);
    }
    return (SD_OK);
}

////////////////////////////////////////////////////////////////////////////////
SD_Error IsCardProgramming(u8* pstatus)
{
    vu32 respR1 = 0, status = 0;
    while (!(SDIO->BUF_CTL & SDIO_BUF_CTLL_DBE));
    SDIO_Send_Cmd(SD_CMD_SEND_STATUS, SDIO_Response_Short, (u32)RCA << 16);
    status = SDIO->CLR_MMC_INT ;
    while (!(status & (SDIO_CLR_MMC_INT_CRCEMC | SDIO_CLR_MMC_INT_CRNTMC | SDIO_CLR_MMC_INT_CMDDMC)))
        status = SDIO->CLR_MMC_INT ;
    if (status & SDIO_CLR_MMC_INT_CRCEMC) {
        SDIO->CLR_MMC_INT = SDIO_CLR_MMC_INT_CRCEMC;
        return SD_CMD_CRC_FAIL;
    }
    if (status & SDIO_CLR_MMC_INT_CRNTMC) {
        SDIO->CLR_MMC_INT = SDIO_CLR_MMC_INT_CRNTMC;
        return SD_CMD_RSP_TIMEOUT;
    }
    if ((SDIO->CMD_BUF4 & 0x3F) != SD_CMD_SEND_STATUS)return SD_ILLEGAL_CMD;
    SDIO->CLR_MMC_INT = 0xff;
    respR1 = SDIO->CMD_BUF3 << 24 | SDIO->CMD_BUF2 << 16 | SDIO->CMD_BUF1 << 8 | SDIO->CMD_BUF0;
    *pstatus = (u8)((respR1 >> 9) & 0x0000000F);
    return SD_OK;
}

////////////////////////////////////////////////////////////////////////////////
SD_Error SD_SendStatus(uint32_t* pcardstatus)
{
    SD_Error errorstatus = SD_OK;
    if (pcardstatus == NULL) {
        errorstatus = SD_INVALID_PARAMETER;
        return errorstatus;
    }
    SDIO_Send_Cmd(SD_CMD_SEND_STATUS, SDIO_Response_Short, RCA << 16);
    errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);
    if (errorstatus != SD_OK)return errorstatus;
    *pcardstatus =  SDIO->CMD_BUF3 << 24 | SDIO->CMD_BUF2 << 16 | SDIO->CMD_BUF1 << 8 | SDIO->CMD_BUF0;
    return errorstatus;
}

////////////////////////////////////////////////////////////////////////////////
SDCardState SD_GetState(void)
{
    u32 resp1 = 0;
    if (SD_SendStatus(&resp1) != SD_OK)return SD_CARD_ERROR;
    else return (SDCardState)((resp1 >> 9) & 0x0F);
}

////////////////////////////////////////////////////////////////////////////////
void SD_DMA_Config(u32* mbuf, u32 bufsize, u8 dir)
{

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SDIO_IRQHandler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SDIO_IRQHandler(void)
{
    SD_ProcessIRQSrc();
}

////////////////////////////////////////////////////////////////////////////////
void BSP_SDIO_Configure()
{
    SD_Error result;
    
    SDIO_ConfigInit();
    result = SD_Init();
    if(result == SD_OK) {
        
    }

}

/// @}

/// @}

/// @}
