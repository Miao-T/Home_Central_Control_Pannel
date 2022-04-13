////////////////////////////////////////////////////////////////////////////////
#ifndef __LPS22HH_H_
#define __LPS22HH_H_
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    LPS22HH_OK       = (uint8_t)0,
    LPS22HH_ERROR    = !LPS22HH_OK
} LPS22HH_Error_Typedef;

typedef enum {
    LPS22HH_DISABLE  = (uint8_t)0,
    LPS22HH_ENABLE   = !LPS22HH_DISABLE
} LPS22HH_State_Typedef;

////////////////////////////////////////////////////////////////////////////////
/// @defgroup HPS22HH Interrupts Definition
/// @{
#define LPS22HH_IT_PHE                  0x01
#define LPS22HH_IT_PLE                  0x02
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup HPS22HH Interrupt Mode Definition
/// @{
#define LPS22HH_IT_MODE_RESETAZ         0x10
#define LPS22HH_IT_MODE_AUTOZERO        0x20
#define LPS22HH_IT_MODE_RESETARP        0x40
#define LPS22HH_IT_MODE_AUTOREFP        0x80
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LPS22HH Statusflags definition
/// @{
#define LPS22HH_ITS_PH_FLAG             0x01
#define LPS22HH_ITS_PL_FLAG             0x02
#define LPS22HH_ITS_IA_FLAG             0x04
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LPS22HH Output data rate
/// @{
#define LPS22HH_OUTPUT_DATA_ONESHOT     0x00
#define LPS22HH_OUTPUT_DATA_RATE_1      0x01
#define LPS22HH_OUTPUT_DATA_RATE_10     0x02
#define LPS22HH_OUTPUT_DATA_RATE_25     0x03
#define LPS22HH_OUTPUT_DATA_RATE_50     0x04
#define LPS22HH_OUTPUT_DATA_RATE_75     0x05
#define LPS22HH_OUTPUT_DATA_RATE_100    0x06
#define LPS22HH_OUTPUT_DATA_RATE_200    0x07
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LPS22HH FIFO mode
/// @{
#define LPS22HH_FIFO_MODE_BYPASS               0x00
#define LPS22HH_FIFO_MODE_FIFO                 0x01
#define LPS22HH_FIFO_MODE_CONTINUOUS           0x02
#define LPS22HH_FIFO_MODE_BYPASS_2_FIFO        0x05
#define LPS22HH_FIFO_MODE_BYPASS_2_CONTINUOUS  0x06
#define LPS22HH_FIFO_MODE_CONTINUOUS_2_FIFO    0x07
/// @}

////////////////////////////////////////////////////////////////////////////////
#ifdef _LPS22HH_C_

// LPS22HH register subAddr
#define LPS22HH_INTERRUPT_CFG           0x0B                                    // Interrupt register
#define LPS22HH_THS_P_L                 0x0C                                    // Pressure threshold registers
#define LPS22HH_THS_P_H                 0x0D                                    // Pressure threshold registers
#define LPS22HH_IF_CTRL                 0x0E                                    // Interface control register
#define LPS22HH_WHO_AM_I                0x0F                                    // Who am I
#define LPS22HH_CTRL_REG1               0x10                                    // Control registers
#define LPS22HH_CTRL_REG2               0x11                                    // Control registers
#define LPS22HH_CTRL_REG3               0x12                                    // Control registers
#define LPS22HH_FIFO_CTRL               0x13                                    // FIFO configuration register
#define LPS22HH_FIFO_WTM                0x14                                    // FIFO watermark level
#define LPS22HH_REF_P_L                 0x15                                    // Reference pressure registers
#define LPS22HH_REF_P_H                 0x16                                    // Reference pressure registers
#define LPS22HH_RPDS_L                  0x18                                    // Pressure offset registers
#define LPS22HH_RPDS_H                  0x19                                    // Pressure offset registers
#define LPS22HH_INT_SOURCE              0x24                                    // Interrupt register
#define LPS22HH_FIFO_STATUS1            0x25                                    // FIFO status registers
#define LPS22HH_FIFO_STATUS2            0x26                                    // FIFO status registers
#define LPS22HH_STATUS                  0x27                                    // Status register
#define LPS22HH_PRESS_OUT_XL            0x28                                    // Pressure output registers
#define LPS22HH_PRESS_OUT_L             0x29                                    // Pressure output registers
#define LPS22HH_PRESS_OUT_H             0x2A                                    // Pressure output registers
#define LPS22HH_TEMP_OUT_L              0x2B                                    // Temperature output registers
#define LPS22HH_TEMP_OUT_H              0x2C                                    // Temperature output registers
#define LPS22HH_FIFO_DATA_OUT_PRESS_XL  0x78                                    // FIFO pressure output registers
#define LPS22HH_FIFO_DATA_OUT_PRESS_L   0x79                                    // FIFO pressure output registers
#define LPS22HH_FIFO_DATA_OUT_PRESS_H   0x7A                                    // FIFO pressure output registers
#define LPS22HH_FIFO_DATA_OUT_TEMP_L    0x7B                                    // FIFO temperature output registers
#define LPS22HH_FIFO_DATA_OUT_TEMP_H    0x7C                                    // FIFO temperature output registers

// LPS22HH Address
#define SLAVE_ADDRESS_SA0               0x00
#define SLAVE_ADDRESS                   (0x5C | SLAVE_ADDRESS_SA0)              // 0x5C
#define SLAVE_ADDRESS_WRITE             SLAVE_ADDRESS << 1                      // 0xB8
#define SLAVE_ADDRESS_READ              (SLAVE_ADDRESS << 1) | 0x01             // 0xB9

// LPS22HH Device ID
#define DEVICE_ID_WHO_AM_I              0xB3

// LPS22HH_INTERRUPT_CFG
#define LPS22HH_ITC_PHE_POS             (0)
#define LPS22HH_ITC_PHE                 (0x01 << LPS22HH_ITC_PHE_POS)
#define LPS22HH_ITC_PLE_POS             (1)
#define LPS22HH_ITC_PLE                 (0x01 << LPS22HH_ITC_PLE_POS)
#define LPS22HH_ITC_LIR_POS             (2)
#define LPS22HH_ITC_LIR                 (0x01 << LPS22HH_ITC_LIR_POS)
#define LPS22HH_ITC_DIFF_EN_POS         (3)
#define LPS22HH_ITC_DIFF_EN             (0x01 << LPS22HH_ITC_DIFF_EN_POS)
#define LPS22HH_ITC_RESET_AZ_POS        (4)
#define LPS22HH_ITC_RESET_AZ            (0x01 << LPS22HH_ITC_RESET_AZ_POS)
#define LPS22HH_ITC_AUTOZERO_POS        (5)
#define LPS22HH_ITC_AUTOZERO            (0x01 << LPS22HH_ITC_AUTOZERO_POS)
#define LPS22HH_ITC_RESET_ARP_POS       (6)
#define LPS22HH_ITC_RESET_ARP           (0x01 << LPS22HH_ITC_RESET_ARP_POS)
#define LPS22HH_ITC_AUTOREFP_POS        (7)
#define LPS22HH_ITC_AUTOREFP            (0x01 << LPS22HH_ITC_AUTOREFP_POS)

// LPS22HH_IF_CTRL
#define LPS22HH_IFC_I2C_DIS_POS         (0)
#define LPS22HH_IFC_I2C_DIS             (0x01 << LPS22HH_IFC_I2C_DIS_POS)
#define LPS22HH_IFC_I3C_DIS_POS         (1)
#define LPS22HH_IFC_I3C_DIS             (0x01 << LPS22HH_IFC_I3C_DIS_POS)
#define LPS22HH_IFC_PD_DIS_INT_POS      (2)
#define LPS22HH_IFC_PD_DIS_INT          (0x01 << LPS22HH_IFC_PD_DIS_INT_POS)
#define LPS22HH_IFC_SDO_PU_EN_POS       (3)
#define LPS22HH_IFC_SDO_PU_EN           (0x01 << LPS22HH_IFC_SDO_PU_EN_POS)
#define LPS22HH_IFC_SDA_PU_EN_POS       (4)
#define LPS22HH_IFC_SDA_PU_EN           (0x01 << LPS22HH_IFC_SDA_PU_EN_POS)
#define LPS22HH_IFC_INT_EN_I3C_POS      (7)
#define LPS22HH_IFC_INT_EN_I3C          (0x01 << LPS22HH_IFC_INT_EN_I3C_POS)

// LPS22HH_CTRL_REG1
#define LPS22HH_CR1_SIM_POS             (0)
#define LPS22HH_CR1_SIM                 (0x01U << LPS22HH_CR1_SIM_POS)
#define LPS22HH_CR1_BDU_POS             (1)
#define LPS22HH_CR1_BDU                 (0x01U << LPS22HH_CR1_BDU_POS)
#define LPS22HH_CR1_LPFP_CFG_POS        (2)
#define LPS22HH_CR1_LPFP_CFG            (0x01U << LPS22HH_CR1_LPFP_CFG_POS)
#define LPS22HH_CR1_EN_LPFP_POS         (3)
#define LPS22HH_CR1_EN_LPFP             (0x01U << LPS22HH_CR1_EN_LPFP_POS)
#define LPS22HH_CR1_ODR_POS             (4)
#define LPS22HH_CR1_ODR_ONE_SHOT        (0x00U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_1               (0x01U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_10              (0x02U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_25              (0x03U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_50              (0x04U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_75              (0x05U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_100             (0x06U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_200             (0x07U << LPS22HH_CR1_ODR_POS)

// LPS22HH_CTRL_REG2
#define LPS22HH_CR2_ONE_SHOT_POS        (0)
#define LPS22HH_CR2_ONE_SHOT            (0x01U << LPS22HH_CR2_ONE_SHOT_POS)
#define LPS22HH_CR2_LOW_NOISE_EN_POS    (1)
#define LPS22HH_CR2_LOW_NOISE_EN        (0x01U << LPS22HH_CR2_LOW_NOISE_EN_POS)
#define LPS22HH_CR2_SWRESET_POS         (2)
#define LPS22HH_CR2_SWRESET             (0x01U << LPS22HH_CR2_SWRESET_POS)
#define LPS22HH_CR2_IF_ADD_INC_POS      (4)
#define LPS22HH_CR2_IF_ADD_INC          (0x01U << LPS22HH_CR2_IF_ADD_INC_POS)
#define LPS22HH_CR2_PP_OD_POS           (5)
#define LPS22HH_CR2_PP_OD               (0x01U << LPS22HH_CR2_PP_OD_POS)
#define LPS22HH_CR2_INT_H_L_POS         (6)
#define LPS22HH_CR2_INT_H_L             (0x01U << LPS22HH_CR2_INT_H_L_POS)
#define LPS22HH_CR2_BOOT_POS            (7)
#define LPS22HH_CR2_BOOT                (0x01U << LPS22HH_CR2_BOOT_POS)

// LPS22HH_CTRL_REG3
#define LPS22HH_CR3_INT_S_POS           (0)
#define LPS22HH_CR3_INT_S_DATA_SIG      (0x00U << LPS22HH_CR3_INT_S_POS)
#define LPS22HH_CR3_INT_S_PH            (0x01U << LPS22HH_CR3_INT_S_POS)
#define LPS22HH_CR3_INT_S_PL            (0x02U << LPS22HH_CR3_INT_S_POS)
#define LPS22HH_CR3_INT_S_PHL           (0x03U << LPS22HH_CR3_INT_S_POS)
#define LPS22HH_CR3_DRDY_POS            (2)
#define LPS22HH_CR3_DRDY                (0x00U << LPS22HH_CR3_DRDY_POS)
#define LPS22HH_CR3_INT_F_OVR_POS       (3)
#define LPS22HH_CR3_INT_F_OVR           (0x01U << LPS22HH_CR3_INT_F_OVR_POS)
#define LPS22HH_CR3_INT_F_WTM_POS       (4)
#define LPS22HH_CR3_INT_F_WTM           (0x01U << LPS22HH_CR3_INT_F_WTM_POS)
#define LPS22HH_CR3_INT_F_FULL_POS      (5)
#define LPS22HH_CR3_INT_F_FULL          (0x01U << LPS22HH_CR3_INT_F_FULL_POS)

// LPS22HH_FIFO_CTRL
#define LPS22HH_FC_MODE_POS             (0)
#define LPS22HH_FC_BYPASS               (0x00U << LPS22HH_FC_MODE_POS)
#define LPS22HH_FC_FIFO                 (0x01U << LPS22HH_FC_MODE_POS)
#define LPS22HH_FC_CONTINUOUS           (0x02U << LPS22HH_FC_MODE_POS)
#define LPS22HH_FC_BYPASS_2_FIFO        (0x05U << LPS22HH_FC_MODE_POS)
#define LPS22HH_FC_BYPASS_2_CONTINUOUS  (0x06U << LPS22HH_FC_MODE_POS)
#define LPS22HH_FC_CONTINUOUS_2_FIFO    (0x07U << LPS22HH_FC_MODE_POS)
#define LPS22HH_FC_STOP_ON_WTM_POS      (3)
#define LPS22HH_FC_STOP_ON_WTM          (0x01 << LPS22HH_FC_STOP_ON_WTM_POS)

// LPS22HH_INT_SOURCE
#define LPS22HH_ITS_PH_POS              (0)
#define LPS22HH_ITS_PH                  (0x01 << LPS22HH_ITS_PH_POS)
#define LPS22HH_ITS_PL_POS              (1)
#define LPS22HH_ITS_PL                  (0x01 << LPS22HH_ITS_PL_POS)
#define LPS22HH_ITS_IA_POS              (2)
#define LPS22HH_ITS_IA                  (0x01 << LPS22HH_ITS_IA_POS)

// LPS22HH_FIFO_STATUS2
#define LPS22HH_FSTA_FULL_IA_POS        (0)
#define LPS22HH_FSTA_FULL_IA            (0x01 << LPS22HH_FSTA_FULL_IA_POS)
#define LPS22HH_FSTA_OVR_IA_POS         (1)
#define LPS22HH_FSTA_OVR_IA             (0x01 << LPS22HH_FSTA_OVR_IA_POS)
#define LPS22HH_FSTA_WTM_IA_POS         (2)
#define LPS22HH_FSTA_WTM_IA             (0x01 << LPS22HH_FSTA_WTM_IA_POS)

// LPS22HH_STATUS
#define LPS22HH_STATUS_P_DA_POS         (0)
#define LPS22HH_STATUS_P_DA             (1 << LPS22HH_STATUS_P_DA_POS)
#define LPS22HH_STATUS_T_DA_POS         (1)
#define LPS22HH_STATUS_T_DA             (1 << LPS22HH_STATUS_T_DA_POS)
#define LPS22HH_STATUS_P_OR_POS         (4)
#define LPS22HH_STATUS_P_OR             (1 << LPS22HH_STATUS_P_OR_POS)
#define LPS22HH_STATUS_T_OR_POS         (5)
#define LPS22HH_STATUS_T_OR             (1 << LPS22HH_STATUS_T_OR_POS)

// typedef struct
// {
//     u8 p_da;
//     u8 t_da;
//     u8 p_or;
//     u8 t_or;
// } LPS22HH_STATUS_Typedef;
// LPS22HH_STATUS_Typedef LPS22HH_STATUS_Initstruct;

#endif
LPS22HH_Error_Typedef LPS22HH_Reg_Write(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt);
LPS22HH_Error_Typedef LPS22HH_Reg_Read(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt);
u8 LPS22HH_WHO_AM_I_Get(I2C_TypeDef *I2Cx);
LPS22HH_Error_Typedef LPS22HH_Software_Reset(I2C_TypeDef *I2Cx);
LPS22HH_Error_Typedef LPS22HH_Init(I2C_TypeDef *I2Cx, bool oneshot, u8 frequency);
LPS22HH_Error_Typedef LPS22HH_DeInit(I2C_TypeDef *I2Cx);
LPS22HH_Error_Typedef LPS22HH_FIFO_Init(I2C_TypeDef *I2Cx, u8 fifoMode, bool wtm, u8 watermark);
LPS22HH_Error_Typedef LPS22HH_Interrupt_Enable(I2C_TypeDef *I2Cx, u8 it, u8 mode, u8 *THS_P);
FlagStatus LPS22HH_INT_SOURCE_Get(I2C_TypeDef *I2Cx, u8 flag);
LPS22HH_Error_Typedef LPS22HH_PRESS_OUT_Get(I2C_TypeDef *I2Cx, int32_t *P_OUT, bool fifo);
LPS22HH_Error_Typedef LPS22HH_TEMP_OUT_Get(I2C_TypeDef *I2Cx, int16_t *T_OUT, bool fifo);
LPS22HH_Error_Typedef LPS22HH_Pressure_Calculation(I2C_TypeDef *I2Cx, int32_t *value, bool fifo);
LPS22HH_Error_Typedef LPS22HH_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t *value, bool fifo);
LPS22HH_Error_Typedef LPS22HH_Calculation(I2C_TypeDef *I2Cx, int32_t *p_value, int16_t *t_value, bool oneshot, bool fifo);
void LPS22HH_Altitude_Calculation(int32_t *p_value);
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
