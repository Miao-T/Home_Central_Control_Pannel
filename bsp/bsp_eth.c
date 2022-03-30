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
#define _BSP_ETH_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_eth.h"
#include "hal_nvic.h"

#include "bsp.h"
#include "common.h"
#include "main.h"

#include "bsp_eth.h"
#include "bsp_phy8720.h"

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
void ETH_NVIC_Config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void ETH_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable GPIOs clocks
    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_GPIOB);
    COMMON_EnableIpClock(emCLOCK_GPIOC);
    COMMON_EnableIpClock(emCLOCK_GPIOD);
    COMMON_EnableIpClock(emCLOCK_GPIOE);
    COMMON_EnableIpClock(emCLOCK_EXTI);

    // Configure PA0, PA1, PA2, PA3 and PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_11);                       //ETH_RX_CLK/REGOFF     PA1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_11);                       //ETH_MDIO              PA2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_11);                       //ETH_RX_DV             PA7--PD8

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_11);                      // ETH_TX_EN     PB11
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_11);                      // ETH_TXD0     PB12
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_11);                      // ETH_TXD1        PB13

    // Configure PC1, PC2, PC3, PC4 and PC5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_11);                       // ETH_MDC       PC1
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_11);                       // ETH_RXD0      PC4
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_11);                       // ETH_RXD1      PC5

    SYSCFG->CFGR2 |= 0X01<<20;   //select RMII
}

////////////////////////////////////////////////////////////////////////////////
s8 buffer_send(u8* p, u16 len)
{
    uint8_t *buffer = (uint8_t *)(DMATxDescToSet->BUF1ADDR);
    __IO ETH_DMADESCTypeDef *DmaTxDesc;
    uint32_t framelength = 0;
    uint32_t byteslefttocopy = len;
    uint32_t payloadoffset = 0;
    uint32_t bufferoffset = 0;
    DmaTxDesc = DMATxDescToSet;
    bufferoffset = 0;

    if((DmaTxDesc->CS & ETH_DMA_TDES_OWN))  return -8;
    while((byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE) {
        memcpy((u8*)((u8*)buffer + bufferoffset),(u8*)((u8*)p + payloadoffset),(ETH_TX_BUF_SIZE - bufferoffset));// Copy data to Tx buffer
        DmaTxDesc = (ETH_DMADESCTypeDef *)(DmaTxDesc->BUF2NDADDR);// Point to next descriptor
        if((DmaTxDesc->CS & ETH_DMA_TDES_OWN))  return -8;// Check if the buffer is available
        buffer = (u8*)(DmaTxDesc->BUF1ADDR);
        byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
        framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
    }
    memcpy((u8*)((u8*)buffer + bufferoffset),(u8*)((u8*)p + payloadoffset),byteslefttocopy);// Copy the remaining bytes
    bufferoffset = bufferoffset + byteslefttocopy;
    framelength = framelength + byteslefttocopy;
    ETH_Prepare_Transmit_Descriptors(framelength);// Prepare transmit descriptors to give to DMA
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
uint32_t ETH_PHYLoopBackCmd(uint16_t addr, FunctionalState sta)
{
    uint16_t ret;
    uint16_t temp_val;
    temp_val = ETH_ReadPHYRegister(addr, PHY_BCR);

    sta ? (temp_val |= PHY_Loopback) : (temp_val &= ~PHY_Loopback);

    if(ETH_WritePHYRegister(addr, PHY_BCR, temp_val))
        ret = ETH_SUCCESS;
    else
        ret = ETH_ERROR;
    return (uint32_t)ret;
}

////////////////////////////////////////////////////////////////////////////////
s8 ETH_MAC_DMA_Config()
{
    ETH_StructInit(&ETH_InitStructure);

    COMMON_EnableIpClock(emCLOCK_ETH);
    ETH_DeInit();
    ETH_SoftwareReset();
    
    u16 overTime = 65535;
    while (ETH_GetSoftwareResetStatus()){
        if (overTime-- == 0){
            EthErr = ethRstErr;
            return EthErr;
        }
    };

    exETH_MDCClock();

    // ------------------------   MAC   ----------------------------------------
    ETH_InitStructure.ETH_AutoNegotiation           = ETH_AutoNegotiation_Enable;
    ETH_InitStructure.ETH_Speed                     = ETH_Speed_100M;
    ETH_InitStructure.ETH_Mode                      = ETH_Mode_FullDuplex;
    ETH_InitStructure.ETH_LoopbackMode              = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission         = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_ReceiveAll                = ETH_ReceiveAll_Disable;
    ETH_InitStructure.ETH_PromiscuousMode           = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_ChecksumOffload           = ETH_ChecksumOffload_Disable;
    ETH_InitStructure.ETH_UnicastFramesFilter       = ETH_UnicastFramesFilter_Perfect;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip      = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter     = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_BroadcastFramesReception  = ETH_BroadcastFramesReception_Enable;
    // ------------------------   DMA   ----------------------------------------
    //  When we use the Checksum offload feature, we need to enable the Store and Forward mode:
    // the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
    // if the checksum is OK the DMA can handle the frame otherwise the frame is dropped
    ETH_InitStructure.ETH_ReceiveStoreForward       = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure.ETH_TransmitStoreForward      = ETH_TransmitStoreForward_Enable;
    ETH_InitStructure.ETH_ForwardErrorFrames        = ETH_ForwardErrorFrames_Disable;
    ETH_InitStructure.ETH_SecondFrameOperate        = ETH_SecondFrameOperate_Enable;
    ETH_InitStructure.ETH_AddressAlignedBeats       = ETH_AddressAlignedBeats_Disable;
    ETH_InitStructure.ETH_FixedBurst                = ETH_FixedBurst_Enable;
    ETH_InitStructure.ETH_RxDMABurstLength          = 0;
    ETH_InitStructure.ETH_TxDMABurstLength          = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_DMAArbitration            = ETH_DMAArbitration_RoundRobin_RxTx_1_1;
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    EthStatus = ETH_Init(&ETH_InitStructure);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void BSP_ETH_Configure()
{
    u16 overTime = 65535;
    EthStatus = 0;

    ETH_GPIO_Config();
    if(ETH_MAC_DMA_Config() < 0)
        return;
    BSP_PHY8720_Configure(0);
    
    while (1){
        if (ETH_ReadPHYRegister(0, PHY_BSR) & PHY_Linked_Status){
            EthStatus |= ETH_LINK_FLAG;
            return;
        }
        if (overTime-- == 0){
            EthErr = ethReadPhyErr;
            return;
        }
    };
}

/// @}

/// @}

/// @}
