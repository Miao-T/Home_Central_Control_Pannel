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
    Sensor_Write(I2Cx, SLAVE_ADDRESS_READ, regAddr, ptr, cnt);

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
    if(HTS221_Reg_Read(I2Cx, LPS22HH_WHO_AM_I, &device_id, 1))
        return LPS22HH_ERROR;
    return device_id;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef LPS22HH_Init(I2C_TypeDef *I2Cx)
{
    u8 buffer[3] = {0, 0, 0};
    buffer[0] = LPS22HH_CR1_BDU | LPS22HH_CR1_ODR_ONE_SHOT;                     // Set BDU and ODR in CTRL_REG1
    buffer[1] = LPS22HH_CR2_ONE_SHOT | LPS22HH_CR2_IF_ADD_INC;                  // Set ONESHOT and AUTOINC in CTRL_REG1
    buffer[2] = 0x00;                                                           // Clear CTRL_REG3

    if(HTS221_WHO_AM_I_Get(I2Cx) == DEVICE_ID_WHO_AM_I){
        if(HTS221_Reg_Write(I2Cx, LPS22HH_CTRL_REG1, buffer, 3))
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

    if(HTS221_Reg_Write(I2Cx, LPS22HH_CTRL_REG1, buffer, 3))
        return LPS22HH_ERROR;

    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef HTS221_DeInit(I2C_TypeDef *I2Cx)
{
    u8 buffer[4] = {0, 0, 0, 0};

    if(HTS221_Reg_Write(I2Cx, LPS22HH_CTRL_REG1, buffer, 3))
        return LPS22HH_ERROR;

    return LPS22HH_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HH_Error_Typedef HTS221_TEMP_OUT_Get(I2C_TypeDef *I2Cx, int16_t *T_OUT)
{
    u8 buffer[4] = {0, 0, 0, 0};

    if(HTS221_Reg_Write(I2Cx, LPS22HH_CTRL_REG1, buffer, 3))
        return LPS22HH_ERROR;

    return LPS22HH_OK;
}