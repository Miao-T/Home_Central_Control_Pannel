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

// ////////////////////////////////////////////////////////////////////////////////
// LPS22HH_STATUS_Typedef LPS22HH_STATUS_Get(I2C_TypeDef *I2Cx)
// {
//     LPS22HH_STATUS_Typedef LPS22HH_STATUS_Initstruct;

// }

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Init_OneShot(I2C_TypeDef *I2Cx)
{
    u8 buffer[3] = {0, 0, 0};

    if(LPS22HH_WHO_AM_I_Get(I2Cx) == DEVICE_ID_WHO_AM_I){
        buffer[0] = LPS22HH_IFC_I3C_DIS;                                        // Disable I3C in IF_CTRL
        if(LPS22HH_Reg_Write(I2Cx, LPS22HH_IF_CTRL, buffer, 1))
            return LPS22HH_ERROR;

        buffer[0] = LPS22HH_CR1_BDU | LPS22HH_CR1_ODR_ONE_SHOT;                 // Set BDU and ODR in CTRL_REG1
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
LPS22HH_Error_Typedef LPS22HH_Init_Frequency(I2C_TypeDef *I2Cx)
{
    u8 buffer[3] = {0, 0, 0};

    if(LPS22HH_WHO_AM_I_Get(I2Cx) == DEVICE_ID_WHO_AM_I){
        buffer[0] = LPS22HH_IFC_I3C_DIS;                                        // Disable I3C in IF_CTRL
        if(LPS22HH_Reg_Write(I2Cx, LPS22HH_IF_CTRL, buffer, 1))
            return LPS22HH_ERROR;

        buffer[0] = LPS22HH_CR1_BDU | LPS22HH_CR1_ODR_10;                       // Set BDU and ODR in CTRL_REG1
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
//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Interrupt_Enable(I2C_TypeDef *I2Cx, u8 it, FunctionalState itState, u8 mode, FunctionalState modeState)
{
    u8 buffer[3] = {0, 0, 0};
    // if(mode == )
    // buffer[0] = it | (itState << LPS22HH_ITC_DIFF_EN_POS) | LPS22HH_ITC_LIR | mode | modeState;
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_INTERRUPT_CFG, &buffer[0], 1))
        return LPS22HH_ERROR;

    buffer[1] = 0x01;
    buffer[2] = 0x01;
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_THS_P_L, &buffer[1], 2))
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
//////////////////////////////////////////////////////////////////////////////
FlagStatus LPS22HH_INT_SOURCE_Get(I2C_TypeDef *I2Cx, u8 flag)
{
    u8 buffer = 0;
    LPS22HH_Reg_Read(I2Cx, LPS22HH_INT_SOURCE, &buffer, 1);
    return ((buffer & flag) ? SET : RESET);
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_PRESS_OUT_Get_OneShot(I2C_TypeDef *I2Cx, int32_t *P_OUT)
{
    uint8_t buffer[3] = {0, 0, 0};
    u8 one_shot = LPS22HH_CR2_ONE_SHOT | LPS22HH_CR2_IF_ADD_INC;
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_CTRL_REG2, &one_shot, 1))
        return LPS22HH_ERROR;
    if(LPS22HH_Reg_Read(I2Cx, LPS22HH_PRESS_OUT_XL, buffer, 3))
        return LPS22HH_ERROR;
    *P_OUT = (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_TEMP_OUT_Get_OneShot(I2C_TypeDef *I2Cx, int16_t *T_OUT)
{
    uint8_t buffer[2] = {0, 0};
    u8 one_shot = LPS22HH_CR2_ONE_SHOT | LPS22HH_CR2_IF_ADD_INC;
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_CTRL_REG2, &one_shot, 1))
        return LPS22HH_ERROR;
    if(LPS22HH_Reg_Read(I2Cx, LPS22HH_TEMP_OUT_L, buffer, 2))
        return LPS22HH_ERROR;
    *T_OUT = (buffer[1] << 8) | buffer[0];
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_PRESS_OUT_Get_Frequency(I2C_TypeDef *I2Cx, int32_t *P_OUT)
{
    uint8_t buffer[3] = {0, 0, 0};
    if(LPS22HH_Reg_Read(I2Cx, LPS22HH_PRESS_OUT_XL, buffer, 3))
        return LPS22HH_ERROR;
    *P_OUT = (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_TEMP_OUT_Get_Frequency(I2C_TypeDef *I2Cx, int16_t *T_OUT)
{
    uint8_t buffer[2] = {0, 0};
    if(LPS22HH_Reg_Read(I2Cx, LPS22HH_TEMP_OUT_L, buffer, 2))
        return LPS22HH_ERROR;
    *T_OUT = (buffer[1] << 8) | buffer[0];
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Pressure_Calculation(I2C_TypeDef *I2Cx, int32_t *value)
{
    int32_t T_OUT = 0x00;
    if(LPS22HH_PRESS_OUT_Get_OneShot(I2Cx, &T_OUT))
        return LPS22HH_ERROR;

    *value = (T_OUT * 10) / 4096;
    printf("Get Pressure value : %d.%d hPa \n", *value / 10, abs(*value % 10));
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t *value)
{
    int16_t T_OUT = 0x00;
    if(LPS22HH_TEMP_OUT_Get_OneShot(I2Cx, &T_OUT))
        return LPS22HH_ERROR;

    *value = T_OUT / 10;
    printf("Get Temperature value : %d.%d C \n", *value / 10, abs(*value % 10));
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
void LPS22HH_Altitude_Calculation(int32_t press_value)
{
    float altitude = 44300 * (1 - pow(press_value / 10132.5, 0.19));
    printf("Get Altitude value : %.1f M \n", altitude);

}