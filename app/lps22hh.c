////////////////////////////////////////////////////////////////////////////////
#define _LPS22HH_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>

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

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Init(I2C_TypeDef *I2Cx)
{
    u8 buffer[3] = {0, 0, 0};

    if(LPS22HH_WHO_AM_I_Get(I2Cx) == 0xB1){
        buffer[0] = LPS22HH_CR1_BDU | LPS22HH_CR1_ODR_ONE_SHOT;                     // Set BDU and ODR in CTRL_REG1
        buffer[1] = LPS22HH_CR2_ONE_SHOT | LPS22HH_CR2_IF_ADD_INC;                  // Set ONESHOT and AUTOINC in CTRL_REG1
        buffer[2] = 0x00;                                                           // Clear CTRL_REG3
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

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_PRESS_OUT_Get(I2C_TypeDef *I2Cx, int16_t *P_OUT)
{
    u8 buffer[3] = {0, 0, 0};
    u8 one_shot = LPS22HH_CR2_ONE_SHOT;
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_CTRL_REG2, &one_shot, 1))
        return LPS22HH_ERROR;
    if(LPS22HH_Reg_Read(I2Cx, LPS22HH_PRESS_OUT_XL, buffer, 3))
        return LPS22HH_ERROR;
    *P_OUT = (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_TEMP_OUT_Get(I2C_TypeDef *I2Cx, int16_t *T_OUT)
{
    u8 buffer[2] = {0, 0};
    u8 one_shot = LPS22HH_CR2_ONE_SHOT;
    if(LPS22HH_Reg_Write(I2Cx, LPS22HH_CTRL_REG2, &one_shot, 1))
        return LPS22HH_ERROR;
    if(LPS22HH_Reg_Read(I2Cx, LPS22HH_TEMP_OUT_L, buffer, 2))
        return LPS22HH_ERROR;
    *T_OUT = (buffer[1] << 8) | buffer[0];
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Pressure_Calculation(I2C_TypeDef *I2Cx, int16_t *value)
{
    int16_t T_OUT = 0x00;
    if(LPS22HH_PRESS_OUT_Get(I2Cx, &T_OUT))
        return LPS22HH_ERROR;

    *value = (T_OUT * 10) << 11;
    printf("Get Pressure value : %d.%d C \n", *value / 10, *value % 10);
    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t *value)
{
    int16_t T_OUT = 0x00;
    if(LPS22HH_TEMP_OUT_Get(I2Cx, &T_OUT))
        return LPS22HH_ERROR;

    *value = T_OUT / 10;
    printf("Get Temperature value : %d.%d C \n", *value / 10, *value % 10);
    return LPS22HH_OK;
}