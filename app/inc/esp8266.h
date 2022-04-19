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

typedef struct 
{
    char                            ssid[32];
    char                            pwd[64];
    bool                            bssidEn;
    char                            bssid[6];
    ESP8266_STA_Scan_Mode_Typedef   scanMode;
} ESP8266_STA_Typedef;

typedef struct
{
    ESP8266_Mode_Typedef            mode;
    bool                            status;
    ESP8266_STA_Typedef             staPara;
} ESP8266_WIFI_Typedef;
ESP8266_WIFI_Typedef ESP8266_WIFI_InitStruct;

////////////////////////////////////////////////////////////////////////////////
#ifdef _ESP8266_C_
char *endSign = "\r\n";

#endif


////////////////////////////////////////////////////////////////////////////////
char* stringJoint(char *cmd);
void ESP8266_UART_SendCmd(UART_TypeDef *UARTx, char *cmd);
void ESP8266_Init();
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
