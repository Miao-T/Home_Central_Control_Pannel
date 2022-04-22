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
    char *wholeCmd = (char *)malloc(strlen(cmd1) + strlen(cmd2) + 1);
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
int charLocation(char *subject, char targetChar, int targetNum)
{
    int len = strlen(subject) + 1;
    int num = 0;
    int i = 0;
    for(i = 0; i <= len; i++){
        if(subject[i] == targetChar){
            num++;
            if(num == targetNum)    break;
        }
    }
    if(i == (len + 1)){
        return -1;
    }else{
        return i + 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
char* charExtract(char *subject, int start, int end)
{
    char *result = (char *)malloc(sizeof(end - 1 - start));
    int num = 0;
    int i = 0;
    for(i = start; i < end - 1; i++){
        result[num] = subject[i];
        num++;
    }
    result[num] = '\0';
    return result;
}

////////////////////////////////////////////////////////////////////////////////
void ESP8266_UART_SendCmd(char *cmd)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));
    stringStart = 1;
    char *wholeCmd = stringEndJoint(cmd);
    UART_SendPackage(ESP8266_UART, (u8*)wholeCmd, strlen(wholeCmd));
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_UART_Send_AT_Command(char *cmd, char *ack, u32 longestWaitTime)
{
    ESP8266_UART_SendCmd(cmd);
    u32 timeOut = 0;
    while(strstr(rxBuffer, ack) == NULL){
        timeOut++;
        if(timeOut == longestWaitTime) break;
    }
    if(timeOut < longestWaitTime){
        printf("Send AT Command:%s   Get Ack:%s \n", cmd, ack);
        return ESP8266_OK;
    }else{
        printf("Send AT Command:%s   No Right Ack:%s \n", cmd, ack);
        return ESP8266_ERROR;
    }
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Init(bool rstEn)
{
    // exit passthrough transmission mode
    char *exitCmd = "+++";
    UART_SendPackage(ESP8266_UART, (u8*)exitCmd, strlen(exitCmd));
    delay(0xff);

    char *cmd = "AT";
    if(rstEn){
        cmd = "AT+RST";
        if(ESP8266_UART_Send_AT_Command(cmd, "OK", 0xffff)){
            printf("Error: ESP8266 RST Fail \n");
            return ESP8266_ERROR;
        }else{
            printf("OK: ESP8266 RST Successfully \n");
        }
    }else{
        if(ESP8266_UART_Send_AT_Command(cmd, "OK", 0xffff)){
            printf("Error: ESP8266 Init Fail \n");
            return ESP8266_ERROR;
        }else{
            printf("OK: ESP8266 Init Successfully \n");
        }
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

    // printf("ssid is %s \n", ESP8266_WIFI_InitStruct.staPara.ssid);
    // printf("pwd is %s \n", ESP8266_WIFI_InitStruct.staPara.pwd);
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Connect_Wifi_STA()
{
    ESP8266_WIFI_Init(STA);

    // Send WIFI Mode Command
    char *cmd = "AT+CWMODE=1";
    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 0xffff)){
        printf("Error: Fail to configure WIFI mode STA \n");
        // return ESP8266_ERROR;
    }else{
        printf("OK: Set WIFI mode STA successfully \n");
    }

    // Send WIFI Connect Command
    char *wifiPara = stringJoint(stringQutJoint(ESP8266_WIFI_SSID), stringCommaJoint(stringQutJoint(ESP8266_WIFI_PWD)));
    cmd = stringJoint("AT+CWJAP=", wifiPara);
    ESP8266_WIFI_InitStruct.status = WIFI_CONNECTING;
    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 0xffffffff)){
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
ESP8266_Error_Typedef ESP8266_Search_Device_IP()
{
    char *cmd = "AT+CIFSR";
    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 2000)){

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
    ESP8266_TCP_InitStruct.port = (uint32_t)ESP8266_TCP_PORT;
    ESP8266_TCP_InitStruct.tcpStatus = TCP_IDLE;
    ESP8266_TCP_InitStruct.passthrough = 0;

    // printf("port is %d \n", ESP8266_TCP_InitStruct.port);
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Connect_TCP()
{
    ESP8266_TCP_Init();

    char *ipAddr = (char *)malloc(3 * 4 + 3 + 1);                               // xxx.xxx.xxx.xxx \0
    sprintf(ipAddr, "%d.%d.%d.%d",  ESP8266_TCP_IPADDR_0,
                                    ESP8266_TCP_IPADDR_1,
                                    ESP8266_TCP_IPADDR_2,
                                    ESP8266_TCP_IPADDR_3);
    char *ipPort = STR2(ESP8266_TCP_PORT);
    char *cmd = stringJoint("AT+CIPSTART=\"TCP\",", stringJoint(stringQutJoint(ipAddr), stringCommaJoint(ipPort)));

    ESP8266_TCP_InitStruct.tcpStatus = TCP_CONNECTING;
    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 0xffffff)){
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

    if(ESP8266_UART_Send_AT_Command(cmd, "OK", 0xffff)){
        printf("Error: Fail to Set CIPMODE %d \n", cipmode);
        return ESP8266_ERROR;
    }else{
        ESP8266_TCP_InitStruct.passthrough = !ESP8266_TCP_InitStruct.passthrough;
        printf("OK: Configure CIPMODE %d successfully \n", cipmode);
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_TCP_Communication_Start()
{
    if(ESP8266_Configure_Passthrough(true))
        return ESP8266_ERROR;

    char *cmd = "AT+CIPSEND";
    if(ESP8266_UART_Send_AT_Command(cmd, ">", 0xffff)){
        printf("Error: Fail to Start Sending \n");
        return ESP8266_ERROR;
    }else{
        printf("OK: Please start to send data \n");
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
void ESP8266_TCP_SendData(char *data)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));
    stringStart = 1;
    UART_SendPackage(ESP8266_UART, (u8*)data, strlen(data));
}