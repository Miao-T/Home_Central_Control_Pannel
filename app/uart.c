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
#include "main.h"
#include "uart.h"	  

#include "hal_uart.h"
#include "hal_gpio.h"	
#include "hal_nvic.h"

    
////////////////////////////////////////////////////////////////////////////////
void initUART()
{
    UART_InitTypeDef UART_InitStructure;
    
    //Init UART1
    COMMON_EnableIpClock(emCLOCK_UART1);
    UART_InitStructure.BaudRate      = BAUDRATE;
    UART_InitStructure.WordLength    = UART_WordLength_8b;
    UART_InitStructure.StopBits      = UART_StopBits_1;
    UART_InitStructure.Parity        = UART_Parity_No;
    UART_InitStructure.Mode          = UART_GCR_RX | UART_GCR_TX;
    UART_InitStructure.HWFlowControl = UART_HWFlowControl_None;
    
    UART_Init(UART1, &UART_InitStructure);
    UART_ITConfig(UART1, UART_IER_RX, ENABLE);
    UART_Cmd(UART1, ENABLE);

    //Init UART2
    COMMON_EnableIpClock(emCLOCK_UART2);
    UART_InitStructure.BaudRate      = BAUDRATE;
    UART_InitStructure.WordLength    = UART_WordLength_8b;
    UART_InitStructure.StopBits      = UART_StopBits_1;
    UART_InitStructure.Parity        = UART_Parity_No;
    UART_InitStructure.Mode          = UART_GCR_RX | UART_GCR_TX;
    UART_InitStructure.HWFlowControl = UART_HWFlowControl_None;
    
    UART_Init(UART2, &UART_InitStructure);
    UART_ITConfig(UART2, UART_IER_RX, ENABLE);
    UART_Cmd(UART2, ENABLE);
    
    //Init UART8
    COMMON_EnableIpClock(emCLOCK_UART8);
    UART_InitStructure.BaudRate      = BAUDRATE;
    UART_InitStructure.WordLength    = UART_WordLength_8b;
    UART_InitStructure.StopBits      = UART_StopBits_1;
    UART_InitStructure.Parity        = UART_Parity_No;
    UART_InitStructure.Mode          = UART_GCR_RX | UART_GCR_TX;
    UART_InitStructure.HWFlowControl = UART_HWFlowControl_None;
    
    UART_Init(UART8, &UART_InitStructure);
    UART_ITConfig(UART8, UART_IER_RX, ENABLE);
    UART_Cmd(UART8, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
void initGPIO_UART()
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_GPIOE);
    
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
    
    //UART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
   
	//UART2_RX   GPIOA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);

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

////////////////////////////////////////////////////////////////////////////////
void NVIC_UART()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
    //UART1 NVIC
	NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
    
    //UART2 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
    
    //UART8 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
} 

////////////////////////////////////////////////////////////////////////////////
void UART1_IRQHandler(void)
{
    if (UART_GetITStatus(UART1, UART_IER_RX) != RESET) {
        UART_ClearITPendingBit(UART1, UART_IER_RX);
        //memmove(&uart1RxBuf[0], &uart1RxBuf[1], 7);
		//uart1RxBuf[7] = UART_ReceiveData(UART1);
		//rf.uart1 = true;
		//error.uart1 = false;
		//rfOverCnt.uart1 = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
void UART2_IRQHandler(void)
{
    if (UART_GetITStatus(UART2, UART_IER_RX) != RESET) {
        UART_ClearITPendingBit(UART2, UART_IER_RX);
        memmove(&uart1RxBuf[0], &uart1RxBuf[1], 7);
		uart1RxBuf[7] = UART_ReceiveData(UART2);
		rf.uart1 = true;
		error.uart1 = false;
		rfOverCnt.uart1 = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
void UART8_IRQHandler(void)
{
    if (UART_GetITStatus(UART8, UART_IER_RX) != RESET) {
        UART_ClearITPendingBit(UART8, UART_IER_RX);
        memmove(&uart2RxBuf[0], &uart2RxBuf[1], 7);
		uart2RxBuf[7] = UART_ReceiveData(UART8);
		rf.uart2 = true;
		error.uart2 = false;
		rfOverCnt.uart2 = 0;
    }
}
    

////////////////////////////////////////////////////////////////////////////////
void BSP_UART_Configure()
{
    initGPIO_UART();
    initUART();
    NVIC_UART();
}