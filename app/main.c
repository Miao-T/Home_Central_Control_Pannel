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
#include "lps22hb.h"

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
    // Scan_All_Addr(I2C2, I2CSlaveAddr, cnt));

    /*                  LPS22HH               */
    LPS22HB_Reset_Software_BOOT(I2C2);
    LPS22HB_Init(I2C2, DISABLE, LPS22HB_OUTPUT_DATA_RATE_10);
    u8 p_ref_value[2] = {0x10, 0x00};
    LPS22HB_Interrupt_Enable(I2C2, LPS22HB_IT_PHE, p_ref_value);
    LPS22HB_AUTO_Configure(I2C2, LPS22HB_AUTORIFP_MODE_EN);
    // LPS22HB_FIFO_Configure(ENABLE, LPS22HB_FIFO_MODE_FIFO, DISABLE, 0x00);
    // LPS22HB_FIFO_Init(I2C2);

    printf("LPS22HB READY \n");
    // u8 ths_p[2] = {0x01, 0x00};
    // LPS22HH_Interrupt_Enable(I2C2, LPS22HH_IT_PHE, LPS22HH_IT_MODE_AUTOREFP, ths_p);
    int32_t pressure_lps = 0;
    int16_t temperature_lps = 0;

    /*                  HTS221               */
    // HTS221_Init(I2C2, DISABLE, HTS221_OUTPUT_DATA_RATE_1);
    // HTS221_Temperature_Calibration_Get(I2C2);
    // HTS221_Humidity_Calibration_Get(I2C2);
    // printf("HTS221 READY \n");
    // int16_t temperature_hts = 0;
    // uint16_t humidity_hts = 0;

    while (1) {
        LD1_on();
        // HTS221_Calculation(I2C2, &humidity_hts, &temperature_hts, DISABLE);
        LPS22HB_Calculation(I2C2, &pressure_lps, &temperature_lps, DISABLE);
        LPS22HB_INT_SOURCE_Get(I2C2, LPS22HB_ITS_PH_FLAG);
        u8 buffer[5] = {0, 0, 0, 0, 0};
        Sensor_Read(I2C2, 0xB8, 0x15, buffer, 3);
        printf("ref is %x%x%x\n", buffer[0], buffer[1], buffer[2]);

        // if(LPS22HH_INT_SOURCE_Get(I2C2, LPS22HH_ITS_PH_FLAG)){
        //     printf("high pressure interrupt happened");
        // }
    }
}

/// @}

/// @}

/// @}
