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
/// @brief  Joint string cmd1 and cmd2.
/// @param  cmd1: First string.
/// @param  cmd2: Second string.
/// @retval The result string.
////////////////////////////////////////////////////////////////////////////////
char* stringJoint(char *cmd1, char *cmd2)
{
    char *wholeCmd = (char *)malloc(strlen(cmd1) + strlen(cmd2) + 1);
    sprintf(wholeCmd, "%s%s", cmd1, cmd2);
    return wholeCmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Joint string cmd and comma.
/// @param  cmd: First string.
/// @retval The result string.
////////////////////////////////////////////////////////////////////////////////
char* stringCommaJoint(char *cmd)
{
    char *sign = ",";
    char *wholeCmd = stringJoint(sign, cmd);
    return wholeCmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Joint string cmd and quotation.
/// @param  cmd: First string.
/// @retval The result string.
////////////////////////////////////////////////////////////////////////////////
char* stringQutJoint(char *cmd)
{
    char *sign = "\"";
    char *wholeCmd = stringJoint(sign, stringJoint(cmd, sign));
    return wholeCmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Joint string cmd and \r\n.
/// @param  cmd: First string.
/// @retval The result string.
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
/// @brief  Send commands to ESP8266 by UART.
/// @param  cmd: Commands sent to ESP8266.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ESP8266_UART_SendCmd(char *cmd)
{
    UART_ClearRxBuffer();
    char *wholeCmd = stringEndJoint(cmd);
    UART_SendPackage(ESP8266_UART, (u8*)wholeCmd, strlen(wholeCmd));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send AT commands to ESP8266 and check ack.
/// @param  cmd: Commands sent to ESP8266.
/// @param  ack: Ack from ESP8266 after receiving AT commands.
/// @param  longestWaitTime: Longest time waitting for right ack.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Send_AT_Command(char *cmd, char *ack, u32 longestWaitTime)
{
    u32 timeOut = 0;
    ESP8266_UART_SendCmd(cmd);
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
/// @brief  Sending AT commands to ESP8266 for maxTimes.
/// @param  cmd: Commands sent to ESP8266.
/// @param  ack: Ack from ESP8266 after receiving AT commands.
/// @param  longestWaitTime: Longest time waitting for right ack.
/// @param  maxTimes: Send commands for times. Return ERROR if no right ack.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Send_AT_Command_Times(char *cmd, char *ack, u32 longestWaitTime, u8 maxTimes)
{
    u8 times = 0;
    while(ESP8266_Send_AT_Command(cmd, ack, longestWaitTime)){
        if(++times >= maxTimes) break;
    }
    if(times < maxTimes){
        return ESP8266_OK;
    }else{
        return ESP8266_ERROR;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Sending +++ without '\r\n' to exit passthrough mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ESP8266_Init_Exit_Passthrough()
{
    // exit passthrough transmission mode
    char *exitCmd = "+++";
    UART_SendPackage(ESP8266_UART, (u8*)exitCmd, strlen(exitCmd));
    // wait for at least 1s
    u32 timeOut;
    for(timeOut = 0; timeOut < 0xffffff; timeOut++);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Exit passthrough mode first and send 'AT' to check ESP8266.
/// @param  None.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Init()
{
    ESP8266_Init_Exit_Passthrough();

    char *cmd = "AT";
    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        printf("Error: ESP8266 Init Fail \n");
        return ESP8266_ERROR;
    }else{
        printf("OK: ESP8266 Init Successfully \n");
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Exit passthrough mode first and reset ESP8266.
/// @param  None.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Reset()
{
    ESP8266_Init_Exit_Passthrough();

    char *cmd = "AT+RST";
    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        printf("Error: ESP8266 RST Fail \n");
        return ESP8266_ERROR;
    }else{
        printf("OK: ESP8266 RST Successfully \n");
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure ESP8266 WIFI parameters.
/// @param  mode: STA / AP / MIX.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ESP8266_WIFI_Init(ESP8266_Mode_Typedef mode)
{
    ESP8266_WIFI_InitStruct.mode = mode;
    ESP8266_WIFI_InitStruct.status = WIFI_IDLE;
    ESP8266_WIFI_InitStruct.staPara.ssid = ESP8266_WIFI_SSID;
    ESP8266_WIFI_InitStruct.staPara.pwd = ESP8266_WIFI_PWD;
    ESP8266_WIFI_InitStruct.staPara.bssidEn = 0;
    ESP8266_WIFI_InitStruct.staPara.bssid = ESP8266_WIFI_BSSID;
    ESP8266_WIFI_InitStruct.staPara.scanMode = QUICK_SCAN;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure STA mode and connect wifi.
/// @param  None.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Connect_Wifi_STA()
{
    ESP8266_WIFI_Init(STA);

    // Send WIFI Mode Command
    char *cmd = "AT+CWMODE=1";
    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        printf("Error: Fail to configure WIFI mode STA \n");
        // return ESP8266_ERROR;
    }else{
        printf("OK: Set WIFI mode STA successfully \n");
    }

    // Send WIFI Connect Command
    char *wifiPara = stringJoint(stringQutJoint(ESP8266_WIFI_SSID), stringCommaJoint(stringQutJoint(ESP8266_WIFI_PWD)));
    cmd = stringJoint("AT+CWJAP=", wifiPara);
    ESP8266_WIFI_InitStruct.status = WIFI_CONNECTING;
    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffffffff, 2)){
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
    if(ESP8266_Send_AT_Command(cmd, "OK", 2000)){

    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure ESP8266 WIFI parameters.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ESP8266_TCP_Init()
{
    ESP8266_TCP_InitStruct.ipAddr.ipAddr0 = ESP8266_TCP_IPADDR_0;
    ESP8266_TCP_InitStruct.ipAddr.ipAddr1 = ESP8266_TCP_IPADDR_1;
    ESP8266_TCP_InitStruct.ipAddr.ipAddr2 = ESP8266_TCP_IPADDR_2;
    ESP8266_TCP_InitStruct.ipAddr.ipAddr3 = ESP8266_TCP_IPADDR_3;
    ESP8266_TCP_InitStruct.port = (uint32_t)ESP8266_TCP_PORT;
    ESP8266_TCP_InitStruct.tcpStatus = TCP_IDLE;
    ESP8266_TCP_InitStruct.passthrough = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Connect TCP.
/// @param  None.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Connect_TCP()
{
    ESP8266_TCP_Init();

    char *ipAddr = (char *)malloc(3 * 4 + 3 + 1);                               // xxx.xxx.xxx.xxx\0
    sprintf(ipAddr, "%d.%d.%d.%d",  ESP8266_TCP_IPADDR_0,
                                    ESP8266_TCP_IPADDR_1,
                                    ESP8266_TCP_IPADDR_2,
                                    ESP8266_TCP_IPADDR_3);
    char *ipPort = STR2(ESP8266_TCP_PORT);
    char *cmd = stringJoint("AT+CIPSTART=\"TCP\",", stringJoint(stringQutJoint(ipAddr), stringCommaJoint(ipPort)));

    ESP8266_TCP_InitStruct.tcpStatus = TCP_CONNECTING;
    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffffff, 2)){
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
/// @brief  Configure passthrough mode.
/// @param  cipmode: 1-passthrough; 0-normal.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Configure_Passthrough(bool cipmode)
{
    char *cmd = "AT+CIPMODE=";
    if(cipmode){
        cmd = stringJoint(cmd,"1");
    }else{
        cmd = stringJoint(cmd,"0");
    }

    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        printf("Error: Fail to Set CIPMODE %d \n", cipmode);
        return ESP8266_ERROR;
    }else{
        ESP8266_TCP_InitStruct.passthrough = !ESP8266_TCP_InitStruct.passthrough;
        printf("OK: Configure CIPMODE %d successfully \n", cipmode);
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Communication with TCP server.
/// @param  None.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_TCP_Communication_Start()
{
    if(ESP8266_Configure_Passthrough(true))
        return ESP8266_ERROR;

    char *cmd = "AT+CIPSEND";
    if(ESP8266_Send_AT_Command_Times(cmd, ">", 0xffff, 2)){
        printf("Error: Fail to Start Sending \n");
        return ESP8266_ERROR;
    }else{
        printf("OK: Please start to send data \n");
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send data to TCP server.
/// @param  data: data sent to TCP server.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ESP8266_TCP_SendData(char *data)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));
    stringStart = 1;
    UART_SendPackage(ESP8266_UART, (u8*)data, strlen(data));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure ESP8266 MQTT parameters.
/// @param  clientId: Client ID num.
/// @param  username: MQTT server username to log in MQTT broker.
/// @param  psd: MQTT server password to log in MQTT broker.
/// @param  scheme: Verification methods.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ESP8266_MQTT_Init(char* clientId, char* username, char* psd, ESP8266_MQTT_Scheme_Typedef scheme)
{
    ESP8266_MQTT_InitStruct.ipAddr.mqttIPAddr0 = ESP8266_MQTT_HOST_IPADDR_0;
    ESP8266_MQTT_InitStruct.ipAddr.mqttIPAddr1 = ESP8266_MQTT_HOST_IPADDR_1;
    ESP8266_MQTT_InitStruct.ipAddr.mqttIPAddr2 = ESP8266_MQTT_HOST_IPADDR_2;
    ESP8266_MQTT_InitStruct.ipAddr.mqttIPAddr3 = ESP8266_MQTT_HOST_IPADDR_3;
    ESP8266_MQTT_InitStruct.port = (uint32_t)ESP8266_MQTT_PORT;
    ESP8266_MQTT_InitStruct.clientID = clientId;
    ESP8266_MQTT_InitStruct.username = username;
    ESP8266_MQTT_InitStruct.password = psd;
    ESP8266_MQTT_InitStruct.scheme = scheme;
    ESP8266_MQTT_InitStruct.mqttStatus = NO_INIT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Connect with MQTT server.
/// @param  clientId: Client ID num.
/// @param  username: MQTT server username to log in MQTT broker.
/// @param  psd: MQTT server password to log in MQTT broker.
/// @param  scheme: Verification methods.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Connect_MQTT(char* clientId, char* username, char* psd, ESP8266_MQTT_Scheme_Typedef scheme)
{
    ESP8266_MQTT_Init(clientId, username, psd, scheme);

    char *cmd = (char*)malloc(256);
    sprintf(cmd, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"", clientId, username, psd);

    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        printf("Error: Fail to Configure MQTT \n");
        return ESP8266_ERROR;
    }else{
        ESP8266_MQTT_InitStruct.mqttStatus = USERCFG_SET;
        printf("OK: Configure MQTT successfully \n");
    }

    sprintf(cmd, "AT+MQTTCONN=0,\"%d.%d.%d.%d\",%d,0", ESP8266_MQTT_HOST_IPADDR_0, ESP8266_MQTT_HOST_IPADDR_1, ESP8266_MQTT_HOST_IPADDR_2, ESP8266_MQTT_HOST_IPADDR_3, ESP8266_MQTT_PORT);
    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        ESP8266_MQTT_InitStruct.mqttStatus = DISCONNECTED;
        printf("Error: Fail to Connect MQTT \n");
        return ESP8266_ERROR;
    }else{
        ESP8266_MQTT_InitStruct.mqttStatus = CONNECTED;
        printf("OK: Connect MQTT successfully \n");
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Check MQTT client state.
/// @param  None.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_Check_MQTT(){
    char *cmd = "AT+MQTTCONN?";

    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        // printf("Error: Fail to Connect MQTT \n");
        return ESP8266_ERROR;
    }else{
        // printf("OK: Connect MQTT successfully \n");
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Subscribe topic from MQTT server.
/// @param  subTopic: Subscribed topic name.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_MQTT_SUB(char* subTopic)
{
    char *cmd = (char*)malloc(strlen("AT+MQTTSUB=0,\"%s\",1") + strlen(subTopic));
    sprintf(cmd, "AT+MQTTSUB=0,\"%s\",1", subTopic);

    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        ESP8266_MQTT_InitStruct.mqttStatus = CONNECTED_NO_SUB;
        printf("Error: Fail to Subscribe topic \n");
        return ESP8266_ERROR;
    }else{
        ESP8266_MQTT_InitStruct.mqttStatus = CONNECTED_AND_SUB;
        printf("OK: Subscribe topic successfully \n");
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Unsubscribe topic from MQTT server.
/// @param  subTopic: Unsubscribed topic name.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_MQTT_UNSUB(char* subTopic)
{
    char *cmd = (char*)malloc(strlen("AT+MQTTUNSUB=0,\"\"") + strlen(subTopic));
    sprintf(cmd, "AT+MQTTUNSUB=0,\"%s\"", subTopic);

    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        ESP8266_MQTT_InitStruct.mqttStatus = CONNECTED_NO_SUB;
        printf("Error: Fail to Subscribe topic \n");
        return ESP8266_ERROR;
    }else{
        ESP8266_MQTT_InitStruct.mqttStatus = CONNECTED_AND_SUB;
        printf("OK: Subscribe topic successfully \n");
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Publish topic to MQTT server.
/// @param  subTopic: Publish topic name.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_MQTT_PUB(char* pubTopic, char* data)
{
    char *cmd = (char*)malloc(strlen("AT+MQTTPUB=0,\"\",\"\",1,0") + strlen(pubTopic) + strlen(data));
    sprintf(cmd, "AT+MQTTPUB=0,\"%s\",\"%s\",1,0", pubTopic, data);

    if(ESP8266_Send_AT_Command_Times(cmd, "OK", 0xffff, 2)){
        printf("Error: Fail to Publish topic \n");
        return ESP8266_ERROR;
    }else{
        printf("OK: Publish topic successfully \n");
    }

    return ESP8266_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Disconnect with MQTT server.
/// @param  None.
/// @retval ESP8266 OK or ERROR.
////////////////////////////////////////////////////////////////////////////////
ESP8266_Error_Typedef ESP8266_MQTT_CLEAN()
{
    char *cmd = "AT+MQTTCLEAN=0";

    if(ESP8266_Send_AT_Command_Times(cmd, "OK\r\n", 0xffff, 2)){
        printf("Error: Fail to Disconnect MQTT \n");
        return ESP8266_ERROR;
    }else{
        ESP8266_MQTT_InitStruct.mqttStatus = NO_INIT;
        printf("OK: Disconnect MQTT successfully \n");
    }

    return ESP8266_OK;
}