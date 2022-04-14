////////////////////////////////////////////////////////////////////////////////
/// @file     UID.C
/// @author   D CHEN
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UID EXAMPLE.
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
#define _I2C_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "main.h" 
#include "hal_i2c.h"
#include "hal_gpio.h"	
#include "hal_nvic.h"
#include "hal_rcc.h"

#include "main.h"
#include "i2c.h"
#include "uart.h"

volatile u8 gRxFlag = 0;
volatile u8 gTxFlag = 0;
volatile u8 gAbrtFlag = 0;
u8 gData;

////////////////////////////////////////////////////////////////////////////////
void initGPIO_I2C(I2C_TypeDef *I2Cx)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;

    if (I2Cx == I2C1) {
        // GPIOB clock enable
        COMMON_EnableIpClock(emCLOCK_GPIOB);

        // I2C1 SCL PB6
        GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;
        GPIO_Init(GPIOB, &GPIO_InitStruct);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_4);

        // I2C1 SDA PB7
        GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;
        GPIO_Init(GPIOB, &GPIO_InitStruct);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_4);
    } else if (I2Cx == I2C2) {
        // GPIOE clock enable
        COMMON_EnableIpClock(emCLOCK_GPIOE);

        // I2C2 SCL PE5
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_4);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_4);
        
        GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5;
        GPIO_Init(GPIOE, &GPIO_InitStruct);
        
        // I2C2 SDA PE6
        GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;
        GPIO_Init(GPIOE, &GPIO_InitStruct);
    }
}

////////////////////////////////////////////////////////////////////////////////
void NVIC_I2C(I2C_TypeDef* I2Cx)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
  
    if(I2Cx == I2C1) {
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;                         // I2C1_EV_IRQHandler
    } else if (I2Cx == I2C2){
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_IRQn;                         // I2C2_EV_IRQHandler
    }

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void I2CInit_Master(I2C_TypeDef *I2Cx, u32 uiI2C_speed)
{
    I2C_InitTypeDef I2C_InitStruct;

    if(I2Cx == I2C1) {
        COMMON_EnableIpClock(emCLOCK_I2C1);
    } else if (I2Cx == I2C2){
        COMMON_EnableIpClock(emCLOCK_I2C2);
    }

    // Configure I2C as master mode
    I2C_InitStruct.Mode = I2C_CR_MASTER;
    I2C_InitStruct.OwnAddress = 0;
    I2C_InitStruct.Speed = I2C_CR_FAST;
    I2C_InitStruct.ClockSpeed = uiI2C_speed;
    I2C_Init(I2Cx, &I2C_InitStruct);

    I2C_Cmd(I2Cx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
void BSP_I2C_Configure()
{
    initGPIO_I2C(I2C2);
    I2CInit_Master(I2C2, 100000);
    NVIC_I2C(I2C2);
    I2C_ITConfig(I2C2, I2C_IT_RX_FULL | I2C_IT_TX_ABRT, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
void Sensor_WriteByte(I2C_TypeDef *I2Cx, u8 dat)
{
    I2C_SendData(I2Cx, dat);
    while(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE) == 0);
}

////////////////////////////////////////////////////////////////////////////////
static u8 Sensor_ReadByte(I2C_TypeDef *I2Cx)
{
    u8 temp;
    gRxFlag = 0;
    I2C_ReadCmd(I2Cx);
    while(gRxFlag == 0);
    temp = gData;
    return temp;
}

////////////////////////////////////////////////////////////////////////////////
void Sensor_WriteBuffer(I2C_TypeDef *I2Cx, u8* ptr, u16 cnt)
{
    while(cnt--){
        Sensor_WriteByte(I2Cx, *ptr);
        ptr++;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Sensor_ReadBuffer(I2C_TypeDef *I2Cx, u8* ptr, u16 cnt)
{
    u8 i;
    for (i = 0; i < cnt; i++){
        ptr[i] = Sensor_ReadByte(I2Cx);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Sensor_Write(I2C_TypeDef *I2Cx, u8 addr, u8 subAddr, u8* ptr, u16 cnt)
{
    I2C_Cmd(I2Cx, DISABLE);
    I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter);
    I2C_Cmd(I2Cx, ENABLE);

    Sensor_WriteByte(I2Cx, subAddr);
    Sensor_WriteBuffer(I2Cx, ptr, cnt);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET)) == 0);
}

////////////////////////////////////////////////////////////////////////////////
void Sensor_Read(I2C_TypeDef *I2Cx, u8 addr, u8 subAddr, u8* ptr, u16 cnt)
{
    I2C_Cmd(I2Cx, DISABLE);
    I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Receiver);
    I2C_Cmd(I2Cx, ENABLE);

    Sensor_WriteByte(I2Cx, subAddr);
    Sensor_ReadBuffer(I2Cx, ptr, cnt);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET)) == 0);
}

////////////////////////////////////////////////////////////////////////////////
void I2C1_IRQHandler()
{
    if(I2C_GetITStatus(I2C1, I2C_IT_TX_EMPTY)) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_TX_EMPTY);
        I2C_ITConfig(I2C1, I2C_IT_TX_EMPTY, DISABLE);
        gTxFlag = 1;
    }
    if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL)) {
        gData = I2C_ReceiveData(I2C1);
        I2C_ClearITPendingBit(I2C1, I2C_IT_RX_FULL);
        gRxFlag = 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
void I2C2_IRQHandler()
{
    // if(I2C_GetITStatus(I2C2, I2C_IT_TX_EMPTY)) {
    //     I2C_ClearITPendingBit(I2C2, I2C_IT_TX_EMPTY);
    //     I2C_ITConfig(I2C2, I2C_IT_TX_EMPTY, DISABLE);
    //     gTxFlag = 1;
    // }
    if(I2C_GetITStatus(I2C2, I2C_IT_RX_FULL)) {
        gData = I2C_ReceiveData(I2C2);
        I2C_ClearITPendingBit(I2C2, I2C_IT_RX_FULL);
        gRxFlag = 1;
    }
    if(I2C_GetITStatus(I2C2, I2C_IT_TX_ABRT)) {
        I2C_ClearITPendingBit(I2C2, I2C_IT_TX_ABRT);
        gAbrtFlag = 1;
    }
}

void Scan_All_Addr(I2C_TypeDef *I2Cx, u8 *ptr, u8 cnt)
{
    for(u8 i = 0x00; i <= 0x7F; i++){
        uint32_t I2C_Timeout = 0x100;
        gAbrtFlag = 0;
        I2C_Cmd(I2Cx, DISABLE);
        I2C_Send7bitAddress(I2Cx, i << 1, I2C_Direction_Receiver);
        I2C_Cmd(I2Cx, ENABLE);
        I2C_ReadCmd(I2Cx);

        while(gAbrtFlag == 0){
            if(--I2C_Timeout == 0){
                printf("Device Address 0x%02x is connected \n", i << 1);
                ptr[cnt++] = i << 1;
                I2C_GenerateSTOP(I2Cx, ENABLE);
                break;
            }
        }
    }

    if(cnt == 0x00){
        printf("Sorry, No I2C Slave Device is connected");
    }else{
        printf("%d I2C Slave Device is connected", cnt);
    }
}