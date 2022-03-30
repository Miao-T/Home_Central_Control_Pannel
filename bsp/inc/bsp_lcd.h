////////////////////////////////////////////////////////////////////////////////
#ifndef _BSP_LCD_H
#define _BSP_LCD_H
////////////////////////////////////////////////////////////////////////////////


#if defined(__MDM2803) || defined(__MDM2803_N)

#if defined(__WIDESCREEN)
#define WIDTH       320                 /* Screen Width (in pixels)           */
#define HEIGHT      240                 /* Screen Hight (in pixels)           */
#else
#define WIDTH       240                 /* Screen Width (in pixels)           */
#define HEIGHT      320                 /* Screen Hight (in pixels)           */
#endif

enum {	SPACE,	FRAME};
enum {	NUL,	FILL};


#define Black        0x0000     /*   0,   0,   0  黑色       */   
#define Navy         0x000F     /*   0,   0, 128  深蓝色     */ 
#define DarkGreen    0x03E0     /*   0, 128,   0  深绿色     */ 
#define DarkCyan     0x03EF     /*   0, 128, 128  深青色     */ 
#define Maroon       0x7800     /* 128,   0,   0  深红色     */     
#define Purple       0x780F     /* 128,   0, 128  紫色       */ 
#define Olive        0x7BE0     /* 128, 128,   0  橄榄绿     */     
#define LightGrey    0xC618     /* 192, 192, 192  灰白色     */ 
#define DarkGrey     0x7BEF     /* 128, 128, 128  深灰色     */     
#define Blue         0x001F     /*   0,   0, 255  蓝色       */   
#define Green        0x07E0     /*   0, 255,   0  绿色       */         
#define Cyan         0x07FF     /*   0, 255, 255  青色       */ 
#define Red          0xF800     /* 255,   0,   0  红色       */      
#define Magenta      0xF81F     /* 255,   0, 255  品红       */
#define Yellow       0xFFE0     /* 255, 255, 0    黄色       */       
#define White        0xFFFF     /* 255, 255, 255  白色       */ 
//chend:RGB565
#define Orange       0xFBE4      /*   0,   0,   0 */
#define SkyBlue      0x749F

////////////////////////////////////////////////////////////////////////////////
#define lcdCsL()	GPIOD->BRR =   GPIO_Pin_7;
#define lcdCsH()	GPIOD->BSRR =  GPIO_Pin_7;

#define lcdWriteL()	GPIOD->BRR =   GPIO_Pin_5;
#define lcdWriteH()	GPIOD->BSRR =  GPIO_Pin_5;

#define lcdReadH()	GPIOD->BSRR =  GPIO_Pin_4;
#define lcdReadL()	GPIOD->BRR =   GPIO_Pin_4;

#define lcdBlcL()	GPIOF->BRR =   GPIO_Pin_11;
#define lcdBlcH()	GPIOF->BSRR =  GPIO_Pin_11;

#define lcdRsL()	GPIOD->BRR =   GPIO_Pin_13;
#define lcdRsH()	GPIOD->BSRR =  GPIO_Pin_13;

#define spiDatL()   GPIOB->BRR =   GPIO_Pin_15;
#define spiDatH()   GPIOB->BSRR =  GPIO_Pin_15;

#define splClkL()   GPIOB->BRR =   GPIO_Pin_13;
#define spiClkH()   GPIOB->BSRR =  GPIO_Pin_13;

#define spiClk()	GPIOB->BSRR =  GPIO_Pin_13; \
					GPIOB->BRR  =  GPIO_Pin_13

#define wrPulse() 	GPIOD->BRR  =  GPIO_Pin_5; \
					GPIOD->BSRR =  GPIO_Pin_5
						
#define spiSoftLow()		spiDatL();				\
							crh = GPIOB->CRH;		\
							crh &= ~0xF0F00000;		\
							crh |=  0x30300000;		\
							GPIOB->CRH = crh;		\
							spiClk()


#define spiSoftHigh()		spiDatH();				\
							crh = GPIOB->CRH;		\
							crh &= ~0xF0F00000;		\
							crh |=  0x30300000;		\
							GPIOB->CRH = crh;		\
							spiClk()
				
#define spiWrite(x)			crh = GPIOB->CRH;		\
							crh &= ~0xF0F00000;		\
							crh |=  0xB0B00000;		\
							GPIOB->CRH = crh;		\
							SPI2->TDR = x;		\
							while(!(SPI2->SR & 0x01))
                                
////////////////////////////////////////////////////////////////////////////////
typedef struct{
	u16	c1;		// button  line
	u16	c2;		// button  back
	u16	c3;		// button  shadow	active
	u16	c4;		// button  shadow	not active
} colorDef;

typedef struct{
	u16	fore;		
	u16	back;		
} Color_Def;

typedef struct {
    u16 width;   
    u16 height;  
    u16 id;      
    u8  dir;     
    u16  wramcmd; 
    u16  setxcmd;
    u16  setycmd;
} Lcd_Dev_Def;

////////////////////////////////////////////////////////////////////////////////
#ifdef _BSP_LCD_C_
#define GLOBAL

#else
#define GLOBAL extern


#endif

GLOBAL colorDef     color;
GLOBAL Color_Def    text;
GLOBAL Lcd_Dev_Def  lcdDev;
#undef GLOBAL

////////////////////////////////////////////////////////////////////////////////
void drawPoint(u16 x, u16 y, u16 c);
void drawLine (s16 x1, s16 y1, s16 x2, s16 y2, u16 c);
void drawSquare(u16 x, u16 y, u16 w, u16 h, u8 frame, u8 fill);
void drawRec (u16 x, u16 y, u16 w, u16 h, u16 c);
void drawCircle(u16 Xpos, u16 Ypos, u16 Radius, u16 c);
void drawTab(u16 x, u16 y, u16 w, u16 h, u8 m, u8 n);
void drawTabTitle(u16 x, u16 y, u16 w, u16 h, u8 m, u8 n);

void drawChar_6x8(u16 x, u16 y, u8 bc, u8 chr);
void drawChar_8x16(u16 x, u16 y, u8 bc, u8 chr);

void putChar(u16 x, u16 y, u8 font, u8 bc, char c);
void putStr(u16 x, u16 y, u8 fontsize, u8 backcolor, char *string);
void putBmp(u16 x, u16 y, u16 w, u16 h, u16 *ptr);

void drawDialog(u16 w, u16 h, char* str, Color_Def c);
void clearDialog(u16 w, u16 h, Color_Def c);

void drawButton(u16 x, u16 y, u16 w, u16 h, u8 att, u8 sel, char* str);

void lcdSetTextColor (u16 c);
u16 lcdGetTextColor (void);
void lcdSetBackColor (u16 c);
u16 lcdGetBackColor (void);
void LCDC_Init_Reg();

void lcdRst();
void lcdFillColor(u16 c);

void lcdCmd(u8 cmd);
void lcdData(u8 dat);

inline void spiSoftMode();
inline void spiHardMode();
static void _delay(u32 n);

void initGPIO_LCD();
void initFSMC(void);

void drawCharCD(u16 x, u16 y, u8 bc, char c);
void drawNum(u16 x, u16 y, u8 bc, u8 mode, s16 num);
void drawTriangle(u16 x, u16 y, u16 w, u16 h, u16 md, u16 c);
void drawRoundRec (u16 x, u16 y, u16 w, u16 h, u16 c);
#endif
#endif