////////////////////////////////////////////////////////////////////////////////
#ifndef __HTS221_H_
#define __HTS221_H_
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    HTS221_OK       = (uint8_t)0,
    HTS221_ERROR    = !HTS221_OK
} HTS221_Error_Typedef;

typedef enum {
    HTS221_DISABLE  = (uint8_t)0,
    HTS221_ENABLE   = !HTS221_DISABLE
} HTS221_State_Typedef;

////////////////////////////////////////////////////////////////////////////////
/// @defgroup HTS221 Output data rate
/// @{
#define HTS221_OUTPUT_DATA_ONESHOT      0x00
#define HTS221_OUTPUT_DATA_RATE_1       0x01
#define HTS221_OUTPUT_DATA_RATE_7       0x02
#define HTS221_OUTPUT_DATA_RATE_12_5    0x03
/// @}

////////////////////////////////////////////////////////////////////////////////
#ifdef _HTS221_C_

// Read or Write registers continuously by setting register address MSB high
#define CONTINUOUS_ADDRESS(REG_ADDR)    REG_ADDR | 0x80

// HTS221 register subAddr
#define HTS221_WHO_AM_I                 0x0F
#define HTS221_AV_CONF                  0x10
#define HTS221_CTRL_REG1                0x20
#define HTS221_CTRL_REG2                0x21
#define HTS221_CTRL_REG3                0x22
#define HTS221_STATUS_REG               0x27
#define HTS221_HUMIDITY_OUT_L           0x28
#define HTS221_HUMIDITY_OUT_H           0x29
#define HTS221_TEMP_OUT_L               0x2A
#define HTS221_TEMP_OUT_H               0x2B

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

// HTS221 Address
#define SLAVE_ADDRESS                   0x5F
#define SLAVE_ADDRESS_WRITE             SLAVE_ADDRESS << 1
#define SLAVE_ADDRESS_READ              (SLAVE_ADDRESS << 1) | 0x01

// HTS221 Device ID
#define DEVICE_ID_WHO_AM_I              0xBC

// HTS221_AV_CONF
#define HTS221_AC_AVGH_POS              (0)
#define HTS221_AC_AVGH_4                (0x00U << HTS221_AC_AVGH_POS)
#define HTS221_AC_AVGH_8                (0x01U << HTS221_AC_AVGH_POS)
#define HTS221_AC_AVGH_16               (0x02U << HTS221_AC_AVGH_POS)
#define HTS221_AC_AVGH_32               (0x03U << HTS221_AC_AVGH_POS)
#define HTS221_AC_AVGH_64               (0x04U << HTS221_AC_AVGH_POS)
#define HTS221_AC_AVGH_128              (0x05U << HTS221_AC_AVGH_POS)
#define HTS221_AC_AVGH_256              (0x06U << HTS221_AC_AVGH_POS)
#define HTS221_AC_AVGH_512              (0x07U << HTS221_AC_AVGH_POS)
#define HTS221_AC_AVGT_POS              (3)
#define HTS221_AC_AVGT_2                (0x00U << HTS221_AC_AVGT_POS)
#define HTS221_AC_AVGT_4                (0x01U << HTS221_AC_AVGT_POS)
#define HTS221_AC_AVGT_8                (0x02U << HTS221_AC_AVGT_POS)
#define HTS221_AC_AVGT_16               (0x03U << HTS221_AC_AVGT_POS)
#define HTS221_AC_AVGT_32               (0x04U << HTS221_AC_AVGT_POS)
#define HTS221_AC_AVGT_64               (0x05U << HTS221_AC_AVGT_POS)
#define HTS221_AC_AVGT_128              (0x06U << HTS221_AC_AVGT_POS)
#define HTS221_AC_AVGT_256              (0x07U << HTS221_AC_AVGT_POS)

// HTS221_CTRL_REG1
#define HTS221_CR1_ODR_POS              (0)
#define HTS221_CR1_ODR_ONESHOT          (0x00U << HTS221_CR1_ODR_POS)
#define HTS221_CR1_ODR_1                (0x01U << HTS221_CR1_ODR_POS)
#define HTS221_CR1_ODR_7                (0x02U << HTS221_CR1_ODR_POS)
#define HTS221_CR1_ODR_12_5             (0x03U << HTS221_CR1_ODR_POS)
#define HTS221_CR1_BDU_POS              (2)
#define HTS221_CR1_BDU                  (0x01U << HTS221_CR1_BDU_POS)
#define HTS221_CR1_PD_POS               (7)
#define HTS221_CR1_PD                   (0x01U << HTS221_CR1_PD_POS)

// HTS221_CTRL_REG2
#define HTS221_CR2_ONE_SHOT_POS         (0)
#define HTS221_CR2_ONE_SHOT             (0x01U << HTS221_CR2_ONE_SHOT_POS)
#define HTS221_CR2_HEATER_POS           (1)
#define HTS221_CR2_HEATER               (0x01U << HTS221_CR2_HEATER_POS)
#define HTS221_CR2_BOOT_POS             (7)
#define HTS221_CR2_BOOT                 (0x01U << HTS221_CR2_BOOT_POS)

// HTS221_CTRL_REG3
#define HTS221_CR3_DRDY_POS             (2)
#define HTS221_CR3_DRDY                 (0x01U << HTS221_CR3_DRDY_POS)
#define HTS221_CR3_PP_OD_POS            (6)
#define HTS221_CR3_PP_OD                (0x01U << HTS221_CR3_PP_OD_POS)
#define HTS221_CR3_DRDY_H_L_POS         (7)
#define HTS221_CR3_DRDY_H_L             (0x01U << HTS221_CR3_DRDY_H_L_POS)

// HTS221_STATUS_REG
#define HTS221_STATUS_T_DA_POS          (0)
#define HTS221_STATUS_T_DA              (0x01U << HTS221_STATUS_T_DA_POS)
#define HTS221_STATUS_H_DA_POS          (1)
#define HTS221_STATUS_H_DA              (0x01U << HTS221_STATUS_H_DA_POS)

typedef struct
{
    int16_t     H0_rH;
    int16_t     H1_rH;
    int16_t     H0_T0_OUT;
    int16_t     H1_T0_OUT;
} HTS221_H_Cal_Typedef;
HTS221_H_Cal_Typedef H_Cal_InitStruct;

typedef struct
{
    int16_t     T0_degC;
    int16_t     T1_degC;
    int16_t     T0_degC_MSB;
    int16_t     T1_degC_MSB;
    int16_t     T0_OUT;
    int16_t     T1_OUT;
} HTS221_T_Cal_Typedef;
HTS221_T_Cal_Typedef T_Cal_InitStruct;

#endif


////////////////////////////////////////////////////////////////////////////////
HTS221_Error_Typedef HTS221_Reg_Write(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt);
HTS221_Error_Typedef HTS221_Reg_Read(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt);
u8 HTS221_WHO_AM_I_Get(I2C_TypeDef *I2Cx);
HTS221_Error_Typedef HTS221_Init(I2C_TypeDef *I2Cx, bool oneshot, u8 frequency);
HTS221_Error_Typedef HTS221_DeInit(I2C_TypeDef *I2Cx);
HTS221_Error_Typedef HTS221_Humidity_Calibration_Get(I2C_TypeDef *I2Cx);
HTS221_Error_Typedef HTS221_Temperature_Calibration_Get(I2C_TypeDef *I2Cx);
HTS221_Error_Typedef HTS221_HUMIDITY_OUT_Get(I2C_TypeDef *I2Cx, int16_t *H_T_OUT);
HTS221_Error_Typedef HTS221_TEMPERATURE_OUT_Get(I2C_TypeDef *I2Cx, int16_t *T_OUT);
HTS221_Error_Typedef HTS221_Humidity_Calculation(I2C_TypeDef *I2Cx, uint16_t *value);
HTS221_Error_Typedef HTS221_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t *value);
HTS221_Error_Typedef HTS221_Calculation(I2C_TypeDef *I2Cx, uint16_t *h_value, int16_t *t_value, bool oneshot);
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
