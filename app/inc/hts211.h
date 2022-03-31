////////////////////////////////////////////////////////////////////////////////
#ifndef __HTS211_H_
#define __HTS211_H_
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#ifdef _HTS211_C_

// Read or Write registers continuously by setting register address MSB high
#define CONTINUOUS_ADDRESS(REG_ADDR)    REG_ADDR | 0x80

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

// HTS211 Address
#define SLAVE_ADDRESS                   0x5F
#define SLAVE_ADDRESS_WRITE             SLAVE_ADDRESS << 1
#define SLAVE_ADDRESS_READ              (SLAVE_ADDRESS << 1) | 0x01

// HTS211 Device ID
#define DEVICE_ID_WHO_AM_I              0xBC

// AV_CONF
#define HTS211_AC_AVGH_Pos              (0)
#define HTS211_AC_AVGH_4                (0x00U << HTS211_AC_AVGH_Pos)
#define HTS211_AC_AVGH_8                (0x01U << HTS211_AC_AVGH_Pos)
#define HTS211_AC_AVGH_16               (0x02U << HTS211_AC_AVGH_Pos)
#define HTS211_AC_AVGH_32               (0x03U << HTS211_AC_AVGH_Pos)
#define HTS211_AC_AVGH_64               (0x04U << HTS211_AC_AVGH_Pos)
#define HTS211_AC_AVGH_128              (0x05U << HTS211_AC_AVGH_Pos)
#define HTS211_AC_AVGH_256              (0x06U << HTS211_AC_AVGH_Pos)
#define HTS211_AC_AVGH_512              (0x07U << HTS211_AC_AVGH_Pos)
#define HTS211_AC_AVGT_Pos              (3)
#define HTS211_AC_AVGT_2                (0x00U << HTS211_AC_AVGT_Pos)
#define HTS211_AC_AVGT_4                (0x01U << HTS211_AC_AVGT_Pos)
#define HTS211_AC_AVGT_8                (0x02U << HTS211_AC_AVGT_Pos)
#define HTS211_AC_AVGT_16               (0x03U << HTS211_AC_AVGT_Pos)
#define HTS211_AC_AVGT_32               (0x04U << HTS211_AC_AVGT_Pos)
#define HTS211_AC_AVGT_64               (0x05U << HTS211_AC_AVGT_Pos)
#define HTS211_AC_AVGT_128              (0x06U << HTS211_AC_AVGT_Pos)
#define HTS211_AC_AVGT_256              (0x07U << HTS211_AC_AVGT_Pos)

// CTRL_REG1
#define HTS211_CR1_ODR0_Pos             (0)
#define HTS211_CR1_ODR0                 (0x01U << HTS211_CR1_ODR0_Pos)
#define HTS211_CR1_ODR1_Pos             (1)
#define HTS211_CR1_ODR1                 (0x01U << HTS211_CR1_ODR1_Pos)
#define HTS211_CR1_BDU_Pos              (2)
#define HTS211_CR1_BDU                  (0x01U << HTS211_CR1_BDU_Pos)
#define HTS211_CR1_PD_Pos               (7)
#define HTS211_CR1_PD                   (0x01U << HTS211_CR1_PD_Pos)

// CTRL_REG2
#define HTS211_CR2_ONE_SHOT_Pos         (0)
#define HTS211_CR2_ONE_SHOT             (0x01U << HTS211_CR2_ONE_SHOT_Pos)
#define HTS211_CR2_HEATER_Pos           (1)
#define HTS211_CR2_HEATER               (0x01U << HTS211_CR2_HEATER_Pos)
#define HTS211_CR2_BOOT_Pos             (7)
#define HTS211_CR2_BOOT                 (0x01U << HTS211_CR2_BOOT_Pos)

// CTRL_REG3
#define HTS211_CR3_DRDY_Pos             (2)
#define HTS211_CR3_DRDY                 (0x01U << HTS211_CR3_DRDY_Pos)
#define HTS211_CR3_PP_OD_Pos            (6)
#define HTS211_CR3_PP_OD                (0x01U << HTS211_CR3_PP_OD_Pos)
#define HTS211_CR3_DRDY_H_L_Pos         (7)
#define HTS211_CR3_DRDY_H_L             (0x01U << HTS211_CR3_DRDY_H_L_Pos)

#endif

typedef struct
{
    int16_t     H0_rH;
    int16_t     H1_rH;
    int16_t     H0_T0_OUT;
    int16_t     H1_T0_OUT;
} HTS211_H_Cal_Typedef;
HTS211_H_Cal_Typedef H_Cal_InitStruct; 

typedef struct
{
    int16_t     T0_degC;
    int16_t     T1_degC;
    int16_t     T0_degC_MSB;
    int16_t     T1_degC_MSB;
    int16_t     T0_OUT;
    int16_t     T1_OUT;
} HTS211_T_Cal_Typedef;
HTS211_T_Cal_Typedef T_Cal_InitStruct; 

typedef enum {
    HTS221_OK       = (uint8_t)0,
    HTS221_ERROR    = !HTS221_OK
} HTS221_Error_Typedef;

typedef enum {
    HTS221_DISABLE  = (uint8_t)0,
    HTS221_ENABLE   = !HTS221_DISABLE
} HTS221_State_Typedef;

////////////////////////////////////////////////////////////////////////////////
// HTS221_Error_Typedef HTS211_Init(I2C_TypeDef *I2Cx);
// void HTS211_Reg_Read(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt);
// u8 HTS211_Temprature_Get(I2C_TypeDef *I2Cx);
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
