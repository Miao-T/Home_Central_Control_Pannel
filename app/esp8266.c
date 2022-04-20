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
char* stringJoint(char *cmd1, char *cmd2)
{
    char *wholeCmd = (char *)malloc(strlen(cmd1) + strlen(cmd2));
    sprintf(wholeCmd, "%s%s", cmd1, cmd2);
    return wholeCmd;
}

////////////////////////////////////////////////////////////////////////////////
char* stringCommaJoint(char *cmd)
{
    char *sign = ",";
    char *wholeCmd = stringJoint(sign, cmd);
    return wholeCmd;
}

////////////////////////////////////////////////////////////////////////////////
char* stringQutJoint(char *cmd)
{
    char *sign = "\"";
    char *wholeCmd = stringJoint(sign, stringJoint(cmd, sign));
    return wholeCmd;
}

////////////////////////////////////////////////////////////////////////////////
char* stringEndJoint(char *cmd)
{
    char *sign = "\r\n";
    char *wholeCmd = stringJoint(cmd, sign);
    return wholeCmd;
}

////////////////////////////////////////////////////////////////////////////////
void ESP8266_UART_SendCmd(char *cmd)
{
    char *wholeCmd = stringEndJoint(cmd);
    UART_SendPackage(ESP8266_UART, (u8*)wholeCmd, strlen(wholeCmd));
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_UART_Send_AT_Command(char *cmd, char *ack, u32 waitTime)
{
    ESP8266_UART_SendCmd(cmd);
    delay(waitTime);
    if(strstr(rxBuffer, ack) == NULL){
        // strstr(), return NULL if no finding
        printf("Send AT Command:%s \n Get Ack:%s \n", cmd, ack);
        return ESP8266_ERROR;
    }else{
        printf("Send AT Command:%s \n No Right Ack:%s \n", cmd, ack);
        return ESP8266_OK;
    }
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Init(bool rstEn)
{
    // exit passthrough transmission mode
    char *exitCmd = "+++";
    ESP8266_UART_SendCmd(exitCmd);

    char *cmd = "AT";
    if(rstEn){
        cmd = "AT+RST";
        if(ESP8266_UART_Send_AT_Command(cmd, "OK", 100)){
            printf("Error: ESP8266 RST Fail \n");
            return ESP8266_ERROR;
        }else{
            printf("Error: ESP8266 RST Successfully \n");
        }
    }
    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 100)){
        printf("Error: ESP8266 Init Fail \n");
        return ESP8266_ERROR;
    }else{
        printf("Error: ESP8266 Init Successfully \n");
    }
    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
void ESP8266_WIFI_Init(ESP8266_Mode_Typedef wifiMode)
{
    ESP8266_WIFI_InitStruct.mode = wifiMode;
    ESP8266_WIFI_InitStruct.status = WIFI_IDLE;
    ESP8266_WIFI_InitStruct.staPara.ssid = ESP8266_WIFI_SSID;
    ESP8266_WIFI_InitStruct.staPara.pwd = ESP8266_WIFI_PWD;
    ESP8266_WIFI_InitStruct.staPara.bssidEn = 0;
    ESP8266_WIFI_InitStruct.staPara.bssid = ESP8266_WIFI_BSSID;
    ESP8266_WIFI_InitStruct.staPara.scanMode = QUICK_SCAN;
    printf("ssid is %s \n", ESP8266_WIFI_InitStruct.staPara.ssid);
    printf("pwd is %s \n", ESP8266_WIFI_InitStruct.staPara.pwd);
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Connect_Wifi_STA()
{
    ESP8266_WIFI_Init(STA);

    // Send WIFI Mode Command
    char *cmd = "AT+CWMODE=1";
    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 500)){
        printf("Error: Fail to configure WIFI mode STA \n");
        return ESP8266_ERROR;
    }else{
        printf("OK: Set WIFI mode STA successfully \n");
    }

    // Send WIFI Connect Command
    char *wifiPara = stringJoint(stringQutJoint(ESP8266_WIFI_SSID), stringCommaJoint(stringQutJoint(ESP8266_WIFI_PWD)));
    cmd = stringJoint("AT+CWJAP=", wifiPara);
    ESP8266_WIFI_InitStruct.status = WIFI_CONNECTING;
    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 2000)){
        ESP8266_WIFI_InitStruct.status = WIFI_FAIL;
        printf("Error: Fail to Connect WIFI %s \n", ESP8266_WIFI_SSID);
        return ESP8266_ERROR;
    }else{
        ESP8266_WIFI_InitStruct.status = WIFI_GOT_IP;
        printf("OK: Connect WIFI %s successfully \n", ESP8266_WIFI_SSID);
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
void ESP8266_TCP_Init()
{
    ESP8266_TCP_InitStruct.wifiPara = ESP8266_WIFI_InitStruct;
    ESP8266_TCP_InitStruct.ipAddr.ipAddr0 = ESP8266_TCP_IPADDR_0;
    ESP8266_TCP_InitStruct.ipAddr.ipAddr1 = ESP8266_TCP_IPADDR_1;
    ESP8266_TCP_InitStruct.ipAddr.ipAddr2 = ESP8266_TCP_IPADDR_2;
    ESP8266_TCP_InitStruct.ipAddr.ipAddr3 = ESP8266_TCP_IPADDR_3;
    ESP8266_TCP_InitStruct.port = (uint8_t)ESP8266_TCP_PORT;
    ESP8266_TCP_InitStruct.tcpStatus = TCP_IDLE;
    ESP8266_TCP_InitStruct.passthrough = 0;

    printf("port is %d \n", ESP8266_TCP_InitStruct.port);
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Connect_TCP()
{
    ESP8266_TCP_Init();

    char *ipAddr = (char *)malloc(sizeof("10.3.1.120"));
    sprintf(ipAddr, "%d.%d.%d.%d",  ESP8266_TCP_IPADDR_0,
                                    ESP8266_TCP_IPADDR_1,
                                    ESP8266_TCP_IPADDR_2,
                                    ESP8266_TCP_IPADDR_3);
    char *ipPort = (char *)malloc(sizeof("5050"));
    sprintf(ipPort, "%d", ESP8266_TCP_PORT);
    // char *cmd = stringJoint("AT+SIPSTART=\"TCP\",",stringJoint(stringQutJoint(ipAddr), stringCommaJoint(ipPort)));
    char *cmd = stringJoint("AT+SIPSTART=\"TCP\",",STR2(ESP8266_TCP_PORT));
    // char *cmd = stringJoint("AT+SIPSTART=\"TCP\",", stringJoint(stringQutJoint("10.1.1.1"), stringCommaJoint("5050")));
    ESP8266_TCP_InitStruct.tcpStatus = TCP_CONNECTING;
    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 5000)){
        ESP8266_TCP_InitStruct.tcpStatus = TCP_FAIL;
        printf("Error: Fail to Connect TCP %s \n", ipAddr);
        return ESP8266_ERROR;
    }else{
        ESP8266_TCP_InitStruct.tcpStatus = TCP_GOT_TCP;
        printf("OK: Connect TCP %s successfully \n", ipAddr);
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Configure_Passthrough(bool cipmode)
{
    char *cmd = "AT+CIPMODE=";
    if(cipmode){
        cmd = stringJoint(cmd,"1");
    }else{
        cmd = stringJoint(cmd,"0");
    }

    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 100)){
        printf("Error: Fail to Set CIPMODE %d \n", cipmode);
        return ESP8266_ERROR;
    }else{
        ESP8266_TCP_InitStruct.passthrough = !ESP8266_TCP_InitStruct.passthrough;
        printf("OK: Configure CIPMODE %d successfully \n", cipmode);
    }

    return ESP8266_OK;
}