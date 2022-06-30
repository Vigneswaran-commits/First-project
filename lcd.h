#ifndef _LCD_H_
#define _LCD_H_

/*
 *	LCD interface header file
 */

/* 	Defining CHECKBUSY will check if the LCD is busy. The RW bit of the 
 * 	LCD must connected to a port of the processor for the check busy
 * 	process to work.
 * 
 * 	If CHECKBUSY is not defined it will instead use a delay loop.
 * 	The RW bit of the LCD does not need to connected in this case.
 */

// #define CHECKBUSY	1

#ifdef CHECKBUSY
	#define	LCD_WAIT lcd_check_busy()
#else
	#define LCD_WAIT DelayMs(5)

#endif

#define MESSAGE_LINE		0x0

#define LCD_RS	LB3
#define LCD_EN	LB1
#define LCD_RW	LB2
#define LCD_DATA	LATD
#define LCD_DATA_PORT	PORTD 
#define LCD_RS_TRIS	TRISB3
#define LCD_EN_TRIS	TRISB1
#define LCD_RW_TRIS	TRISB2
#define LCD_DATA_TRIS	TRISD

#define FOURBIT_MODE	0x0
#define EIGHTBIT_MODE	0x1
#define OUTPUT_PIN      0x0	
#define INPUT_PIN       0x1	
#define OUTPUT_DATA     0xF0	
#define INPUT_DATA      0xFF	

#define LCD_STROBE()	LCD_EN = 1; asm("nop"); asm("nop"); LCD_EN = 0

#define LCD_STROBE_READ(value)	LCD_EN = 1; \
				asm("nop"); asm("nop"); \
				value=LCD_DATA_PORT; \
				LCD_EN = 0; 

#define	lcd_cursor(x)			lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()			lcd_cmd(0x1)
#define lcd_putch(x)			lcd_data(x)
#define lcd_goto(x)			lcd_cmd(0x80+(x));
#define lcd_cursor_right()		lcd_cmd(0x14)
#define lcd_cursor_left()		lcd_cmd(0x10)
#define lcd_display_shift()		lcd_cmd(0x1C)
#define lcd_home()			lcd_cmd(0x2)

extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char * s);
extern void lcd_putsint(int iValue);
extern void lcd_putsTwoint(int iFirstValue,int iSecondValue);
extern void lcd_putsfloat(float fValue);
extern void lcd_init(unsigned char);

#endif

