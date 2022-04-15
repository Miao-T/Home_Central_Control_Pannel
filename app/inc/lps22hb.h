////////////////////////////////////////////////////////////////////////////////
#ifndef __LPS22HB_H_
#define __LPS22HB_H_
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    LPS22HB_OK       = (uint8_t)0,
    LPS22HB_ERROR    = !LPS22HB_OK
} LPS22HB_Error_Typedef;

typedef enum {
    LPS22HB_DISABLE  = (uint8_t)0,
    LPS22HB_ENABLE   = !LPS22HB_DISABLE
} LPS22HB_State_Typedef;

typedef enum {
    LPS22HB_FIFO_MODE_BYPASS               = 0x00,
    LPS22HB_FIFO_MODE_FIFO                 = 0x01,
    LPS22HB_FIFO_MODE_STREAM               = 0x02,
    LPS22HB_FIFO_MODE_STREAM_2_FIFO        = 0x03,
    LPS22HB_FIFO_MODE_BYPASS_2_STREAM      = 0x04,
    LPS22HB_FIFO_MODE_DYNAMIC_STREAM       = 0x06,
    LPS22HB_FIFO_MODE_BYPASS_2_FIFO        = 0x07
} LPS22HB_FIFO_Mode_Typedef;

typedef enum {
    LPS22HB_AUTORIFP_MODE_EN               = 0x80,
    LPS22HB_AUTORIFP_MODE_DIS              = 0x40,
    LPS22HB_AUTOZERO_MODE_EN               = 0x20,
    LPS22HB_AUTOZERO_MODE_DIS              = 0x10
} LPS22HB_AUTO_Mode_Typedef;

typedef enum {
    LPS22HB_DATA_SIG                       = 0x00,
    LPS22HB_IT_PHE                         = 0x01,
    LPS22HB_IT_PLE                         = 0x02,
    LPS22HB_IT_PHE_PLE                     = 0x03
} LPS22HB_IT_Mode_Typedef;

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LPS22HB Statusflags definition
/// @{
#define LPS22HB_ITS_PH_FLAG             0x01
#define LPS22HB_ITS_PL_FLAG             0x02
#define LPS22HB_ITS_IA_FLAG             0x04
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LPS22HB Output data rate
/// @{
#define LPS22HB_OUTPUT_DATA_ONESHOT     0x00
#define LPS22HB_OUTPUT_DATA_RATE_1      0x01
#define LPS22HB_OUTPUT_DATA_RATE_10     0x02
#define LPS22HB_OUTPUT_DATA_RATE_25     0x03
#define LPS22HB_OUTPUT_DATA_RATE_50     0x04
#define LPS22HB_OUTPUT_DATA_RATE_75     0x05
#define LPS22HB_OUTPUT_DATA_RATE_100    0x06
#define LPS22HB_OUTPUT_DATA_RATE_200    0x07
/// @}

////////////////////////////////////////////////////////////////////////////////
#ifdef _LPS22HB_C_

// LPS22HB register subAddr
#define LPS22HB_INTERRUPT_CFG           0x0B                                    // Interrupt register
#define LPS22HB_THS_P_L                 0x0C                                    // Pressure threshold registers
#define LPS22HB_THS_P_H                 0x0D                                    // Pressure threshold registers
#define LPS22HB_WHO_AM_I                0x0F                                    // Who am I
#define LPS22HB_CTRL_REG1               0x10                                    // Control registers
#define LPS22HB_CTRL_REG2               0x11                                    // Control registers
#define LPS22HB_CTRL_REG3               0x12                                    // Control registers
#define LPS22HB_FIFO_CTRL               0x14                                    // FIFO configuration register
#define LPS22HB_REF_P_XL                0x15                                    // Reference pressure registers
#define LPS22HB_REF_P_L                 0x16                                    // Reference pressure registers
#define LPS22HB_REF_P_H                 0x17                                    // Reference pressure registers
#define LPS22HB_RPDS_L                  0x18                                    // Pressure offset registers
#define LPS22HB_RPDS_H                  0x19                                    // Pressure offset registers
#define LPS22HB_RES_CONF                0x1A                                    // Resolution register
#define LPS22HB_INT_SOURCE              0x25                                    // Interrupt register
#define LPS22HB_FIFO_STATUS             0x26                                    // FIFO status register
#define LPS22HB_STATUS                  0x27                                    // Status register
#define LPS22HB_PRESS_OUT_XL            0x28                                    // Pressure output registers
#define LPS22HB_PRESS_OUT_L             0x29                                    // Pressure output registers
#define LPS22HB_PRESS_OUT_H             0x2A                                    // Pressure output registers
#define LPS22HB_TEMP_OUT_L              0x2B                                    // Temperature output registers
#define LPS22HB_TEMP_OUT_H              0x2C                                    // Temperature output registers
#define LPS22HB_LPFP_RES                0x33                                    // Filter reset register

// LPS22HB Address
#define SLAVE_ADDRESS_SA0               0x00
#define SLAVE_ADDRESS                   (0x5C | SLAVE_ADDRESS_SA0)              // 0x5C
#define SLAVE_ADDRESS_WRITE             SLAVE_ADDRESS << 1                      // 0xB8
#define SLAVE_ADDRESS_READ              (SLAVE_ADDRESS << 1) | 0x01             // 0xB9

// LPS22HB Device ID
#define DEVICE_ID_WHO_AM_I              0xB1

// LPS22HB_INTERRUPT_CFG
#define LPS22HB_ITC_PHE_POS             (0)
#define LPS22HB_ITC_PHE                 (0x01U << LPS22HB_ITC_PHE_POS)
#define LPS22HB_ITC_PLE_POS             (1)
#define LPS22HB_ITC_PLE                 (0x01U << LPS22HB_ITC_PLE_POS)
#define LPS22HB_ITC_LIR_POS             (2)
#define LPS22HB_ITC_LIR                 (0x01U << LPS22HB_ITC_LIR_POS)
#define LPS22HB_ITC_DIFF_EN_POS         (3)
#define LPS22HB_ITC_DIFF_EN             (0x01U << LPS22HB_ITC_DIFF_EN_POS)
#define LPS22HB_ITC_RESET_AZ_POS        (4)
#define LPS22HB_ITC_RESET_AZ            (0x01U << LPS22HB_ITC_RESET_AZ_POS)
#define LPS22HB_ITC_AUTOZERO_POS        (5)
#define LPS22HB_ITC_AUTOZERO            (0x01U << LPS22HB_ITC_AUTOZERO_POS)
#define LPS22HB_ITC_RESET_ARP_POS       (6)
#define LPS22HB_ITC_RESET_ARP           (0x01U << LPS22HB_ITC_RESET_ARP_POS)
#define LPS22HB_ITC_AUTORIFP_POS        (7)
#define LPS22HB_ITC_AUTORIFP            (0x01U << LPS22HB_ITC_AUTORIFP_POS)

// LPS22HB_CTRL_REG1
#define LPS22HB_CR1_SIM_POS             (0)
#define LPS22HB_CR1_SIM                 (0x01U << LPS22HB_CR1_SIM_POS)
#define LPS22HB_CR1_BDU_POS             (1)
#define LPS22HB_CR1_BDU                 (0x01U << LPS22HB_CR1_BDU_POS)
#define LPS22HB_CR1_LPFP_CFG_POS        (2)
#define LPS22HB_CR1_LPFP_CFG            (0x01U << LPS22HB_CR1_LPFP_CFG_POS)
#define LPS22HB_CR1_EN_LPFP_POS         (3)
#define LPS22HB_CR1_EN_LPFP             (0x01U << LPS22HB_CR1_EN_LPFP_POS)
#define LPS22HB_CR1_ODR_POS             (4)
#define LPS22HB_CR1_ODR_ONE_SHOT        (0x00U << LPS22HB_CR1_ODR_POS)
#define LPS22HB_CR1_ODR_1               (0x01U << LPS22HB_CR1_ODR_POS)
#define LPS22HB_CR1_ODR_10              (0x02U << LPS22HB_CR1_ODR_POS)
#define LPS22HB_CR1_ODR_25              (0x03U << LPS22HB_CR1_ODR_POS)
#define LPS22HB_CR1_ODR_50              (0x04U << LPS22HB_CR1_ODR_POS)
#define LPS22HB_CR1_ODR_75              (0x05U << LPS22HB_CR1_ODR_POS)

// LPS22HB_CTRL_REG2
#define LPS22HB_CR2_ONE_SHOT_POS        (0)
#define LPS22HB_CR2_ONE_SHOT            (0x01U << LPS22HB_CR2_ONE_SHOT_POS)
#define LPS22HB_CR2_SWRESET_POS         (2)
#define LPS22HB_CR2_SWRESET             (0x01U << LPS22HB_CR2_SWRESET_POS)
#define LPS22HB_CR2_I2C_DIS_POS         (3)
#define LPS22HB_CR2_I2C_DIS             (0x01U << LPS22HB_CR2_I2C_DIS_POS)
#define LPS22HB_CR2_IF_ADD_INC_POS      (4)
#define LPS22HB_CR2_IF_ADD_INC          (0x01U << LPS22HB_CR2_IF_ADD_INC_POS)
#define LPS22HB_CR2_STOP_ON_FTH_POS     (5)
#define LPS22HB_CR2_STOP_ON_FTH         (0x01U << LPS22HB_CR2_STOP_ON_FTH_POS)
#define LPS22HB_CR2_FIFO_EN_POS         (6)
#define LPS22HB_CR2_FIFO_EN             (0x01U << LPS22HB_CR2_FIFO_EN_POS)
#define LPS22HB_CR2_BOOT_POS            (7)
#define LPS22HB_CR2_BOOT                (0x01U << LPS22HB_CR2_BOOT_POS)

// LPS22HB_CTRL_REG3
#define LPS22HB_CR3_INT_S_POS           (0)
#define LPS22HB_CR3_INT_S_DATA_SIG      (0x00U << LPS22HB_CR3_INT_S_POS)
#define LPS22HB_CR3_INT_S_PH            (0x01U << LPS22HB_CR3_INT_S_POS)
#define LPS22HB_CR3_INT_S_PL            (0x02U << LPS22HB_CR3_INT_S_POS)
#define LPS22HB_CR3_INT_S_PHL           (0x03U << LPS22HB_CR3_INT_S_POS)
#define LPS22HB_CR3_DRDY_POS            (2)
#define LPS22HB_CR3_DRDY                (0x00U << LPS22HB_CR3_DRDY_POS)
#define LPS22HB_CR3_F_OVR_POS           (3)
#define LPS22HB_CR3_F_OVR               (0x01U << LPS22HB_CR3_INT_F_OVR_POS)
#define LPS22HB_CR3_F_FTH_POS           (4)
#define LPS22HB_CR3_F_FTH               (0x01U << LPS22HB_CR3_INT_F_FTH_POS)
#define LPS22HB_CR3_F_FSS5_POS          (5)
#define LPS22HB_CR3_F_FSS5              (0x01U << LPS22HB_CR3_INT_F_FSS5_POS)
#define LPS22HB_CR3_PP_OD_POS           (6)
#define LPS22HB_CR3_PP_OD               (0x01U << LPS22HB_CR3_PP_OD_POS)
#define LPS22HB_CR3_INT_H_L_POS         (7)
#define LPS22HB_CR3_INT_H_L             (0x01U << LPS22HB_CR3_INT_H_L_POS)

// LPS22HB_FIFO_CTRL
#define LPS22HB_FC_WTM_POS              (0)
#define LPS22HB_FC_MODE_POS             (5)
#define LPS22HB_FC_BYPASS               (0x00U << LPS22HB_FC_MODE_POS)
#define LPS22HB_FC_FIFO                 (0x01U << LPS22HB_FC_MODE_POS)
#define LPS22HB_FC_STREAM               (0x02U << LPS22HB_FC_MODE_POS)
#define LPS22HB_FC_STREAM_2_FIFO        (0x03U << LPS22HB_FC_MODE_POS)
#define LPS22HB_FC_BYPASS_2_STREAM      (0x04U << LPS22HB_FC_MODE_POS)
#define LPS22HB_FC_DYNAMIC_STREAM       (0x06U << LPS22HB_FC_MODE_POS)
#define LPS22HB_FC_BYPASS_2_FIFO        (0x07U << LPS22HB_FC_MODE_POS)

// LPS22HB_RES_CONF
#define LPS22HB_RESC_LC_EN_POS          (0)
#define LPS22HB_RESC_LC_EN              (0x01U << LPS22HB_RESC_LC_EN_POS)

// LPS22HB_INT_SOURCE
#define LPS22HB_ITS_PH_POS              (0)
#define LPS22HB_ITS_PH                  (0x01U << LPS22HB_ITS_PH_POS)
#define LPS22HB_ITS_PL_POS              (1)
#define LPS22HB_ITS_PL                  (0x01U << LPS22HB_ITS_PL_POS)
#define LPS22HB_ITS_IA_POS              (2)
#define LPS22HB_ITS_IA                  (0x01U << LPS22HB_ITS_IA_POS)
#define LPS22HB_ITS_BOOT_STATUS_POS     (7)
#define LPS22HB_ITS_BOOT_STATUS         (0x01U << LPS22HB_ITS_BOOT_STATUS_POS)

// LPS22HB_FIFO_STATUS
#define LPS22HB_FSTA_FSS_POS            (0)
#define LPS22HB_FSTA_OVR_POS            (6)
#define LPS22HB_FSTA_OVR                (0x01U << LPS22HB_FSTA_OVR_POS)
#define LPS22HB_FSTA_FTH_FIFO_POS       (7)
#define LPS22HB_FSTA_FTH_FIFO           (0x01U << LPS22HB_FSTA_FTH_FIFO_POS)

// LPS22HB_STATUS
#define LPS22HB_STATUS_P_DA_POS         (0)
#define LPS22HB_STATUS_P_DA             (0x01U << LPS22HB_STATUS_P_DA_POS)
#define LPS22HB_STATUS_T_DA_POS         (1)
#define LPS22HB_STATUS_T_DA             (0x01U << LPS22HB_STATUS_T_DA_POS)
#define LPS22HB_STATUS_P_OR_POS         (4)
#define LPS22HB_STATUS_P_OR             (0x01U << LPS22HB_STATUS_P_OR_POS)
#define LPS22HB_STATUS_T_OR_POS         (5)
#define LPS22HB_STATUS_T_OR             (0x01U << LPS22HB_STATUS_T_OR_POS)

typedef struct
{
    bool                            fifo_en;
    LPS22HB_FIFO_Mode_Typedef       fifo_mode;
    bool                            stop_on_fth;
    u8                              watermark;
} LPS22HB_FIFO_Typedef;
LPS22HB_FIFO_Typedef LPS22HB_FIFO_InitStruct = {0, LPS22HB_FIFO_MODE_BYPASS, 0, 0};

#endif
LPS22HB_Error_Typedef LPS22HB_Reg_Write(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt);
LPS22HB_Error_Typedef LPS22HB_Reg_Read(I2C_TypeDef *I2Cx, u8 regAddr, u8* ptr, u16 cnt);
u8 LPS22HB_WHO_AM_I_Get(I2C_TypeDef *I2Cx);
LPS22HB_Error_Typedef LPS22HB_Reset_Software_BOOT(I2C_TypeDef *I2Cx);
LPS22HB_Error_Typedef LPS22HB_Init(I2C_TypeDef *I2Cx, bool oneshot, u8 frequency);
LPS22HB_Error_Typedef LPS22HB_DeInit(I2C_TypeDef *I2Cx);
LPS22HB_Error_Typedef LPS22HB_DRDY_PIN_DataReady_Configure(I2C_TypeDef *I2Cx);
void LPS22HB_FIFO_Configure(bool fifo, LPS22HB_FIFO_Mode_Typedef mode, bool fth, u8 watermark);
LPS22HB_Error_Typedef LPS22HB_FIFO_Init(I2C_TypeDef *I2Cx);
LPS22HB_Error_Typedef LPS22HB_FIFO_Restart(I2C_TypeDef *I2Cx);
LPS22HB_Error_Typedef LPS22HB_Interrupt_Enable(I2C_TypeDef *I2Cx, LPS22HB_IT_Mode_Typedef it, u8 *THS_P);
LPS22HB_Error_Typedef LPS22HB_DRDY_PIN_Interrupt_Configure(I2C_TypeDef *I2Cx, LPS22HB_IT_Mode_Typedef int_s);
LPS22HB_Error_Typedef LPS22HB_AUTO_Configure(I2C_TypeDef *I2Cx, LPS22HB_AUTO_Mode_Typedef autoMode);
FlagStatus LPS22HB_INT_SOURCE_Get(I2C_TypeDef *I2Cx, u8 flag);
LPS22HB_Error_Typedef LPS22HB_PRESS_OUT_Get(I2C_TypeDef *I2Cx, int32_t *P_OUT);
LPS22HB_Error_Typedef LPS22HB_TEMP_OUT_Get(I2C_TypeDef *I2Cx, int16_t *T_OUT);
LPS22HB_Error_Typedef LPS22HB_Pressure_Calculation(I2C_TypeDef *I2Cx, int32_t *value);
LPS22HB_Error_Typedef LPS22HB_Temperature_Calculation(I2C_TypeDef *I2Cx, int16_t *value);
LPS22HB_Error_Typedef LPS22HB_FIFO_Rolling_Get(I2C_TypeDef *I2Cx, int32_t *p_value, int16_t *t_value);
LPS22HB_Error_Typedef LPS22HB_Calculation(I2C_TypeDef *I2Cx, int32_t *p_value, int16_t *t_value, bool oneshot);
void LPS22HB_Altitude_Calculation(int32_t *p_value);

void LPS22HB_initGPIO_DRDY_Pin();
void LPS22HB_EXTI_GPIO_DRDY_Pin();
void LPS22HB_NVIC_GPIO_DRDY_Pin();
void LPS22HB_GPIO_EXTI_Init();
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
