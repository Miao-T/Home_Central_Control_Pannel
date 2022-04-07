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
#define SLAVE_ADDRESS                   0x5C && SLAVE_ADDRESS_SA0               // 0x5C
#define SLAVE_ADDRESS_WRITE             SLAVE_ADDRESS << 1                      // 0xB8
#define SLAVE_ADDRESS_READ              (SLAVE_ADDRESS << 1) | 0x01             // 0xB9

// LPS22HH Device ID
#define DEVICE_ID_WHO_AM_I              0xB3

// CTRL_REG1
#define LPS22HH_CR1_SIM_POS             (0)
#define LPS22HH_CR1_SIM                 (0X01U << LPS22HH_CR1_SIM_POS)
#define LPS22HH_CR1_BDU_POS             (1)
#define LPS22HH_CR1_BDU                 (0X01U << LPS22HH_CR1_BDU_POS)
#define LPS22HH_CR1_LPFP_CFG_POS        (2)
#define LPS22HH_CR1_LPFP_CFG            (0X01U << LPS22HH_CR1_LPFP_CFG_POS)
#define LPS22HH_CR1_EN_LPFP_POS         (3)
#define LPS22HH_CR1_EN_LPFP             (0X01U << LPS22HH_CR1_EN_LPFP_POS)
#define LPS22HH_CR1_ODR_POS             (4)
#define LPS22HH_CR1_ODR_ONE_SHOT        (0X00U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_1               (0X01U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_10              (0X02U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_25              (0X03U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_50              (0X04U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_75              (0X05U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_100             (0X06U << LPS22HH_CR1_ODR_POS)
#define LPS22HH_CR1_ODR_200             (0X07U << LPS22HH_CR1_ODR_POS)

// CTRL_REG2
#define LPS22HH_CR2_ONE_SHOT_POS        (0)
#define LPS22HH_CR2_ONE_SHOT            (0X01U << LPS22HH_CR2_ONE_SHOT_POS)
#define LPS22HH_CR2_LOW_NOISE_EN_POS    (1)
#define LPS22HH_CR2_LOW_NOISE_EN        (0X01U << LPS22HH_CR2_LOW_NOISE_EN_POS)
#define LPS22HH_CR2_SWRESET_POS         (2)
#define LPS22HH_CR2_SWRESET             (0X01U << LPS22HH_CR2_SWRESET_POS)
#define LPS22HH_CR2_IF_ADD_INC_POS      (4)
#define LPS22HH_CR2_IF_ADD_INC          (0X01U << LPS22HH_CR2_IF_ADD_INC_POS)
#define LPS22HH_CR2_PP_OD_POS           (5)
#define LPS22HH_CR2_PP_OD               (0X01U << LPS22HH_CR2_PP_OD_POS)
#define LPS22HH_CR2_INT_H_L_POS         (6)
#define LPS22HH_CR2_INT_H_L             (0X01U << LPS22HH_CR2_INT_H_L_POS)
#define LPS22HH_CR2_BOOT_POS            (7)
#define LPS22HH_CR2_BOOT                (0X01U << LPS22HH_CR2_BOOT_POS)

// CTRL_REG3
#define LPS22HH_CR3_INT_S_POS           (0)
#define LPS22HH_CR3_INT_S_DATA_SIG      (0X00U << LPS22HH_CR3_INT_S_POS)
#define LPS22HH_CR3_INT_S_PH            (0X01U << LPS22HH_CR3_INT_S_POS)
#define LPS22HH_CR3_INT_S_PL            (0X02U << LPS22HH_CR3_INT_S_POS)
#define LPS22HH_CR3_INT_S_PHL           (0X03U << LPS22HH_CR3_INT_S_POS)
#define LPS22HH_CR3_DRDY_POS            (2)
#define LPS22HH_CR3_DRDY                (0X00U << LPS22HH_CR3_DRDY_POS)
#define LPS22HH_CR3_INT_F_OVR_POS       (3)
#define LPS22HH_CR3_INT_F_OVR           (0X00U << LPS22HH_CR3_INT_F_OVR_POS)
#define LPS22HH_CR3_INT_F_WTM_POS       (4)
#define LPS22HH_CR3_INT_F_WTM           (0X00U << LPS22HH_CR3_INT_F_WTM_POS)
#define LPS22HH_CR3_INT_F_FULL_POS      (5)
#define LPS22HH_CR3_INT_F_FULL          (0X00U << LPS22HH_CR3_INT_F_FULL_POS)



#endif



////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
