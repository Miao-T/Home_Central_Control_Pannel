////////////////////////////////////////////////////////////////////////////////
#ifndef __I2C_H_
#define __I2C_H_
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    SLAVE_FOUND     = (uint8_t)0,
    SLAVE_NONE      = !SLAVE_FOUND
} I2C_SlaveScan_Typedef;

////////////////////////////////////////////////////////////////////////////////
#ifdef _I2C_C_
#define GLOBAL

u8 i2cTx[] = {0x84,0x02,0x00,0x00,0x00,0x00,0x00,0x00};
u8 i2cRx[] = {0,0,0,0,0,0,0,0};


#else
#define GLOBAL extern

// extern u8 i2cTx[8];
// extern u8 i2cRx[8];

#endif


#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////

void initGPIO_I2C(I2C_TypeDef *I2Cx);
void NVIC_I2C(I2C_TypeDef* I2Cx);
void I2CInit_Master(I2C_TypeDef *I2Cx, u32 uiI2C_speed);
void BSP_I2C_Configure();
void Sensor_WriteByte(I2C_TypeDef *I2Cx, u8 dat);
void Sensor_ReadBuffer(I2C_TypeDef *I2Cx, u8* ptr, u16 cnt);
void Sensor_WriteBuffer(I2C_TypeDef *I2Cx, u8* ptr, u16 cnt);
void Sensor_Read(I2C_TypeDef *I2Cx, u8 addr, u8 subAddr, u8* ptr, u16 cnt);
void Sensor_Write(I2C_TypeDef *I2Cx, u8 addr, u8 subAddr, u8* ptr, u16 cnt);
I2C_SlaveScan_Typedef Scan_All_Addr(I2C_TypeDef *I2Cx);

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////


