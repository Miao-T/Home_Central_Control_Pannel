////////////////////////////////////////////////////////////////////////////////
/// @file     UID.H
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE UID
///           EXAMPLES.
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
#ifndef __MAIN_H
#define __MAIN_H

// Files includes  -------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_UID
/// @brief UID example modules
/// @{







enum{
	REFRESH_ADC,
	REFRESH_TEMP,
	REFRESH_FLASH,
	REFRESH_I2C,
	REFRESH_CAN,
	REFRESH_RTC,
	REFRESH_UART1,
	REFRESH_UART2,
	REFRESH_IR,
	REFRESH_LED,
    REFRESH_MUSIC,
    REFRESH_SDIO,
    REFRESH_ETH
};

typedef struct {
	u16 adc;
	u16 temp;
	u16 flash;
	u16 i2c;
	u16 can;
	u16 rtc;
	u16 uart1;
	u16 uart2;
	u16 led;
	u16 lcd;
    u16 music;
    u16 sdio;
    u16 eth;
}Peripheraldef;

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Variables
/// @{

#ifdef _MAIN_C_

#define GLOBAL
#else
#define GLOBAL extern

#endif

GLOBAL char ready;
GLOBAL u32 tickCnt;

GLOBAL Peripheraldef error;
GLOBAL Peripheraldef rf;
GLOBAL Peripheraldef rfCnt;
GLOBAL Peripheraldef rfOverCnt;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Functions
/// @{

/// @}

void processOver();

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __UID_H */
////////////////////////////////////////////////////////////////////////////////
