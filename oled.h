#ifndef __OLED_H
#define __OLED_H	

//#include "stm32f10x.h"
#include "Arduino.h"
#include "stdlib.h"	    	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		  0x00
#define XLevelH		  0x10
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xFF 
#define X_WIDTH 	  128
#define Y_WIDTH 	  64	   

#define oled_scl  A1 
#define oled_sda  A0



/* OLED屏幕有7行 */
#define line1 0
#define line2 1
#define line3 2
#define line4 3
#define line5 4
#define line6 5
#define line7 6
#define line8 7
/* OLED屏幕有128的点,也就是16列（16*8） */
#define byte(data)  data*8
//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() digitalWrite(oled_scl,LOW)//SCL
#define OLED_SCLK_Set() digitalWrite(oled_scl,HIGH)

#define OLED_SDIN_Clr() digitalWrite(oled_sda,LOW)//SDA
#define OLED_SDIN_Set() digitalWrite(oled_sda,HIGH)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ClearBlue(void);
void OLED_ClearLine(unsigned char line);
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size);
void OLED_ShowNum(unsigned char x,unsigned char y,int num,unsigned char len,unsigned char size);
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(unsigned char x,unsigned char y,const char data[][16],unsigned char num);
void OLED_ShowCHinese7x7(unsigned char x,unsigned char y,const char data[][16],unsigned char num);
void OLED_ShowCHineseString(unsigned char x,unsigned char y,const char data[][16],unsigned char num);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);

void IIC_Wait_Ack(void);
#endif  
	 
