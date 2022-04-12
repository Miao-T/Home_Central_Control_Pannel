////////////////////////////////////////////////////////////////////////////////
/// @file     MAIN.C
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
#define _MAIN_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include "mm32_types.h"
#include "mm32_system.h"
#include "common.h"

#include "bsp_key.h"
#include "bsp_led.h"

#include "hal_uart.h"

#include "main.h"
#include "uart.h"
#include "i2c.h"
#include "hts221.h"
#include "lps22hh.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_UID
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UID_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
void initPeri()
{
    BSP_KEY_Configure();
    BSP_LED_Configure();
    BSP_UART_Configure();
    BSP_I2C_Configure();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ >= 500) {
        tickFlag = true;
        tickCnt = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  main function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    MCUID = SetSystemClock(emSYSTICK_On, AppTaskTick);

    initPeri();
    // u8 ptr[4] = {0, 0, 0, 0};
    // if(Scan_All_Addr(I2C2, ptr)){
    //     printf("Sorry, No I2C Slave Device is connected");
    // }

    /*                  LPS22HH               */
    LPS22HH_Init(I2C2, DISABLE, LPS22HH_OUTPUT_DATA_RATE_1);
    LPS22HH_FIFO_Init(I2C2, LPS22HH_FIFO_MODE_FIFO, ENABLE, 0x0A);
    u8 data[2] = {0, 0};
    Sensor_Read(I2C2, 0XB8, 0X13, data, 2);
    printf("LPS22HH READY \n");
    // u8 ths_p[2] = {0x01, 0x00};
    // LPS22HH_Interrupt_Enable(I2C2, LPS22HH_IT_PHE, LPS22HH_IT_MODE_AUTOREFP, ths_p);
    int32_t pressure_lps = 0;
    int16_t temperature_lps = 0;

    /*                  HTS221               */
    HTS221_Init(I2C2, DISABLE, HTS221_OUTPUT_DATA_RATE_1);
    HTS221_Temperature_Calibration_Get(I2C2);
    HTS221_Humidity_Calibration_Get(I2C2);
    printf("HTS221 READY \n");
    int16_t temperature_hts = 0;
    uint16_t humidity_hts = 0;

    while (1) {
        LD1_on();
        // HTS221_Calculation(I2C2, &humidity_hts, &temperature_hts, DISABLE);
        LPS22HH_Calculation(I2C2, &pressure_lps, &temperature_lps, DISABLE, ENABLE);

        u8 ptr[2] = {0x00, 0x00};
        Sensor_Read(I2C2, 0xB8, 0x25, &ptr[0], 1);
        Sensor_Read(I2C2, 0xB8, 0x26, &ptr[1], 1);
        printf("status is %d, %d \n", ptr[0], ptr[1]);

        // if(LPS22HH_INT_SOURCE_Get(I2C2, LPS22HH_ITS_PH_FLAG)){
        //     printf("high pressure interrupt happened");
        // }
    }
}

/// @}

/// @}

/// @}
