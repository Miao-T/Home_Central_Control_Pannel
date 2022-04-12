////////////////////////////////////////////////////////////////////////////////
#define _HTS221_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>

#include "common.h"
#include "HAL_i2c.h"
#include "hts221.h"
#include "i2c.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Reg_Write(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt)
{
    Sensor_Write(I2Cx, SLAVE_ADDRESS_READ, regAddr, ptr, cnt);

    return HTS221_OK;
}

////////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Reg_Read(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt)
{
    Sensor_Read(I2Cx, SLAVE_ADDRESS_READ, regAddr, ptr, cnt);

    return HTS221_OK;
}

////////////////////////////////////////////////////////////////////////////////
u8 HTS221_WHO_AM_I_Get(I2C_TypeDef *I2Cx)
{
    u8 device_id;
    if(HTS221_Reg_Read(I2Cx, HTS221_WHO_AM_I, &device_id, 1))
        return HTS221_ERROR;
    return device_id;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Init(I2C_TypeDef *I2Cx, bool oneshot, u8 frequency)
{
    u8 buffer[4] = {0, 0, 0, 0};
    if(oneshot){
        // Set PD / BDU and ODR in CTRL_REG1
        buffer[0] = HTS221_CR1_PD | HTS221_CR1_BDU | HTS221_CR1_ODR_ONESHOT;
    }else{
        // Set PD / BDU and ODR in CTRL_REG1
        buffer[0] = HTS221_CR1_PD | HTS221_CR1_BDU | frequency << HTS221_CR1_ODR_POS;
    }
    buffer[1] = 0x00;                                                           // Clear CTRL_REG2
    buffer[2] = 0x00;                                                           // Clear CTRL_REG3
    buffer[3] = HTS221_AC_AVGH_32 | HTS221_AC_AVGT_16;                          // Set AVGH and AVGT in AV_CONF

    if(HTS221_WHO_AM_I_Get(I2Cx) == DEVICE_ID_WHO_AM_I){                        // Check Device ID
        if(HTS221_Reg_Write(I2Cx, CONTINUOUS_ADDRESS(HTS221_CTRL_REG1), buffer, 3))
            return HTS221_ERROR;
        if(HTS221_Reg_Write(I2Cx, HTS221_AV_CONF, &buffer[3], 1))
            return HTS221_ERROR;

        return HTS221_OK;
    }else{
        return HTS221_ERROR;
    }
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_DeInit(I2C_TypeDef *I2Cx)
{
    u8 buffer[4] = {0, 0, 0, 0};

    if(HTS221_Reg_Write(I2Cx, CONTINUOUS_ADDRESS(HTS221_CTRL_REG1), buffer, 3))
        return HTS221_ERROR;
    if(HTS221_Reg_Write(I2Cx, HTS221_AV_CONF, &buffer[3], 1))
        return HTS221_ERROR;

    return HTS221_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Humidity_Calibration_Get(I2C_TypeDef *I2Cx)
{
    u8 buffer[2] = {0, 0};
    if(HTS221_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_H0_RH_X2), buffer, 2))
        return HTS221_ERROR;
    H_Cal_InitStruct.H0_rH = (int16_t)(buffer[0] >> 1);                         // H0_rH = H0_rH_x2 / 2
    H_Cal_InitStruct.H1_rH = (int16_t)(buffer[1] >> 1);                         // H1_rH = H1_rH_x2 / 2

    if(HTS221_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_H0_T0_OUT_L), buffer, 2))
        return HTS221_ERROR;
    H_Cal_InitStruct.H0_T0_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];

    if(HTS221_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_H1_T0_OUT_L), buffer, 2))
        return HTS221_ERROR;
    H_Cal_InitStruct.H1_T0_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];

    return HTS221_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Temperature_Calibration_Get(I2C_TypeDef *I2Cx)
{
    u8 buffer[4] = {0, 0, 0, 0};
    uint8_t temp;
    if(HTS221_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_T0_DEGC_X8), buffer, 2))
        return HTS221_ERROR;
    if(HTS221_Reg_Read(I2Cx, HTS221_T1_T0_MSB, &temp, 1))
        return HTS221_ERROR;
    T_Cal_InitStruct.T0_degC_MSB = (int16_t)(temp & 0x03);
    T_Cal_InitStruct.T1_degC_MSB = (int16_t)((temp & 0x0C) >> 2);
    T_Cal_InitStruct.T0_degC = ((T_Cal_InitStruct.T0_degC_MSB << 8) | buffer[0]) >> 3;
    T_Cal_InitStruct.T1_degC = ((T_Cal_InitStruct.T1_degC_MSB << 8) | buffer[1]) >> 3;

    if(HTS221_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_H0_T0_OUT_L), buffer, 4))
        return HTS221_ERROR;
    T_Cal_InitStruct.T0_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];
    T_Cal_InitStruct.T1_OUT = (uint16_t)(buffer[3] << 8) | (uint16_t)buffer[2];

    return HTS221_OK;
}

////////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_HUMIDITY_OUT_Get(I2C_TypeDef *I2Cx, int16_t *H_T_OUT)
{
    u8 buffer[2] = {0, 0};
    // Read HUMIDITY_OUT_L and HUMIDITY_OUT_H continuously
    if(HTS221_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_HUMIDITY_OUT_L), buffer, 2))
        return HTS221_ERROR;
    *H_T_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];
    return HTS221_OK;
}

////////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_TEMPERATURE_OUT_Get(I2C_TypeDef *I2Cx, int16_t *T_OUT)
{
    u8 buffer[2] = {0, 0};
    // Read TEMP_OUT_L and TEMP_OUT_H continuously
    if(HTS221_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_TEMP_OUT_L), buffer, 2))
        return HTS221_ERROR;
    *T_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];
    return HTS221_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Humidity_Calculation(I2C_TypeDef *I2Cx, uint16_t *value)
{
    int16_t H_T_OUT = 0x00;
    // Read HUMIDITY_OUT registers
    if(HTS221_HUMIDITY_OUT_Get(I2Cx, &H_T_OUT))
        return HTS221_ERROR;

    int32_t temp;
    temp = ((int32_t)(H_T_OUT - H_Cal_InitStruct.H0_T0_OUT)) * ((int32_t)(H_Cal_InitStruct.H1_rH - H_Cal_InitStruct.H0_rH) * 10);
    *value = (uint16_t)((temp / (H_Cal_InitStruct.H1_T0_OUT - H_Cal_InitStruct.H0_T0_OUT)) + H_Cal_InitStruct.H0_rH * 10);
    if(*value > 1000){
        *value = 1000;
    }
    printf("Get Humidity value : %d.%d %% \n", *value / 10, *value % 10);
    return HTS221_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t *value)
{
    int16_t T_OUT = 0x00;
    // Read TEMP_OUT registers
    if(HTS221_TEMPERATURE_OUT_Get(I2Cx, &T_OUT))
        return HTS221_ERROR;

    int32_t temp;
    temp = ((int32_t)(T_OUT - T_Cal_InitStruct.T0_OUT)) * (((int32_t)(T_Cal_InitStruct.T1_degC - T_Cal_InitStruct.T0_degC) * 10));
    *value = (temp / (T_Cal_InitStruct.T1_OUT - T_Cal_InitStruct.T0_OUT)) + T_Cal_InitStruct.T0_degC * 10;
    printf("Get Temperature value : %d.%d C \n", *value / 10, *value % 10);
    return HTS221_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Calculation(I2C_TypeDef *I2Cx, uint16_t *h_value, int16_t *t_value, bool oneshot)
{
    if(oneshot){
        u8 one_shot = HTS221_CR2_ONE_SHOT;
        // oneshot trigger
        if(HTS221_Reg_Write(I2Cx, HTS221_CTRL_REG2, &one_shot, 1))
            return HTS221_ERROR;
    }
    u8 buffer = 0x00;
    if(HTS221_Reg_Read(I2Cx, HTS221_STATUS_REG, &buffer, 1))
        return HTS221_ERROR;
    // wait for humidity data available
    if(buffer && HTS221_STATUS_H_DA != 0){
        HTS221_Humidity_Calculation(I2Cx, h_value);
    }
    // wait for temp data available
    if(buffer && HTS221_STATUS_T_DA != 0){
        HTS221_Temperature_Calculation(I2Cx, t_value);
    }
    return HTS221_OK;
}