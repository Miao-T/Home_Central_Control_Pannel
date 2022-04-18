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
////////////////////////////////////////////////////////////////////////////////
#define _UART_C_
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>	
#include "mm32_types.h"

#include "common.h"
#include "uart.h"

#include "hal_uart.h"
#include "hal_gpio.h"
#include "hal_nvic.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief  printf redirection function.
/// @param  ch: One character.
/// @param  f: File pointer.
/// @retval int32_t: One character.
////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUC__
#    define PUTCHAR_PROTOTYPE int32_t __io_putchar(int32_t ch)
#else
#    define PUTCHAR_PROTOTYPE int32_t fputc(int32_t ch, FILE * f)
#endif
PUTCHAR_PROTOTYPE
{
    UART_SendData(UART1, (uint8_t)ch);
    while (UART_GetFlagStatus(UART1, UART_CSR_TXC) == RESET);
    return ch;
}

#define BUFFERSIZE  10
char newTxBuffer[BUFFERSIZE];
char newRxBuffer[BUFFERSIZE];
u16 gSendLen;
////////////////////////////////////////////////////////////////////////////////
void initGPIO_UART(UART_TypeDef *UARTx)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if(UARTx == UART1){
        COMMON_EnableIpClock(emCLOCK_GPIOA);

        //UART1_TX   GPIOA.9
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);

        //UART1_RX   GPIOA.10
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
    }else if(UARTx == UART8){
        COMMON_EnableIpClock(emCLOCK_GPIOE);

        //UART8_TX   GPIOE.1
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOE, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_8);

        //UART8_RX   GPIOE.0
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOE, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_8);
    }
}

////////////////////////////////////////////////////////////////////////////////
void initUART(UART_TypeDef *UARTx, uint32_t baudrate)
{
    UART_InitTypeDef UART_InitStructure;

    if(UARTx == UART1){
        //Init UART1
        COMMON_EnableIpClock(emCLOCK_UART1);
        UART_InitStructure.BaudRate      = baudrate;
        UART_InitStructure.WordLength    = UART_WordLength_8b;
        UART_InitStructure.StopBits      = UART_StopBits_1;
        UART_InitStructure.Parity        = UART_Parity_No;
        UART_InitStructure.Mode          = UART_GCR_RX | UART_GCR_TX;
        UART_InitStructure.HWFlowControl = UART_HWFlowControl_None;

        UART_Init(UART1, &UART_InitStructure);
        UART_ITConfig(UART1, UART_IER_RX, ENABLE);
        UART_Cmd(UART1, ENABLE);
    }else if(UARTx == UART8){
        //Init UART8
        COMMON_EnableIpClock(emCLOCK_UART8);
        UART_InitStructure.BaudRate      = baudrate;
        UART_InitStructure.WordLength    = UART_WordLength_8b;
        UART_InitStructure.StopBits      = UART_StopBits_1;
        UART_InitStructure.Parity        = UART_Parity_No;
        UART_InitStructure.Mode          = UART_GCR_RX | UART_GCR_TX;
        UART_InitStructure.HWFlowControl = UART_HWFlowControl_None;

        UART_Init(UART8, &UART_InitStructure);
        UART_ITConfig(UART8, UART_IER_RX, ENABLE);
        UART_Cmd(UART8, ENABLE);
    }
}

////////////////////////////////////////////////////////////////////////////////
void NVIC_UART(UART_TypeDef *UARTx)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    if(UARTx == UART1){
        //UART1 NVIC
        NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }else if(UARTx == UART8){
        //UART8 NVIC
        NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}

////////////////////////////////////////////////////////////////////////////////
void UART1_IRQHandler(void)
{
    if(UART_GetITStatus(UART1, UART_ISR_TX) != RESET){
        UART_ClearITPendingBit(UART1, UART_ISR_TX);
        UART_ITConfig(UART1, UART_IER_TX, DISABLE);
        static u16 tCnt = 0;
        if (++ tCnt < gSendLen){
            UART_ITConfig(UART1, UART_IER_TX, ENABLE);
            UART_SendData(UART1, *(newTxBuffer + tCnt));
        }else{
            // txSuccess = true;
            tCnt = 0;
        }
    }
    if(UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
        static u16 rCnt = 0;
        *(newRxBuffer + rCnt) = UART_ReceiveData(UART1);
        rCnt++;
        if (rCnt >= BUFFERSIZE){
            // rxSuccess = true;
            rCnt = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void UART8_IRQHandler(void)
{
    if(UART_GetITStatus(UART8, UART_ISR_TX) != RESET){
        UART_ClearITPendingBit(UART8, UART_ISR_TX);
        UART_ITConfig(UART8, UART_IER_TX, DISABLE);
        static u16 tCnt = 0;
        if (++ tCnt < gSendLen){
            UART_ITConfig(UART8, UART_IER_TX, ENABLE);
            UART_SendData(UART8, *(newTxBuffer + tCnt));
        }else{
            // txSuccess = true;
            tCnt = 0;
        }
    }
    if(UART_GetITStatus(UART8, UART_ISR_RX) != RESET) {
        UART_ClearITPendingBit(UART8, UART_ISR_RX);
        static u16 rCnt = 0;
        *(newRxBuffer + rCnt) = UART_ReceiveData(UART8);
        rCnt++;
        if (rCnt >= BUFFERSIZE){
            // rxSuccess = true;
            rCnt = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void UART_SendPackage(UART_TypeDef *UARTx, u8* ptr, u16 len)
{
    gSendLen = len;
    UART_SendData(UART1, *(u16*)ptr);
    UART_ITConfig(UART1, UART_IER_TX, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
void BSP_UART_Configure()
{
    initGPIO_UART(UART1);
    initUART(UART1, 115200);
    // NVIC_UART(UART1);
    initGPIO_UART(UART8);
    initUART(UART8, 115200);
    NVIC_UART(UART8);
}