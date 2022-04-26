////////////////////////////////////////////////////////////////////////////////
#ifndef __ESP8266_H_
#define __ESP8266_H_
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    ESP8266_OK       = (uint8_t)0,
    ESP8266_ERROR    = !ESP8266_OK
} ESP8266_Error_Typedef;

typedef enum {
    NO_WIFI         = (uint8_t)0,
    STA             = (uint8_t)1,
    AP              = (uint8_t)2,
    MIX             = (uint8_t)3
} ESP8266_Mode_Typedef;

typedef enum {
    QUICK_SCAN      = (uint8_t)0,
    ALL_SCAN        = (uint8_t)1
} ESP8266_STA_Scan_Mode_Typedef;

typedef enum {
    WIFI_IDLE            = (uint8_t)0,
    WIFI_CONNECTING      = (uint8_t)1,
    WIFI_WRONG_PSD       = (uint8_t)2,
    WIFI_NO_SSID         = (uint8_t)3,
    WIFI_FAIL            = (uint8_t)4,
    WIFI_GOT_IP          = (uint8_t)5
} ESP8266_Wifi_Status_Typedef;

typedef enum {
    TCP_IDLE            = (uint8_t)0,
    TCP_CONNECTING      = (uint8_t)1,
    TCP_FAIL            = (uint8_t)4,
    TCP_GOT_TCP         = (uint8_t)5
} ESP8266_TCP_Status_Typedef;

typedef enum{
    NO_INIT             = (uint8_t)0,
    USERCFG_SET         = (uint8_t)1,
    CONNCFG_SET         = (uint8_t)2,
    DISCONNECTED        = (uint8_t)3,
    CONNECTED           = (uint8_t)4,
    CONNECTED_NO_SUB    = (uint8_t)5,
    CONNECTED_AND_SUB   = (uint8_t)6
} ESP8266_MQTT_Status_Typedef;

////////////////////////////////////////////////////////////////////////////////
#ifdef _ESP8266_C_

#define ESP8266_UART                    UART8

#define ESP8266_WIFI_SSID               "MMNJ"
#define ESP8266_WIFI_PWD                "MMNJ2017"
#define ESP8266_WIFI_BSSID              "0"

#define ESP8266_TCP_IPADDR_0            10
#define ESP8266_TCP_IPADDR_1            3
#define ESP8266_TCP_IPADDR_2            1
#define ESP8266_TCP_IPADDR_3            100

#define ESP8266_TCP_PORT                5555

#define ESP8266_MQTT_CLIENTIP           "mm32f3270"
#define ESP8266_MQTT_USERNAME           "mmnj"
#define ESP8266_MQTT_PSD                "mmnjmmnj"
#define ESP8266_MQTT_HOST               "broker-cn.emqx.io"
#define ESP8266_MQTT_HOST_IPADDR_0      47
#define ESP8266_MQTT_HOST_IPADDR_1      111
#define ESP8266_MQTT_HOST_IPADDR_2      117
#define ESP8266_MQTT_HOST_IPADDR_3      220
#define ESP8266_MQTT_PORT               1883
#define ESP8266_MQTT_TOPIC              "esp8266"

#define STR1(R)  #R
#define STR2(R)  STR1(R)

typedef struct 
{
    char*                               ssid;
    char*                               pwd;
    bool                                bssidEn;
    char*                               bssid;
    ESP8266_STA_Scan_Mode_Typedef       scanMode;
} ESP8266_STA_Typedef;

typedef struct 
{
    uint8_t                             staIPAddr0;
    uint8_t                             staIPAddr1;
    uint8_t                             staIPAddr2;
    uint8_t                             staIPAddr3;
} ESP8266_Device_IPADDR_Typedef;

typedef struct 
{
    uint8_t                             staMAC0;
    uint8_t                             staMAC1;
    uint8_t                             staMAC2;
    uint8_t                             staMAC3;
    uint8_t                             staMAC4;
    uint8_t                             staMAC5;
} ESP8266_Device_IPMAC_Typedef;

typedef struct 
{
    ESP8266_Device_IPADDR_Typedef       staIP;
    ESP8266_Device_IPMAC_Typedef        staMAC;
} ESP8266_Device_IP_Typedef;

typedef struct
{
    ESP8266_Mode_Typedef                mode;
    ESP8266_Wifi_Status_Typedef         status;
    ESP8266_STA_Typedef                 staPara;
    ESP8266_Device_IP_Typedef           deviceIP;
} ESP8266_WIFI_Typedef;
ESP8266_WIFI_Typedef ESP8266_WIFI_InitStruct;

typedef struct
{
    uint8_t                             ipAddr0;
    uint8_t                             ipAddr1;
    uint8_t                             ipAddr2;
    uint8_t                             ipAddr3;
}ESP8266_IPADDR_Typedef;

typedef struct
{
    ESP8266_IPADDR_Typedef              ipAddr;
    uint32_t                            port;
    ESP8266_TCP_Status_Typedef          tcpStatus;
    bool                                passthrough;
} ESP8266_TCP_Typedef;
ESP8266_TCP_Typedef ESP8266_TCP_InitStruct;

typedef struct
{
    uint8_t                             mqttIPAddr0;
    uint8_t                             mqttIPAddr1;
    uint8_t                             mqttIPAddr2;
    uint8_t                             mqttIPAddr3;
}ESP8266_MQTT_IPADDR_Typedef;

typedef struct
{
    ESP8266_MQTT_IPADDR_Typedef         ipAddr;
    uint32_t                            port;
    char*                               clientID;
    char*                               username;
    char*                               password;
    ESP8266_MQTT_Status_Typedef         mqttStatus;
} ESP8266_MQTT_Typedef;
ESP8266_MQTT_Typedef ESP8266_MQTT_InitStruct;

#endif

////////////////////////////////////////////////////////////////////////////////
char* stringJoint(char *cmd1, char *cmd2);
char* stringCommaJoint(char *cmd);
char* stringQutJoint(char *cmd);
char* stringEndJoint(char *cmd);
int charLocation(char *subject, char targetChar, int targetNum);
char* charExtract(char *subject, int start, int end);
void ESP8266_UART_SendCmd(char *cmd);
ESP8266_Error_Typedef ESP8266_Send_AT_Command(char *cmd, char *ack, u32 waitTime);
ESP8266_Error_Typedef ESP8266_Send_AT_Command_Times(char *cmd, char *ack, u32 longestWaitTime, u8 maxTimes);
void ESP8266_Init_Exit_Passthrough();
ESP8266_Error_Typedef ESP8266_Init();
ESP8266_Error_Typedef ESP8266_Reset();
void ESP8266_WIFI_Init(ESP8266_Mode_Typedef wifiMode);
ESP8266_Error_Typedef ESP8266_Connect_Wifi_STA();
void ESP8266_TCP_Init();
ESP8266_Error_Typedef ESP8266_Connect_TCP();
ESP8266_Error_Typedef ESP8266_Configure_Passthrough(bool cipmode);
ESP8266_Error_Typedef ESP8266_TCP_Communication_Start();
void ESP8266_TCP_SendData(char *data);
void ESP8266_MQTT_Init();
ESP8266_Error_Typedef ESP8266_Connect_MQTT();
ESP8266_Error_Typedef ESP8266_MQTT_SUB(char* subTopic);
ESP8266_Error_Typedef ESP8266_MQTT_UNSUB(char* subTopic);
ESP8266_Error_Typedef ESP8266_MQTT_PUB(char* pubTopic, char* data);
ESP8266_Error_Typedef ESP8266_MQTT_CLEAN();
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
