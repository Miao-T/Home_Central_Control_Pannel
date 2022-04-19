////////////////////////////////////////////////////////////////////////////////
#define _ESP8266_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "mm32.h"
#include "HAL_uart.h"
#include "esp8266.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
char* stringJoint(char *cmd)
{
    char *sign = "\r\n";
    char *wholeCmd = (char *)malloc(strlen(cmd) + strlen(sign));
    sprintf(wholeCmd, "%s%s", cmd, sign);
    return wholeCmd;
}

////////////////////////////////////////////////////////////////////////////////
void ESP8266_UART_SendCmd(UART_TypeDef *UARTx, char *cmd)
{
    char *wholeCmd = stringJoint(cmd);
    UART_SendPackage(UARTx, (u8*)wholeCmd, sizeof(wholeCmd));
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_UART_Check_AT_Command(UART_TypeDef *UARTx, char *cmd, char *ack, u32 waitTime)
{
    ESP8266_UART_SendCmd(UARTx, *cmd);
    delay(waitTime);
    if(strstr(rxBuffer, ack)){
        // strstr(), return NULL if no finding
        printf("Send AT Command:%s \n Get Ack:%s \n", cmd, ack);
        return ESP8266_ERROR;
    }else{
        printf("Send AT Command:%s \n No Right Ack:%s \n", cmd, ack);
        return ESP8266_OK;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ESP8266_Init(){
    
}
