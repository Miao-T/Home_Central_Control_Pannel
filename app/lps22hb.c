////////////////////////////////////////////////////////////////////////////////
#define _LPS22HB_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "HAL_i2c.h"
#include "lps22hb.h"
#include "i2c.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_Reg_Write(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt)
{
    Sensor_Write(I2Cx, SLAVE_ADDRESS_WRITE, regAddr, ptr, cnt);

    return LPS22HB_OK;
}

////////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_Reg_Read(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt)
{
    Sensor_Read(I2Cx, SLAVE_ADDRESS_READ, regAddr, ptr, cnt);

    return LPS22HB_OK;
}

////////////////////////////////////////////////////////////////////////////////
u8 LPS22HB_WHO_AM_I_Get(I2C_TypeDef *I2Cx)
{
    u8 device_id;
    if(LPS22HB_Reg_Read(I2Cx, LPS22HB_WHO_AM_I, &device_id, 1))
        return LPS22HB_ERROR;
    return device_id;
}

////////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_Reset_Software_BOOT(I2C_TypeDef *I2Cx)
{
    u8 resetValue = LPS22HB_CR2_SWRESET | LPS22HB_CR2_BOOT;
    if(LPS22HB_Reg_Read(I2Cx, LPS22HB_CTRL_REG2, &resetValue, 1))
        return LPS22HB_ERROR;
    return LPS22HB_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_Init(I2C_TypeDef *I2Cx, bool oneshot, u8 frequency)
{
    u8 buffer[3] = {0, 0, 0};

    if(LPS22HB_WHO_AM_I_Get(I2Cx) == DEVICE_ID_WHO_AM_I){
        if(oneshot){
            // Set BDU / LPFP and ODR in CTRL_REG1
            buffer[0] = LPS22HB_CR1_BDU | LPS22HB_CR1_EN_LPFP | LPS22HB_CR1_ODR_ONE_SHOT;
        }else{
            // Set BDU / LPFP and ODR in CTRL_REG1
            buffer[0] = LPS22HB_CR1_BDU | LPS22HB_CR1_EN_LPFP | (frequency << LPS22HB_CR1_ODR_POS);
        }
        // Set AUTOINC in CTRL_REG2
        buffer[1] = LPS22HB_CR2_IF_ADD_INC ;
        buffer[2] = 0x00;                                                       // Clear CTRL_REG3
        if(LPS22HB_Reg_Write(I2Cx, LPS22HB_CTRL_REG1, buffer, 3))
            return LPS22HB_ERROR;

        return LPS22HB_OK;
    }else{
        return LPS22HB_ERROR;
    }
}

//////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_DeInit(I2C_TypeDef *I2Cx)
{
    u8 buffer[3] = {0, 0, 0};

    if(LPS22HB_Reg_Write(I2Cx, LPS22HB_CTRL_REG1, buffer, 3))
        return LPS22HB_ERROR;

    return LPS22HB_OK;
}

////////////////////////////////////////////////////////////////////////////////
void LPS22HB_FIFO_Configure(LPS22HB_FIFO_Mode_Typedef mode, bool fth, u8 watermark)
{
    LPS22HB_FIFO_InitStruct.fifo_en = ENABLE;
    LPS22HB_FIFO_InitStruct.fifo_mode = mode;
    LPS22HB_FIFO_InitStruct.stop_on_fth = fth;
    LPS22HB_FIFO_InitStruct.watermark = watermark <= 0x1F ? watermark : 0x1F;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set FIFO mode and watermark level.
/// @param  I2Cn: where n can be 1 or 2 to select the I2C peripheral.
/// @param  fifoMode: specifies the fifo modes.
///   This parameter can be one of the following values:
/// @arg  LPS22HB_FIFO_MODE_BYPASS : 
/// @arg  LPS22HB_FIFO_MODE_FIFO : 
/// @arg  LPS22HB_FIFO_MODE_CONTINUOUS : 
/// @arg  LPS22HB_FIFO_MODE_BYPASS_2_FIFO : 
/// @arg  LPS22HB_FIFO_MODE_BYPASS_2_CONTINUOUS : 
/// @arg  LPS22HB_FIFO_MODE_CONTINUOUS_2_FIFO : 
/// @param  wtm: Enable the use of the FIFO watermark level.
///   This parameter can be: ENABLE or DISABLE.
/// @param  watermark: FIFO watermark level.
/// @retval The state of whether error happened when I2C operates HPS22HH.
////////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_FIFO_Init(I2C_TypeDef *I2Cx, LPS22HB_FIFO_Mode_Typedef fifoMode, bool fth, u8 watermark)
{
    uint8_t tmp = 0x00;
    if(LPS22HB_Reg_Read(I2Cx, LPS22HB_CTRL_REG2, &tmp, 1))
        return LPS22HB_ERROR;
    tmp |= LPS22HB_CR2_FIFO_EN | fth << LPS22HB_CR2_STOP_ON_FTH_POS;
    if(LPS22HB_Reg_Write(I2Cx, LPS22HB_CTRL_REG2, &tmp, 1))
        return LPS22HB_ERROR;

    tmp = 0x00;
    if(LPS22HB_Reg_Write(I2Cx, LPS22HB_FIFO_CTRL, &tmp, 1))
        return LPS22HB_ERROR;

    if(fth){
        tmp = (watermark <= 0x1F) ? (watermark << LPS22HB_FC_WTM_POS | fifoMode << LPS22HB_FC_MODE_POS) :
                                       (0x1F << LPS22HB_FC_WTM_POS | fifoMode << LPS22HB_FC_MODE_POS);
    }else{
        tmp = fifoMode << LPS22HB_FC_MODE_POS;
    }
    // Configure FIFO mode and watermark level
    if(LPS22HB_Reg_Write(I2Cx, LPS22HB_FIFO_CTRL, &tmp, 1))
        return LPS22HB_ERROR;

    return LPS22HB_OK;
}

////////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_FIFO_Restart(I2C_TypeDef *I2Cx, LPS22HB_FIFO_Mode_Typedef fifoMode, bool fth, u8 watermark)
{
    uint8_t tmp = 0x00;
    if(LPS22HB_Reg_Write(I2Cx, LPS22HB_FIFO_CTRL, &tmp, 1))
        return LPS22HB_ERROR;

    if(fth){
        tmp = (watermark <= 0x1F) ? (watermark << LPS22HB_FC_WTM_POS | fifoMode << LPS22HB_FC_MODE_POS) :
                                       (0x1F << LPS22HB_FC_WTM_POS | fifoMode << LPS22HB_FC_MODE_POS);
    }else{
        tmp = fifoMode << LPS22HB_FC_MODE_POS;
    }
    // Configure FIFO mode and watermark level
    if(LPS22HB_Reg_Write(I2Cx, LPS22HB_FIFO_CTRL, &tmp, 1))
        return LPS22HB_ERROR;

    return LPS22HB_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable or disable the specified HPS22HH interrupts.
/// @param  I2Cn: where n can be 1 or 2 to select the I2C peripheral.
/// @param  it: specifies the HPS22HH interrupt sources to be enabled
///             or disabled.
///   This parameter can be one of the following values:
/// @arg  LPS22HB_ITC_PHE : Enable interrupr request on pressure value higher
///                         than preset threshold.
/// @arg  LPS22HB_ITC_PLE : Enable interrupr request on pressure value lower
///                         than preset threshold.
/// @param  state: new state of the specified HPS22HH interrupts.
///   This parameter can be: ENABLE or DISABLE.
/// @retval The state of whether error happened when I2C operates HPS22HH.
////////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_Interrupt_Enable(I2C_TypeDef *I2Cx, u8 it, u8 mode, u8 *THS_P)
{
    u8 buffer;
    buffer = it | LPS22HB_ITC_DIFF_EN | LPS22HB_ITC_LIR | mode;
    if(LPS22HB_Reg_Write(I2Cx, LPS22HB_INTERRUPT_CFG, &buffer, 1))
        return LPS22HB_ERROR;

    if(LPS22HB_Reg_Write(I2Cx, LPS22HB_THS_P_L, THS_P, 2))
        return LPS22HB_ERROR;

    return LPS22HB_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks whether the specified HPS22HH interrupt flag is set or not.
/// @param  I2Cn: where n can be 1 or 2 to select the I2C peripheral.
/// @param  flag: specifies the flag to check.
///   This parameter can be one of the following values:
/// @arg  LPS22HB_ITS_PH_FLAG : High differential pressure flag.
/// @arg  LPS22HB_ITS_PL_FLAG : Low differential pressure flag.
/// @arg  LPS22HB_ITS_IA_FLAG : High or Low differential pressure flag.
/// @retval The new state of HTS22HH_Interrupt_Flag (SET or RESET).
////////////////////////////////////////////////////////////////////////////////
FlagStatus LPS22HB_INT_SOURCE_Get(I2C_TypeDef *I2Cx, u8 flag)
{
    u8 buffer = 0;
    LPS22HB_Reg_Read(I2Cx, LPS22HB_INT_SOURCE, &buffer, 1);
    return ((buffer & flag) ? SET : RESET);
}

//////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_PRESS_OUT_Get(I2C_TypeDef *I2Cx, int32_t *P_OUT)
{
    uint8_t buffer[3] = {0, 0, 0};
    // read from standard registers
    if(LPS22HB_Reg_Read(I2Cx, LPS22HB_PRESS_OUT_XL, buffer, 3))
        return LPS22HB_ERROR;

    *P_OUT = (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
    return LPS22HB_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_TEMP_OUT_Get(I2C_TypeDef *I2Cx, int16_t *T_OUT)
{
    uint8_t buffer[2] = {0, 0};
    // read from normal registers
    if(LPS22HB_Reg_Read(I2Cx, LPS22HB_TEMP_OUT_L, buffer, 2))
        return LPS22HB_ERROR;

    *T_OUT = (buffer[1] << 8) | buffer[0];
    return LPS22HB_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_Pressure_Calculation(I2C_TypeDef *I2Cx, int32_t *value)
{
    int32_t P_OUT = 0x00;
    if(LPS22HB_PRESS_OUT_Get(I2Cx, &P_OUT))
        return LPS22HB_ERROR;

    *value = (P_OUT * 10) / 4096;
    printf("Get Pressure value : %d.%d hPa \n", *value / 10, abs(*value % 10));
    return LPS22HB_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t *value)
{
    int16_t T_OUT = 0x00;
    if(LPS22HB_TEMP_OUT_Get(I2Cx, &T_OUT))
        return LPS22HB_ERROR;

    *value = T_OUT / 10;
    printf("Get Temperature value : %d.%d C \n", *value / 10, abs(*value % 10));
    return LPS22HB_OK;
}

// //////////////////////////////////////////////////////////////////////////////
// LPS22HB_Error_Typedef LPS22HB_FIFO_Rolling_Get(I2C_TypeDef *I2Cx, int32_t *p_value, int16_t *t_value, LPS22HB_FIFO_Typedef fifo)
// {
//     if(fifo.fifo_en){
//         uint8_t *buffer;
//         u16 number = fifo.stop_on_fth ? (fifo.watermark + 1) * 5 : 160;
//         if(LPS22HB_Reg_Read(I2Cx, LPS22HB_PRESS_OUT_XL, buffer, number))
//             return LPS22HB_ERROR;

//         uint8_t data[5] = {0, 0, 0, 0, 0};
//         for(int i = 0; i < number; i++){
//             switch(i % 5){
//                 case 0: data[0] = buffer[i]; break;
//                 case 1: data[1] = buffer[i]; break;
//                 case 2: data[2] = buffer[i]; break;
//                 case 3: data[3] = buffer[i]; break;
//                 case 4: {
//                     data[4] = buffer[i];
//                     *p_value = ((data[2] << 16) | (data[1] << 8) | data[0]) * 10 / 4096;
//                     *t_value = ((data[4] << 8) | data[3]) / 10;
//                     printf("Get Pressure value : %d.%d hPa \n", *p_value / 10, abs(*p_value % 10));
//                     printf("Get Temperature value : %d.%d C \n", *t_value / 10, abs(*t_value % 10));
//                     break;
//                 }
//                 default:break;
//             }
//         }
//     }

//     return LPS22HB_OK;
// }

//////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_FIFO_Rolling_Get(I2C_TypeDef *I2Cx, int32_t *p_value, int16_t *t_value)
{
        uint8_t buffer[160];
        if(LPS22HB_Reg_Read(I2Cx, LPS22HB_PRESS_OUT_XL, buffer, 160))
            return LPS22HB_ERROR;

        uint8_t data[5] = {0, 0, 0, 0, 0};
        for(int i = 0; i < 160; i++){
            switch(i % 5){
                case 0: data[0] = buffer[i]; break;
                case 1: data[1] = buffer[i]; break;
                case 2: data[2] = buffer[i]; break;
                case 3: data[3] = buffer[i]; break;
                case 4: {
                    data[4] = buffer[i];
                    *p_value = ((data[2] << 16) | (data[1] << 8) | data[0]) * 10 / 4096;
                    *t_value = ((data[4] << 8) | data[3]) / 10;
                    printf("Get Pressure value : %d.%d hPa \n", *p_value / 10, abs(*p_value % 10));
                    printf("Get Temperature value : %d.%d C \n", *t_value / 10, abs(*t_value % 10));
                    break;
                }
                default:break;
            }
        }

    return LPS22HB_OK;
}

//////////////////////////////////////////////////////////////////////////////
LPS22HB_Error_Typedef LPS22HB_Calculation(I2C_TypeDef *I2Cx, int32_t *p_value, int16_t *t_value, bool oneshot, bool fifo, bool fth)
{
    if(oneshot){
        uint8_t tmp = 0x00;
        if(LPS22HB_Reg_Read(I2Cx, LPS22HB_CTRL_REG2, &tmp, 1))
            return LPS22HB_ERROR;
        tmp |= LPS22HB_CR2_ONE_SHOT;
        if(LPS22HB_Reg_Write(I2Cx, LPS22HB_CTRL_REG2, &tmp, 1))
            return LPS22HB_ERROR;
    }

    if(!fifo){
        u8 buffer = 0x00;
        if(LPS22HB_Reg_Read(I2Cx, LPS22HB_STATUS, &buffer, 1))
            return LPS22HB_ERROR;
        if(buffer & LPS22HB_STATUS_P_DA != 0){
            LPS22HB_Pressure_Calculation(I2Cx, p_value);
            LPS22HB_Altitude_Calculation(p_value);
        }
        if(buffer & LPS22HB_STATUS_T_DA != 0){
            LPS22HB_Temperature_Calculation(I2Cx, t_value);
        }
        return LPS22HB_OK;
    }else{
        u8 buffer = 0x00;
        if(LPS22HB_Reg_Read(I2Cx, LPS22HB_FIFO_STATUS, &buffer, 1))
            return LPS22HB_ERROR;
        // printf("fifo status is %d \n", buffer);
        if(fth){
            if((buffer & LPS22HB_FSTA_FTH_FIFO) != 0){
                LPS22HB_FIFO_Rolling_Get(I2Cx, p_value, t_value);
                LPS22HB_FIFO_Restart(I2Cx, LPS22HB_FIFO_MODE_FIFO, fth, 0x0A);
            }
        }else{
            if((buffer & LPS22HB_FSTA_OVR) != 0){
                LPS22HB_FIFO_Rolling_Get(I2Cx, p_value, t_value);
                LPS22HB_FIFO_Restart(I2Cx, LPS22HB_FIFO_MODE_FIFO, fth, 0x00);
            }
        }

        return LPS22HB_OK;
    }
}

//////////////////////////////////////////////////////////////////////////////
void LPS22HB_Altitude_Calculation(int32_t *p_value)
{
    float altitude = 44300 * (1 - pow(*p_value / 10132.5, 0.19));
    printf("Get Altitude value : %.1f M \n", altitude);

}