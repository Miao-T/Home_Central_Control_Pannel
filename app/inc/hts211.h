////////////////////////////////////////////////////////////////////////////////
#ifndef __HTS211_H_
#define __HTS211_H_
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
#ifdef __HTS211_C_

enum{WR, RD};

// HTS211 register subAddr
#define HTS211_WHO_AM_I                 0x0F
#define HTS211_AV_CONF                  0x10
#define HTS211_CTRL_REG1                0x20
#define HTS211_CTRL_REG2                0x21
#define HTS211_CTRL_REG3                0x22
#define HTS211_STATUS_REG               0x27
#define HTS211_HUMIDITY_OUT_L           0x28
#define HTS211_HUMIDITY_OUT_H           0x29
#define HTS211_TEMP_OUT_L               0x2A
#define HTS211_TEMP_OUT_H               0x2B

#define HTS221_H0_RH_X2                 0x30
#define HTS221_H1_RH_X2                 0x31
#define HTS221_T0_DEGC_X8               0x32
#define HTS221_T1_DEGC_X8               0x33
#define HTS221_T1_T0_MSB                0x35
#define HTS221_H0_T0_OUT_L              0x36
#define HTS221_H0_T0_OUT_H              0x37
#define HTS221_H1_T0_OUT_L              0x3A
#define HTS221_H1_T0_OUT_H              0x3B
#define HTS221_T0_OUT_L                 0x3C
#define HTS221_T0_OUT_H                 0x3D
#define HTS221_T1_OUT_L                 0x3E
#define HTS221_T1_OUT_H                 0x3F

#define SLAVE_ADDRESS                   0x5F
#define WHO_AM_I                        0xBC
#define SLAVE_ADDRESS_WRITE             (SLAVE_ADDRESS << 1) | WR
#define SLAVE_ADDRESS_READ              (SLAVE_ADDRESS << 1) | RD

#endif

////////////////////////////////////////////////////////////////////////////////

void HTS211_Init(I2C_TypeDef *I2Cx);
void HTS211_humidity_get();
void HTS211_temperature_get();
void HTS211_Read_WHO_AM_I(I2C_TypeDef *I2Cx);
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
