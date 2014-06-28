/*
	Author	:		Pranjal Joshi
	Date		:		15/6/2014
	Compiler:		Keil uV4
	This header file is used for Arduino similar programming for 8051/8052 microcontrollers that allow easy
	and arduino style programming, but it will increase code size & may affect efficiency. Not recommended
	for timing critical applications or applications that require extremely fast program execution.
	This is open source software.
*/

// SPECIAL HEADER FILE FOR 4-BIT LCD INTERFACE

#ifndef __ARDU51_LCD_H__
#define __ARDU51_LCD_H__

#include <arduino51.h>

// Function definations :
 
#define LCD_EN 0x20
#define LCD_RS 0x10

unsigned char COUNTER;
char lcdbuf[8] = {0,0,0,0,0,0,0,0};
char lcdbufInv[8] = {0,0,0,0,0,0,0,0};
unsigned char lcdcnt = 0;
unsigned char lcdcntInv = 0;
 
void lcd_cmd(char cmd)
{
lcd_port = ((cmd >> 4) & 0x0F)|LCD_EN;
lcd_port = ((cmd >> 4) & 0x0F);
 
lcd_port = (cmd & 0x0F)|LCD_EN;
lcd_port = (cmd & 0x0F);
 
delay(2);
}
 
void lcd_data(unsigned char dat)
{
lcd_port = (((dat >> 4) & 0x0F)|LCD_EN|LCD_RS);
lcd_port = (((dat >> 4) & 0x0F)|LCD_RS);
lcd_port = ((dat & 0x0F)|LCD_EN|LCD_RS);
lcd_port = ((dat & 0x0F)|LCD_RS);
 
delay(2);
}

void lcd_reset()
{
lcd_port = 0xFF;
delay(20);
lcd_port = 0x03+LCD_EN;
lcd_port = 0x03;
delay(10);
lcd_port = 0x03+LCD_EN;
lcd_port = 0x03;
delay(1);
lcd_port = 0x03+LCD_EN;
lcd_port = 0x03;
delay(1);
lcd_port = 0x02+LCD_EN;
lcd_port = 0x02;
delay(1);
}
 
void lcd_begin()
{
lcd_reset(); // Call LCD reset
lcd_cmd(0x28); // 4-bit mode - 2 line - 5x7 font.
lcd_cmd(0x0C); // Display no cursor - no blink.
lcd_cmd(0x06); // Automatic Increment - No Display shift.
lcd_cmd(0x80); // Address DDRAM with 0 offset 80h.
}

void lcd_print(unsigned char *str)
{
	unsigned int x;
	for(x=0;str[x]!=0;x++)
		lcd_data(str[x]);
}

void lcd_println(unsigned char *str)
{
	unsigned int x;
	for(x=0;str[x]!=0;x++)
		lcd_data(str[x]);
	lcd_cmd(0xC0);
}

void lcd_clear()
{
	lcd_cmd(0x01);
}

void lcd_cursorOn()
{
	lcd_cmd(0x0E);
}

void lcd_cursorOff()
{
	lcd_cmd(0x0C);
}

void lcd_cursorBlink()
{
	lcd_cmd(0x0F);
}

void lcd_displayOff()
{
	lcd_cmd(0x08);
}

void lcd_displayOn()
{
	lcd_cmd(0x0C);
}

void lcd_shiftLeft(unsigned char howMany, unsigned int DelayMS)
{
	for(COUNTER=0;COUNTER<howMany;COUNTER++)
	{
		lcd_cmd(0x18);
		delay(DelayMS);
	}
}

void lcd_shiftRight(unsigned char howMany, unsigned int DelayMS)
{
	for(COUNTER=0;COUNTER<howMany;COUNTER++)
	{
		lcd_cmd(0x1C);
		delay(DelayMS);
	}
}

void lcd_setCursor(unsigned char row, unsigned char coloumn)
{
	if(row == 1)
		COUNTER = 0x80;
	else if(row == 2)
		COUNTER = 0xC0;
	lcd_cmd(COUNTER + coloumn - 1);
}

void lcd_printInt(unsigned long num)
{
	lcdcnt = 0;
	lcdcntInv = 0;
	if(num != 0)
	{
		while(num > 0)
		{
			lcdbuf[lcdcnt] = (num % 10) + 48;
			num /= 10;
			lcdcnt++;
		}
		while(lcdcnt > 0)
		{
			lcdcnt--;
			lcdbufInv[lcdcntInv] = lcdbuf[lcdcnt];
			lcdcntInv++;
		}
		lcd_print(lcdbufInv);
	}
	else
	{
		lcd_print("0");
	}
}

#endif
