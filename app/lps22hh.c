////////////////////////////////////////////////////////////////////////////////
#define _LPS22HH_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "HAL_i2c.h"
#include "lps22hh.h"
#include "i2c.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Reg_Write(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt)
{
    Sensor_Write(I2Cx, SLAVE_ADDRESS_WRITE, regAddr, ptr, cnt);

    return LPS22HH_OK;
}

////////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Reg_Read(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt)
{
    Sensor_Read(I2Cx, SLAVE_ADDRESS_READ, regAddr, ptr, cnt);

    return LPS22HH_OK;
}

////////////////////////////////////////////////////////////////////////////////
u8 LPS22HH_WHO_AM_I_Get(I2C_TypeDef *I2Cx)
{
    u8 device_id;
    if(LPS22HH_Reg_Read(I2Cx, LPS22HH_WHO_AM_I, &device_id, 1))
        return LPS22HH_ERROR;
    return device_id;
}

////////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Software_Reset(I2C_TypeDef *I2Cx)
{
    u8 swreset = 0x04;
    if(LPS22HH_Reg_Read(I2Cx, LPS22HH_CTRL_REG2, &swreset, 1))
        return LPS22HH_ERROR;
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Init(I2C_TypeDef *I2Cx, bool oneshot, u8 frequency)
{
    u8 buffer[3] = {0, 0, 0};

    if(LPS22HH_WHO_AM_I_Get(I2Cx) == DEVICE_ID_WHO_AM_I){
        buffer[0] = LPS22HH_IFC_I3C_DIS;                                        // Disable I3C in IF_CTRL
        if(LPS22HH_Reg_Write(I2Cx, LPS22HH_IF_CTRL, buffer, 1))
            return LPS22HH_ERROR;

        if(oneshot){
            buffer[0] = LPS22HH_CR1_BDU | LPS22HH_CR1_ODR_ONE_SHOT;             // Set BDU and ODR in CTRL_REG1
        }else{
            buffer[0] = LPS22HH_CR1_BDU | (frequency << LPS22HH_CR1_ODR_POS);   // Set BDU and ODR in CTRL_REG1
        }
        buffer[1] = LPS22HH_CR2_IF_ADD_INC;                                     // Set ONESHOT and AUTOINC in CTRL_REG1
        buffer[2] = 0x00;                                                       // Clear CTRL_REG3
        if(LPS22HH_Reg_Write(I2Cx, LPS22HH_CTRL_REG1, buffer, 3))
            return LPS22HH_ERROR;

        return LPS22HH_OK;
    }else{
        return LPS22HH_ERROR;
    }
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_DeInit(I2C_TypeDef *I2Cx)
{
    u8 buffer[4] = {0, 0, 0, 0};

    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_CTRL_REG1, buffer, 3))
        return LPS22HH_ERROR;

    return LPS22HH_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set FIFO mode and watermark level.
/// @param  I2Cn: where n can be 1 or 2 to select the I2C peripheral.
/// @param  fifoMode: specifies the fifo modes.
///   This parameter can be one of the following values:
/// @arg  LPS22HH_FIFO_MODE_BYPASS : 
/// @arg  LPS22HH_FIFO_MODE_FIFO : 
/// @arg  LPS22HH_FIFO_MODE_CONTINUOUS : 
/// @arg  LPS22HH_FIFO_MODE_BYPASS_2_FIFO : 
/// @arg  LPS22HH_FIFO_MODE_BYPASS_2_CONTINUOUS : 
/// @arg  LPS22HH_FIFO_MODE_CONTINUOUS_2_FIFO : 
/// @param  wtm: Enable the use of the FIFO watermark level.
///   This parameter can be: ENABLE or DISABLE.
/// @param  watermark: FIFO watermark level.
/// @retval The state of whether error happened when I2C operates HPS22HH.
////////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_FIFO_Init(I2C_TypeDef *I2Cx, u8 fifoMode, bool wtm, u8 watermark)
{
    u8 buffer = 0;

    buffer = 0x7F & watermark;       // WTM is 7 bit, ensure MSB = 0
    // Set FIFO watermark level first
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_FIFO_WTM, &buffer, 1))
        return LPS22HH_ERROR;

    // Set fifo mode and enable watermark
    buffer = fifoMode << LPS22HH_FC_MODE_POS | wtm << LPS22HH_FC_STOP_ON_WTM_POS;
    // Enable FIFO
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_FIFO_CTRL, &buffer, 1))
        return LPS22HH_ERROR;

    return LPS22HH_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable or disable the specified HPS22HH interrupts.
/// @param  I2Cn: where n can be 1 or 2 to select the I2C peripheral.
/// @param  it: specifies the HPS22HH interrupt sources to be enabled
///             or disabled.
///   This parameter can be one of the following values:
/// @arg  LPS22HH_ITC_PHE : Enable interrupr request on pressure value higher
///                         than preset threshold.
/// @arg  LPS22HH_ITC_PLE : Enable interrupr request on pressure value lower
///                         than preset threshold.
/// @param  state: new state of the specified HPS22HH interrupts.
///   This parameter can be: ENABLE or DISABLE.
/// @retval The state of whether error happened when I2C operates HPS22HH.
////////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Interrupt_Enable(I2C_TypeDef *I2Cx, u8 it, u8 mode, u8 *THS_P)
{
    u8 buffer;
    buffer = it | LPS22HH_ITC_DIFF_EN | LPS22HH_ITC_LIR | mode;
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_INTERRUPT_CFG, &buffer, 1))
        return LPS22HH_ERROR;

    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_THS_P_L, THS_P, 2))
        return LPS22HH_ERROR;

    return LPS22HH_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified HPS22HH interrupt flag is set or not.
/// @param  I2Cn: where n can be 1 or 2 to select the I2C peripheral.
/// @param  flag: specifies the flag to check.
///   This parameter can be one of the following values:
/// @arg  LPS22HH_ITS_PH_FLAG : High differential pressure flag.
/// @arg  LPS22HH_ITS_PL_FLAG : Low differential pressure flag.
/// @arg  LPS22HH_ITS_IA_FLAG : High or Low differential pressure flag.
/// @retval The new state of HTS22HH_Interrupt_Flag (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
FlagStatus LPS22HH_INT_SOURCE_Get(I2C_TypeDef *I2Cx, u8 flag)
{
    u8 buffer = 0;
    LPS22HH_Reg_Read(I2Cx, LPS22HH_INT_SOURCE, &buffer, 1);
    return ((buffer & flag) ? SET : RESET);
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_PRESS_OUT_Get(I2C_TypeDef *I2Cx, int32_t *P_OUT, bool fifo)
{
    uint8_t buffer[3] = {0, 0, 0};
    if(fifo){
        // read from fifo
        if(LPS22HH_Reg_Read(I2Cx, LPS22HH_FIFO_DATA_OUT_PRESS_XL, buffer, 3))
            return LPS22HH_ERROR;
    }else{
        // read from standard registers
        if(LPS22HH_Reg_Read(I2Cx, LPS22HH_PRESS_OUT_XL, buffer, 3))
            return LPS22HH_ERROR;
    }
    *P_OUT = (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_TEMP_OUT_Get(I2C_TypeDef *I2Cx, int16_t *T_OUT, bool fifo)
{
    uint8_t buffer[2] = {0, 0};
    if(fifo){
        // read from fifo
        if(LPS22HH_Reg_Read(I2Cx, LPS22HH_FIFO_DATA_OUT_TEMP_L, buffer, 2))
            return LPS22HH_ERROR;
    }else{
        // read from normal registers
        if(LPS22HH_Reg_Read(I2Cx, LPS22HH_TEMP_OUT_L, buffer, 2))
            return LPS22HH_ERROR;
    }

    *T_OUT = (buffer[1] << 8) | buffer[0];
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Pressure_Calculation(I2C_TypeDef *I2Cx, int32_t *value, bool fifo)
{
    int32_t P_OUT = 0x00;
    if(LPS22HH_PRESS_OUT_Get(I2Cx, &P_OUT, fifo))
        return LPS22HH_ERROR;

    *value = (P_OUT * 10) / 4096;
    printf("Get Pressure value : %d.%d hPa \n", *value / 10, abs(*value % 10));
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t *value, bool fifo)
{
    int16_t T_OUT = 0x00;
    if(LPS22HH_TEMP_OUT_Get(I2Cx, &T_OUT, fifo))
        return LPS22HH_ERROR;

    *value = T_OUT / 10;
    printf("Get Temperature value : %d.%d C \n", *value / 10, abs(*value % 10));
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Calculation(I2C_TypeDef *I2Cx, int32_t *p_value, int16_t *t_value, bool oneshot, bool fifo)
{
    if(oneshot){
        u8 one_shot = LPS22HH_CR2_ONE_SHOT | LPS22HH_CR2_IF_ADD_INC;
        if(LPS22HH_Reg_Write(I2Cx, LPS22HH_CTRL_REG2, &one_shot, 1))
            return LPS22HH_ERROR;
    }

    if(!fifo){
        u8 buffer = 0x00;
        if(LPS22HH_Reg_Read(I2Cx, LPS22HH_STATUS, &buffer, 1))
            return LPS22HH_ERROR;
        if(buffer && LPS22HH_STATUS_P_DA != 0){
            LPS22HH_Pressure_Calculation(I2Cx, p_value, fifo);
            LPS22HH_Altitude_Calculation(p_value);
        }
        if(buffer && LPS22HH_STATUS_T_DA != 0){
            LPS22HH_Temperature_Calculation(I2Cx, t_value, fifo);
        }
        return LPS22HH_OK;
    }else{
        u8 buffer = 0x00;
        if(LPS22HH_Reg_Read(I2Cx, LPS22HH_FIFO_STATUS2, &buffer, 1))
            return LPS22HH_ERROR;
        if(buffer && LPS22HH_FSTA_WTM_IA != 0){
            LPS22HH_Pressure_Calculation(I2Cx, p_value, fifo);
            LPS22HH_Temperature_Calculation(I2Cx, t_value, fifo);
            LPS22HH_Altitude_Calculation(p_value);

            LPS22HH_FIFO_Init(I2Cx, LPS22HH_FIFO_MODE_BYPASS, DISABLE, 0x00);
            LPS22HH_FIFO_Init(I2C2, LPS22HH_FIFO_MODE_FIFO, ENABLE, 0x0F);
        }
        return LPS22HH_OK;
    }
}

//////////////////////////////////////////////////////////////////////////////
void LPS22HH_Altitude_Calculation(int32_t *p_value)
{
    float altitude = 44300 * (1 - pow(*p_value / 10132.5, 0.19));
    printf("Get Altitude value : %.1f M \n", altitude);

}