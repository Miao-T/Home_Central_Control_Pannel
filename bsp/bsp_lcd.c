////////////////////////////////////////////////////////////////////////////////
#define _BSP_LCD_C_
////////////////////////////////////////////////////////////////////////////////

#if defined(__MDM2803) || defined(__MDM2803_N)

#include <string.h>

#include "common.h"
#include "bsp_lcd.h"
#include "font_evb.h"
#include "font.h"

#include "hal_gpio.h"
#include "hal_fsmc.h"

////////////////////////////////////////////////////////////////////////////////
//  LCD Bus Ctrl
////////////////////////////////////////////////////////////////////////////////
// ¢NNone	nRST
// ¢PPF_11	BLC 
// ¢PPD_13	RS  
// ¢PPD_7 	nCS 
// ¢PPD_5 	nWR 
// ¢PPD_4 	nRD 
//   FMC D0-D15
////////////////////////////////////////////////////////////////////////////////
void initGPIO_LCD()
{
	GPIO_InitTypeDef GPIO_InitStructure;

    COMMON_EnableIpClock(emCLOCK_GPIOB);
    COMMON_EnableIpClock(emCLOCK_GPIOD);
    COMMON_EnableIpClock(emCLOCK_GPIOE);
    COMMON_EnableIpClock(emCLOCK_GPIOF);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_12);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_12);

    GPIO_InitStructure.GPIO_Pin     =  GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOF, GPIO_Pin_11);
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);  
    
    lcdCsL();
}

////////////////////////////////////////////////////////////////////////////////
void initFSMC(void)
{
    FSMC_InitTypeDef        FSMC_InitStructure;
    FSMC_TimingInitTypeDef  FSMC_TimingInitStructure;

    COMMON_EnableIpClock(emCLOCK_FSMC);
    
    FSMC_TimingInitStructure.FSMC_SMReadPipe    = 0;
    FSMC_TimingInitStructure.FSMC_ReadyMode     = 0;
    FSMC_TimingInitStructure.FSMC_WritePeriod   = 0x2;
    FSMC_TimingInitStructure.FSMC_WriteHoldTime = 1;
    FSMC_TimingInitStructure.FSMC_AddrSetTime   = 3;
    FSMC_TimingInitStructure.FSMC_ReadPeriod    = 0x1;
    
    FSMC_InitStructure.FSMC_Mode                = FSMC_Mode_8080;
    FSMC_InitStructure.FSMC_MemoryDataWidth     = FSMC_DataWidth_16bits;
    FSMC_InitStructure.FSMC_TimingRegSelect     = FSMC_TimingReg_Set0;
    FSMC_InitStructure.FSMC_MemSize             = FSMC_MemSize_64MB;
    FSMC_InitStructure.FSMC_MemType             = FSMC_MemType_NorSRAM;
    FSMC_InitStructure.FSMC_AddrDataMode        = FSMC_AddrDataMUX;
    FSMC_InitStructure.FSMC_TimingStruct        = &FSMC_TimingInitStructure;

    FSMC_NORSRAMInit(&FSMC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void lcdCmd(u8 cmd)
{	
    *(u16*)(0x60000000) = cmd;
}

////////////////////////////////////////////////////////////////////////////////
void lcdData(u8 dat)
{	
    *(u16*)(0x60000000 | (0x01 << 19)) = dat;
}

////////////////////////////////////////////////////////////////////////////////
void lcdData16(u16 dat)
{	
    *(u16*)(0x60000000 | (0x01 << 19)) = dat;
}

////////////////////////////////////////////////////////////////////////////////
void lcdSetWindow(u32 x, u32 y, u32 w, u32 h) 
{
    lcdCmd(0x2A);
    lcdData(x >> 8);
    lcdData(x & 0xFF);
    lcdData((x + w - 1) >> 8);
    lcdData((x + w - 1) & 0xFF);
	
    lcdCmd(0x2B);
    lcdData(y >> 8);
    lcdData(y & 0xFF);
    lcdData((y + h - 1) >> 8);
    lcdData((y + h - 1) & 0xFF);
}

////////////////////////////////////////////////////////////////////////////////
void lcdRst()
{
    
}

////////////////////////////////////////////////////////////////////////////////
void lcdFillColor(u16 c) 
{
	lcdSetWindow(0, 0, WIDTH,  HEIGHT);
    lcdCmd(0x2c);

#ifdef MDM2803	
    lcdData(c);
	for(u16 i = 0; i < (WIDTH * HEIGHT) / 8 ; i++) {
		lcdData(0x0000);
        lcdData(0x0000);
        lcdData(0x0000);
        lcdData(0x0000);
        lcdData(0x0000);
        lcdData(0x0000);
        lcdData(0x0000);
        lcdData(0x0000);
	}
#else
	for(u16 i = 0; i < (WIDTH * HEIGHT) / 8  ; i++) {
    	lcdData16(c);
    	lcdData16(c);
    	lcdData16(c);
    	lcdData16(c);

		lcdData16(c);
    	lcdData16(c);
    	lcdData16(c);
    	lcdData16(c);
	}
#endif	
}

////////////////////////////////////////////////////////////////////////////////
void drawSquare(u16 x, u16 y, u16 w, u16 h, u8 frame, u8 fill)
{
	if (!frame) {
		if (fill)
			drawRec (x, y, w, h, color.c2);
		drawRec (x, 			y, 			w, 	1, color.c1);
		drawRec (x, 			y + h - 1, 	w, 	1, color.c1);
		drawRec (x, 			y, 			1, 	h, color.c1);
		drawRec (x + w - 1, 	y, 			1, 	h, color.c1);
	}
	else {
		drawRec (x, y, w, h, color.c1);
	}
}

////////////////////////////////////////////////////////////////////////////////
void drawShadow(u16 x, u16 y, u16 w, u16 h,u8 frame, u16 c)
{
	drawLine (x + 1,	y + h,  x + w, y + h, c);
	drawLine (x + w, y + 1, 	x + w, y + h, c);
}

////////////////////////////////////////////////////////////////////////////////
void drawFrame(u16 x, u16 y, u16 w, u16 h,u8 frame, u8 sel)
{
	drawSquare(x, y, w, h, frame, FILL);
	
	u16 colour = (!sel) ?  color.c3 :  color.c4;
	drawShadow(x, y, w, h, frame, colour);
}

////////////////////////////////////////////////////////////////////////////////
void drawSquare_1(u16 x, u16 y, u16 w, u16 h, u16 c)
{
	drawRec (x, y, w, h, c);
}

////////////////////////////////////////////////////////////////////////////////
void drawFrame_1(u16 x, u16 y, u16 w, u16 h, u16 c)
{
	drawRec (x, 			y, 			w, 	1, c);
	drawRec (x, 			y + h - 1, 	w, 	1, c);
	drawRec (x, 			y, 			1, 	h, c);
	drawRec (x + w - 1, 	y, 			1, 	h, c);
}

////////////////////////////////////////////////////////////////////////////////
void drawDialog(u16 w, u16 h, char* str, Color_Def c)
{
	u16 x = (320 - w) / 2;
	u16 y = (240 - h) / 2;

	drawSquare_1(x, y, w, h, c.back);
	drawFrame_1(x + 6,  y + 6,  w - 12,  h - 12,  c.fore);
	
	text.back = LightGrey;
	putStr(x + 12,  y + 1, 2, 1, str);
}

////////////////////////////////////////////////////////////////////////////////
void clearDialog(u16 w, u16 h, Color_Def c)
{
	u16 x = (320 - w) / 2;
	u16 y = (240 - h) / 2;
	drawSquare_1(x, y, w, h, c.back);
}

////////////////////////////////////////////////////////////////////////////////
void drawButton(u16 x, u16 y, u16 w, u16 h, u8 frame, u8 sel, char* str)
{
	drawFrame(x,  y,  w,  h,  frame,  sel);
	putStr(x + (w -  strlen((char*)str) * 8) / 2,  y + 3, 2, 0, str);
}

////////////////////////////////////////////////////////////////////////////////
void drawChar_6x8(u16 x, u16 y, u8 bc, u8 chr) 
{
    if (0x1F < chr && chr < 0x90) {
        u16 k = (chr - 0x20) * 8;
        for (u16 i = 0; i < 8; i++) {
            for (u16 j = 0; j < 6; j++) {
                if (Font_6x8_h[k + i] & (0x01 << (7 - j))) {
                    drawPoint(x + j, y + i, text.fore);
                }
                else {
                    if (bc) {
                        drawPoint(x + j, y + i, text.back);
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawChar_8x16(u16 x, u16 y, u8 bc, u8 chr) 
{
    if (0x1F < chr && chr < 0x90) {
        u16 k = (chr - 0x20) * 16;
        for (u16 i = 0; i < 16; i++) {
            for (u16 j = 0; j < 8; j++){
              	if (Font_8x16_h[k + i] & (0x01 << (7-j))){
                  	drawPoint(x + j, y + i, text.fore);
              	}
              	else{
                  	if (bc) {
                    	drawPoint(x + j, y + i, text.back);
                  	}
              	}
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawChar_8x12(u16 x, u16 y,  u8 bc, char c) 
{
    if (0x1F < c && c < 0x90) {
        u16 k = (c - 0x20) * 12;
        for (u16 i = 0; i < 12; i++) {
            for (u16 j = 0; j < 8; j++){
              	if (Font_8x12_h[k + i] & 0x01 << (7-j)){
                  	drawPoint(x + j, y + i, text.fore);
              	}
              	else{
                  	if (bc) {
                    	drawPoint(x + j, y + i, text.back);
                  	}
              	}
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawChar_16x24(u16 x, u16 y, u8 bc, char c) 
{
    if (0x1F < c && c < 0x90) {
        u16 k = (c - 0x20) * 24;
        for (u16 i = 0; i < 24; i++) {
            for (u16 j = 0; j < 16; j++){
              	if (Font_16x24_h[k + i] & (0x01 << j)){
                  	drawPoint(x + j, y + i, text.fore);
              	}
              	else{
                  	if (bc) {
                    	drawPoint(x + j, y + i, text.back);
                  	}
              	}
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawCharCD(u16 x, u16 y, u8 bc, char c) 
{
    if (0x1F < c && c < 0x90) {
        u16 k = (c - 0x20) * 9;
        for (u16 i = 0; i < 9; i++) {
            for (u16 j = 0; j < 64; j++){
                for (u16 m = 0; m < 8; m++){
                    //u16 tempXLoc = (((i+1) * (j+1) - 1)*8 + (m)) % 48 ;
                    //u16 tempYLoc = (((i+1) * (j+1) - 1)*8 + (m)) / 48 ;
                    u16 tempXLoc = (((i) * (j))*8 + (m)) % 48 ;
                    u16 tempYLoc = (((i) * (j))*8 + (m)) / 48 ;
                    if (asc2_1206[k+i][j] & (0x01 << m)){
                        drawPoint(x + tempXLoc, y + tempYLoc, text.fore);
                    }
                    else{
                        if (bc) {
                            drawPoint(x + tempXLoc, y + tempYLoc, text.back);
                        }
                    }
                }
            }
        }
    }
}

/* 88 * 107*/
////////////////////////////////////////////////////////////////////////////////
void drawDSEG_88_107(u16 x, u16 y, u8 bc, u8 idx) 
{
    u16 k = idx;
    for (u16 i = 0; i < 1177; i++) {
        for (u16 j = 0; j < 8; j++){
            u16 tempXLoc = (i*8 + j) % 88 ;
            u16 tempYLoc = (i*8 + j) / 88 ;
            if (DSEG_88_107[k][i] & (0x01 << (7-j))){
                drawPoint(x + tempXLoc, y + tempYLoc, text.fore);
            }
            else{
                if (bc) {
                    drawPoint(x + tempXLoc, y + tempYLoc, text.back);
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void drawDSEG_72_80(u16 x, u16 y, u8 bc, u8 idx) 
{
    u16 k = idx;
    for (u16 i = 0; i < 720; i++) {
        for (u16 j = 0; j < 8; j++){
            u16 tempXLoc = (i*8 + j) % 72 ;
            u16 tempYLoc = (i*8 + j) / 72 ;
            if (DSEG_72_80[k][i] & (0x01 << (7-j))){
                drawPoint(x + tempXLoc, y + tempYLoc, text.fore);
            }
            else{
                if (bc) {
                    drawPoint(x + tempXLoc, y + tempYLoc, text.back);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawTriangle(u16 x, u16 y, u16 w, u16 h, u16 md, u16 c)
{
    if (md){
        for(u16 dy = 0; dy < h; dy++){
            if(w>w*dy/h)
                drawRec(x + w*dy/2/h, y + dy, w-w*dy/h, 1, c);
        }
    }
    else{
        for(u16 dy = 0; dy < h; dy++){
            if(w>w*dy/h)
                drawRec(x + w*dy/2/h, y - dy, w-w*dy/h, 1, c);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawNum(u16 x, u16 y, u8 bc, u8 mode, s16 num) 
{
    if (mode){
        u16 x1 = x, x2 = x + 88, x3 = x + 176;
        if (num >= 0 && num <= 9) {
            drawDSEG_88_107(x1, y, bc, 0);
            drawDSEG_88_107(x2, y, bc, 0);
            drawDSEG_88_107(x3, y, bc, num);
        }
        if (num >= 9 && num <= 99) {
            drawDSEG_88_107(x1, y, bc, 0);
            drawDSEG_88_107(x2, y, bc, num / 10);
            drawDSEG_88_107(x3, y, bc, num % 10);
        }
        if (num >= 99 && num <= 999) {
            drawDSEG_88_107(x1, y, bc, num / 100);
            drawDSEG_88_107(x2, y, bc, (num % 100) / 10);
            drawDSEG_88_107(x3, y, bc, num % 10);
        }
    }
    else {
        u16 x1 = x, x2 = x + 72, x3 = x + 144;
        if (num >= 0 && num <= 9) {
            drawDSEG_72_80(x1, y, bc, 0);
            drawDSEG_72_80(x2, y, bc, 0);
            drawDSEG_72_80(x3, y, bc, num);
        }
        if (num >= 9 && num <= 99) {
            drawDSEG_72_80(x1, y, bc, 0);
            drawDSEG_72_80(x2, y, bc, num / 10);
            drawDSEG_72_80(x3, y, bc, num % 10);
        }
        if (num >= 99 && num <= 999) {
            drawDSEG_72_80(x1, y, bc, num / 100);
            drawDSEG_72_80(x2, y, bc, (num % 100) / 10);
            drawDSEG_72_80(x3, y, bc, num % 10);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void putChar(u16 x, u16 y, u8 font, u8 bc, char c) 
{
    if (c & 0x80) return;
    switch (font) {
        case 0:  /* Font 6 x 8 */
            drawChar_6x8(x, y, bc, c);
            break;
        case 1:  /* Font 8 x 12 */
            drawChar_8x12(x, y, bc, c);
            break;
		case 2:  /* Font 8 x 16 */
            drawChar_8x16(x, y, bc, c);
			break;
        case 3:  /* */
            drawCharCD(x, y, bc, c);
            break;
        default:
            drawChar_16x24(x, y, bc, c);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void putStr(u16 x, u16 y, u8 font, u8 bc, char *str) 
{
    while (*str) {
        switch(font) {
        case 0:
          putChar(x, y, font, bc, *str++);
          x += 6;
          break;
        case 1:
          putChar(x, y, font, bc, *str++);
          x += 8;
          break;
		case 2:
          putChar(x, y, font, bc, *str++);
          x += 8;
          break;
		default:
          putChar(x, y, font, bc, *str++);
          x += 16;
          break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void putBmp(u16 x, u16 y, u16 w, u16 h, u16 *ptr) 
{
    lcdSetWindow(x, y, w, h);
    lcdCmd(0x2c);
    
    for(u32 i = 0; i < (w * h); i++){
		lcdData(*ptr++); 
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawHalfCircle(u16 x, u16 y, u16 r, u16 dir, u16 c)
{
    s32 D = 3 - (r << 1);	/* Decision Variable */
    u32 CurX = 0;			/* Current X Value */
    u32 CurY = r;			/* Current Y Value */
	
    if (dir){
        while (CurX <= CurY) {
            drawRec(x - CurX, y + CurY, CurX, 1, c);
            drawRec(x - CurX, y - CurY, CurX, 1, c);
            drawRec(x - CurY, y + CurX, CurY, 1, c);
            drawRec(x - CurY, y - CurX, CurY, 1, c);
            if (D < 0)
                D += (CurX << 2) + 6;
            else {
                D += ((CurX - CurY) << 2) + 10;
                CurY--;
            }
            CurX++;
        }
    }
    else {
        while (CurX <= CurY) {
            drawRec(x, y + CurY, CurX, 1, c);
            drawRec(x, y - CurY, CurX, 1, c);
            drawRec(x, y + CurX, CurY, 1, c);
            drawRec(x, y - CurX, CurY, 1, c);
            if (D < 0)
                D += (CurX << 2) + 6;
            else {
                D += ((CurX - CurY) << 2) + 10;
                CurY--;
            }
            CurX++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawRoundRec (u16 x, u16 y, u16 w, u16 h, u16 c)
{
	drawRec (x + h / 2, y, w - h + 1, h, c);
    drawHalfCircle(x + h / 2 , y + h / 2, h/2, 1, c);
    drawHalfCircle(x + w - h / 2 , y + h / 2, h/2, 0, c);
}

//void image_display(u16 x,u16 y, u8* imgx)
//{
//	HEADCOLOR *imginfo;
// 	u8 ifosize = sizeof(HEADCOLOR);
//	imginfo=(HEADCOLOR*)imgx;
// 	putBmp(x, y, imginfo->w , imginfo->h,  (u16*)(imgx + ifosize));
//}

////////////////////////////////////////////////////////////////////////////////
void drawRec (u16 x, u16 y, u16 w, u16 h, u16 c)
{
	lcdSetWindow (x, y, w, h);
	lcdCmd(0x2c);
	
    u16 cnt = (w * h) / 8;
    u8  mod = (w * h) % 8;
    
    for (u16 i = 0; i < cnt; i++){
        lcdData16(c);
        lcdData16(c);
        lcdData16(c);
        lcdData16(c);
        lcdData16(c);
        lcdData16(c);
        lcdData16(c);
        lcdData16(c);
    }
    
    for (u8 i = 0; i < mod; i++){
        lcdData16(c);
    }
}

////////////////////////////////////////////////////////////////////////////////
void drawPoint(u16 x, u16 y, u16 c) 
{
	drawRec(x, y, 1, 1, c);
}

////////////////////////////////////////////////////////////////////////////////
void drawLine (s16 x1, s16 y1, s16 x2, s16 y2, u16 c)
{
	u16 w = (x1 > x2) ? (x1 - x2) : (x2 - x1);
	u16 h = (y1 > y2) ? (y1 - y2) : (y2 - y1);
	
	u16 x = (x1 > x2) ? (x2) : (x1);
	u16 y = (y1 > y2) ? (y2) : (y1);
	
	if (y1 == y2) {
		drawRec(x, y, w, 1, c);
	}
	else if (x1 == x2){
		drawRec(x, y, 1, h, c);
	}
	else if (w >= h) {
		for (u16 i = 0; i <= w; i++){
			drawRec(x + i, 
                    i * (y2 - y1) / w + y1, 
                    1, 
                    1, 
                    c);
		}		
	}
	else {
		for (u16 i = 0; i < h; i ++){
			drawRec(
                    ((y + i) * w ) / h + x,  
                    y + i, 
                    1, 
                    1, 
                    c);
		}			   
	}
}

////////////////////////////////////////////////////////////////////////////////
void drawTab(u16 x, u16 y, u16 w, u16 h, u8 m, u8 n)
{
	for (u8 i = 1; i < m; i++){
		drawLine (x + i * (w / m),   y,  x + i * (w / m),  y + h,  color.c1);
	}
	for (u8 i = 1; i < n; i++){
		drawLine (x,  y + i * (h / n),  x + w,  y + i * (h / n),  color.c1);
	}
	drawSquare(x, y, w, h, SPACE, NUL);
}

////////////////////////////////////////////////////////////////////////////////
void drawTabTitle(u16 x, u16 y, u16 w, u16 h, u8 m, u8 n)
{	
	char str[8];
	strcpy( str, "ADC");
	putStr(175, 165, 0, 1, str);

	strcpy( str, "IR");
	putStr(253, 165, 0, 1, str);
	
	strcpy( str, "UART1");
	putStr(175, 180, 0, 1, str);
	
	strcpy( str, "UART2");
	putStr(243, 180, 0, 1, str);
	
	strcpy( str, "SPI");
	putStr(175, 195, 0, 1, str);

	strcpy( str, "I2C");
	putStr(175, 210, 0, 1, str);

	strcpy( str, "CAN");
	putStr(175, 225, 0, 1, str);
}

////////////////////////////////////////////////////////////////////////////////
void drawCircle(u16 x, u16 y, u16 r, u16 c)
{
    s32 D = 3 - (r << 1);	/* Decision Variable */
    u32 CurX = 0;			/* Current X Value */
    u32 CurY = r;			/* Current Y Value */
	
    while (CurX <= CurY) {
        drawRec(x + CurX, y + CurY, 1, 1, c);
        drawRec(x + CurX, y - CurY, 1, 1, c);
        drawRec(x - CurX, y + CurY, 1, 1, c);
        drawRec(x - CurX, y - CurY, 1, 1, c);
        drawRec(x + CurY, y + CurX, 1, 1, c);
        drawRec(x + CurY, y - CurX, 1, 1, c);
        drawRec(x - CurY, y + CurX, 1, 1, c);
        drawRec(x - CurY, y - CurX, 1, 1, c);
        if (D < 0){
            D += (CurX << 2) + 6;
        }
        else {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }
}

////////////////////////////////////////////////////////////////////////////////
void lcdSetTextColor (u16 _color) 
{
	text.fore = _color;
}

////////////////////////////////////////////////////////////////////////////////
u16 lcdGetTextColor (void) 
{
	return text.fore;
}

////////////////////////////////////////////////////////////////////////////////
void lcdSetBackColor (u16 _color) 
{
	text.back = _color;
}

////////////////////////////////////////////////////////////////////////////////
u16 lcdGetBackColor (void) 
{
	return text.back;
}

////////////////////////////////////////////////////////////////////////////////
void LCDC_Init_Reg() 
{ 
#if defined(__MDM2803)
	lcdCmd(0x28);   //display off
	
    lcdCmd(0xCB);   //Power control A
    lcdData(0x39); 
    lcdData(0x2C); 
    lcdData(0x00); 
    lcdData(0x34); 
    lcdData(0x02); 

    lcdCmd(0xCF);   //Power control B
    lcdData(0x00); 
    lcdData(0xC1); 	//lcdData(0x81); 
    lcdData(0X30); 
    
    lcdCmd(0xED);   //Power on sequence control
    lcdData(0x64); 
    lcdData(0x03); 
    lcdData(0X12); 
    lcdData(0X81); 
    
    lcdCmd(0xE8);   //Driver timing control A
    lcdData(0x85); 
    lcdData(0x10); 
    lcdData(0x7A); 
    
    lcdCmd(0xF7);   //Pump ratio control
    lcdData(0x20); 
    
    lcdCmd(0xEA);   //Driver timing control B
    lcdData(0x00); 
    lcdData(0x00); 
    
    lcdCmd(0xC0);    //Power control 
    lcdData(0x1B);   //VRH[5:0]  1B
    
    lcdCmd(0xC1);    //Power control 
    lcdData(0x01);   //SAP[2:0];BT[3:0] 
    
    lcdCmd(0xC5);    //VCM control 
    lcdData(0x45);   //3F
    lcdData(0x25);   //3C
    
    lcdCmd(0xC7);    //VCM control2 
    lcdData(0XB7);   //b7
    
    lcdCmd(0x36);    // Memory Access Control 
#if defined(__WIDESCREEN)
    lcdData(0x28);
#else
    lcdData(0x48); 
#endif
    
    lcdCmd(0x3A);    //Pixel Format Set
    lcdData(0x55); 
    
    lcdCmd(0xB1);    //Frame Rate Control
    lcdData(0x00);   
    lcdData(0x1A); 
    
    lcdCmd(0xB6);    //Display Function Control 
    lcdData(0x0A); 
    lcdData(0x82); 
    
    lcdCmd(0xF2);    //3Gamma Function Disable 
    lcdData(0x00); 
    
    lcdCmd(0x26);    //Gamma curve selected 
    lcdData(0x01); 
    
    lcdCmd(0xE0);    //Set Gamma 
    lcdData(0x0F); 
    lcdData(0x2A); 
    lcdData(0x28); 
    lcdData(0x08); 
    lcdData(0x0E); 
    lcdData(0x08); 
    lcdData(0x54); 
    lcdData(0XA9); 
    lcdData(0x43); 
    lcdData(0x0A); 
    lcdData(0x0F); 
    lcdData(0x00); 
    lcdData(0x00); 
    lcdData(0x00); 
    lcdData(0x00); 
    
    lcdCmd(0xE1);    //Set Gamma 
    lcdData(0x00); 
    lcdData(0x15); 
    lcdData(0x17); 
    lcdData(0x07); 
    lcdData(0x11); 
    lcdData(0x06); 
    lcdData(0x2B); 
    lcdData(0x56); 
    lcdData(0x3C); 
    lcdData(0x05); 
    lcdData(0x10); 
    lcdData(0x0F); 
    lcdData(0x3F); 
    lcdData(0x3F); 
    lcdData(0x0F); 
    
    lcdCmd(0x11);   //Exit Sleep
    while(!delay(120)); 
    lcdCmd(0x29);   //display on
    
#else
    lcdCmd(0x11); 
    while(!delay(120)); 
    
    lcdCmd(0x36);
#if defined(__WIDESCREEN)    
    lcdData(0x60);
#else
    lcdData(0xC0);
#endif
    
    lcdCmd(0x3A); 
    lcdData(0X05); 
    
    lcdCmd(0xB2);  
    lcdData(0x0C); 
    lcdData(0x0C); 
    lcdData(0x00); 
    lcdData(0x33);
    lcdData(0x33);
    
    lcdCmd(0xB7);  
    lcdData(0x35); 
    
    lcdCmd(0xBB);  				//vcom
    lcdData(0x32);    //30
    
    lcdCmd(0xC0);  				  //
    lcdData(0x2C); 
    
    lcdCmd(0xC2);  
    lcdData(0x01); 
    
    lcdCmd(0xC3);  	   //vrh
    lcdData(0x10); 	   //17		   0D
    
    lcdCmd(0xC4);  			   //vdv
    lcdData(0x20);	  //20
    
    lcdCmd(0xC6);  
    lcdData(0x0f);     //0f µ÷´¥ÃþÉÁ¶¯
    
    lcdCmd(0xD0);  
    lcdData(0xA4); 
    lcdData(0xA1); 
    
    lcdCmd(0xE0);    //Set Gamma 
    lcdData(0xd0); 
    lcdData(0x00); 
    lcdData(0x02); 
    lcdData(0x07); 
    lcdData(0x0a); 
    lcdData(0x28); 
    lcdData(0x32); 
    lcdData(0X44); 
    lcdData(0x42); 
    lcdData(0x06); 
    lcdData(0x0e); 
    lcdData(0x12); 
    lcdData(0x14);
    lcdData(0x17); 
    
    
    lcdCmd(0XE1);    //Set Gamma 
    lcdData(0xd0); 
    lcdData(0x00); 
    lcdData(0x02); 
    lcdData(0x07); 
    lcdData(0x0a); 
    lcdData(0x28); 
    lcdData(0x31); 
    lcdData(0x54); 
    lcdData(0x47); 
    lcdData(0x0e); 
    lcdData(0x1c); 
    lcdData(0x17); 
    lcdData(0x1b); 
    lcdData(0x1e); 
    
    
    lcdCmd(0x2A); 
    lcdData(0x00);
    lcdData(0x00);
    lcdData(0x00);
    lcdData(0xef);	
    
    lcdCmd(0x2B); 
    lcdData(0x00);
    lcdData(0x00);
    lcdData(0x01);
    lcdData(0x3f);
    
    lcdCmd(0x29); //display on
    lcdCmd(0x2c);   
#endif
}
#endif