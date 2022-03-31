////////////////////////////////////////////////////////////////////////////////
#define _HTS211_C_
////////////////////////////////////////////////////////////////////////////////

#include "string.h"

#include "common.h"
#include "HAL_i2c.h"
#include "hts211.h"
#include "i2c.h"

////////////////////////////////////////////////////////////////////////////////
void HTS211_Reg_Write(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt)
{
    Sensor_Write(I2Cx, SLAVE_ADDRESS_READ, regAddr, ptr, cnt);
}

////////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS211_Reg_Read(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt)
{
    Sensor_Read(I2Cx, SLAVE_ADDRESS_READ, regAddr, ptr, cnt);

    return HTS221_OK;
}

////////////////////////////////////////////////////////////////////////////////
u8 HTS211_WHO_AM_I_Get(I2C_TypeDef *I2Cx)
{
    u8 device_id;
    HTS211_Reg_Read(I2Cx, HTS211_WHO_AM_I, &device_id, 1);
    return device_id;
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_AV_CONF_Set(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Write(I2Cx, HTS211_AV_CONF, ptr, 1);
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_AV_CONF_Get(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Read(I2Cx, HTS211_AV_CONF, ptr, 1);
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_CTRL_REG1_Set(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Write(I2Cx, HTS211_CTRL_REG1, ptr, 1);
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_CTRL_REG1_Get(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Read(I2Cx, HTS211_CTRL_REG1, ptr, 1);
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_CTRL_REG2_Set(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Write(I2Cx, HTS211_CTRL_REG2, ptr, 1);
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_CTRL_REG2_Get(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Read(I2Cx, HTS211_CTRL_REG2, ptr, 1);
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_CTRL_REG3_Set(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Write(I2Cx, HTS211_CTRL_REG3, ptr, 1);
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_CTRL_REG3_Get(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Read(I2Cx, HTS211_CTRL_REG3, ptr, 1);
}

////////////////////////////////////////////////////////////////////////////////
void HTS211_STATUS_REG_Get(I2C_TypeDef *I2Cx, u8* ptr)
{
    HTS211_Reg_Read(I2Cx, HTS211_STATUS_REG, ptr, 1);
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS211_Humidity_Calibration_Get(I2C_TypeDef *I2Cx)
{
    u8 buffer[2] = {0, 0};
    if(HTS211_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_H0_RH_X2), buffer, 2))
        return HTS221_ERROR;
    H_Cal_InitStruct.H0_rH = (int16_t)(buffer[0] >> 1);                         // H0_rH = H0_rH_x2 / 2
    H_Cal_InitStruct.H1_rH = (int16_t)(buffer[1] >> 1);                         // H1_rH = H1_rH_x2 / 2

    if(HTS211_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_H0_T0_OUT_L), buffer, 2))
        return HTS221_ERROR;
    H_Cal_InitStruct.H0_T0_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];

    if(HTS211_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_H1_T0_OUT_L), buffer, 2))
        return HTS221_ERROR;
    H_Cal_InitStruct.H1_T0_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];

    return HTS211_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS211_Temperature_Calibration_Get(I2C_TypeDef *I2Cx)
{
    u8 buffer[4] = {0, 0, 0, 0};
    uint8_t temp;
    if(HTS211_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_T0_DEGC_X8), buffer, 2))
        return HTS221_ERROR;
    if(HTS211_Reg_Read(I2Cx, HTS221_T1_T0_MSB, &temp, 1))
        return HTS221_ERROR;
    T_Cal_InitStruct.T0_degC_MSB = (int16_t)(temp & 0x03);
    T_Cal_InitStruct.T1_degC_MSB = (int16_t)((temp & 0x0C) >> 2);
    T_Cal_InitStruct.T0_degC = ((T_Cal_InitStruct.T0_degC_MSB << 8) | buffer[0]) >> 3;
    T_Cal_InitStruct.T1_degC = ((T_Cal_InitStruct.T1_degC_MSB << 8) | buffer[1]) >> 3;

    if(HTS211_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS221_H0_T0_OUT_L), buffer, 4))
        return HTS221_ERROR;
    T_Cal_InitStruct.T0_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];
    T_Cal_InitStruct.T1_OUT = (uint16_t)(buffer[3] << 8) | (uint16_t)buffer[2];

    return HTS211_OK;
}

////////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS211_HUMIDITY_OUT_Get(I2C_TypeDef *I2Cx, int16_t H_T_OUT)
{
    u8 buffer[2] = {0, 0};
    if(HTS211_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS211_HUMIDITY_OUT_L), buffer, 2))
        return HTS221_ERROR;
    H_T_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];
    return HTS211_OK;
}

////////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS211_TEMPERATURE_OUT_Get(I2C_TypeDef *I2Cx, int16_t T_OUT)
{
    u8 buffer[2] = {0, 0};
    if(HTS211_Reg_Read(I2Cx, CONTINUOUS_ADDRESS(HTS211_TEMP_OUT_L), buffer, 2))
        return HTS221_ERROR;
    T_OUT = (uint16_t)(buffer[1] << 8) | (uint16_t)buffer[0];
    return HTS211_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS211_Humidity_Calculation(I2C_TypeDef *I2Cx, uint16_t* value)
{
    int16_t H_T_OUT;
    if(HTS211_HUMIDITY_OUT_Get(I2Cx, H_T_OUT))
        return HTS221_ERROR;

    int32_t temp;
    temp = ((int32_t)(H_T_OUT - H_Cal_InitStruct.H0_T0_OUT)) * ((int32_t)(H_Cal_InitStruct.H1_rH - H_Cal_InitStruct.H0_rH) * 10);
    *value = (uint16_t)((temp / (H_Cal_InitStruct.H1_T0_OUT - H_Cal_InitStruct.H0_T0_OUT)) + H_Cal_InitStruct.H0_rH * 10);
    if(*value > 1000){
        *value = 1000;
    }

    return HTS211_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS211_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t* value)
{
    int16_t T_OUT;
    if(HTS211_TEMPERATURE_OUT_Get(I2Cx, T_OUT))
        return HTS221_ERROR;

    int32_t temp;
    temp = ((int32_t)(T_OUT - T_Cal_InitStruct.T0_OUT)) * (((int32_t)(T_Cal_InitStruct.T1_degC - T_Cal_InitStruct.T0_degC) * 10));
    *value = (temp / (T_Cal_InitStruct.T1_OUT - T_Cal_InitStruct.T0_OUT)) + T_Cal_InitStruct.T0_degC * 10;

    return HTS211_OK;
}

//////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS211_Init(I2C_TypeDef *I2Cx)
{
    u8 temp = 0x00;

    if(HTS211_WHO_AM_I_Get(I2Cx) == DEVICE_ID_WHO_AM_I){
        // Set AVGH and AVGT in AV_CONF
        temp = HTS211_AC_AVGH_32 | HTS211_AC_AVGT_16;
        HTS211_AV_CONF_Set(I2Cx, &temp);

        // Set PD and BDU in CTRL_REG1
        temp = HTS211_CR1_PD | HTS211_CR1_BDU;
        HTS211_CTRL_REG1_Set(I2Cx, &temp);

        // Clear CTRL_REG2
        temp = 0x00;
        HTS211_CTRL_REG1_Set(I2Cx, &temp);

        // Clear CTRL_REG3
        temp = 0x00;
        HTS211_CTRL_REG1_Set(I2Cx, &temp);

        return HTS221_OK;
    }else{
        return HTS221_ERROR;
    }
}