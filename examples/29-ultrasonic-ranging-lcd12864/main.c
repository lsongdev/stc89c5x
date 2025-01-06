// Sample program for the QX-mini51 STC89C52 development board
//
// Use LCD12864 (St7920 / St7921) + ultrasonic distance measuring module
// Wiring:
// Module TRIG is connected to P1.2 ECH0 to P1.1
// RS (CS) connect P0.7
// RW (SID) connect P0.6
// E (SCLK) connect P0.5
// PSB connected to GND serial mode
//
#include <utils.h>



#define	RX	P1_1
#define	TX	P1_2

#define	_nop_()	__asm__("nop")

__code unsigned char num[] = "0123456789 :.-";
__code unsigned char waves[] = "超声波测距系统";
__code unsigned char znwk[] = "清翔电子科技  ";
__code unsigned char CM[] = "M";
unsigned int time = 0;
long S = 0;
char flag = 0;
unsigned char disbuff[4] = { 0, 0, 0, 0, };

__code unsigned char logo[] = {
/*--  调入了一幅图像：logo.bmp  -- */
/*--  宽度x高度=128x64 欢迎使用本产品LOGO --*/
/*--  调入了一幅图像：C:\Documents and Settings\lwd\桌面\新建文件夹\X.bmp  --*/
/*--  宽度x高度=128x64  --*/
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x40, 0x70, 0x00, 0x00, 0x00, 0x80, 0x00,
	0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x60, 0x70, 0x00, 0x00, 0x0F, 0xE0, 0x00,
	0x00, 0x00, 0x01, 0xC0, 0x00, 0x60, 0x3C, 0x00, 0x00, 0xE0, 0x60, 0x00, 0x1F, 0xFF, 0xE0, 0x00,
	0x00, 0x00, 0x03, 0x80, 0x00, 0x70, 0x70, 0x40, 0x00, 0xE0, 0x7E, 0x00, 0x1F, 0xE0, 0xC0, 0x00,
	0x00, 0x00, 0x03, 0x00, 0x00, 0x73, 0xC3, 0xF0, 0x01, 0xC3, 0xFC, 0x00, 0x38, 0xE1, 0xC0, 0x00,
	0x00, 0x03, 0xC6, 0x3C, 0x00, 0x33, 0x9F, 0xF0, 0x03, 0x8F, 0xC0, 0x00, 0x30, 0xC1, 0xC0, 0x00,
	0x00, 0x3F, 0xEF, 0xFE, 0x00, 0x03, 0x1C, 0xE0, 0x07, 0x01, 0xCC, 0x00, 0x31, 0xF1, 0x80, 0x00,
	0x00, 0x79, 0xDF, 0xBC, 0x00, 0x06, 0x18, 0xC0, 0x0E, 0x01, 0xFE, 0x00, 0x77, 0xF9, 0x80, 0x00,
	0x00, 0x01, 0xB0, 0x30, 0x01, 0xC6, 0x19, 0xC0, 0x1E, 0x7F, 0xDE, 0x00, 0x6F, 0x83, 0x80, 0x00,
	0x00, 0x23, 0xAC, 0x60, 0x0F, 0xE6, 0xF1, 0x80, 0x3E, 0x7B, 0x38, 0x00, 0x61, 0x83, 0x00, 0x00,
	0x00, 0x37, 0x0E, 0x00, 0x0F, 0xC7, 0xB7, 0x80, 0xEC, 0x63, 0x30, 0x00, 0xE3, 0x03, 0x00, 0x00,
	0x00, 0x3E, 0x0E, 0x00, 0x01, 0x8F, 0x37, 0x01, 0xCC, 0x6F, 0xF0, 0x00, 0xC7, 0xF7, 0x00, 0x00,
	0x00, 0x1E, 0x0C, 0x00, 0x03, 0x1E, 0x66, 0x03, 0x0C, 0x7F, 0x00, 0x01, 0xFF, 0xE6, 0x00, 0x00,
	0x00, 0x1E, 0x1C, 0x00, 0x03, 0x1C, 0x60, 0x02, 0x19, 0xAC, 0x00, 0x01, 0x96, 0x06, 0x00, 0x00,
	0x00, 0x3E, 0x3E, 0x00, 0x03, 0x18, 0x60, 0x00, 0x19, 0xFC, 0x00, 0x03, 0x86, 0x0E, 0x00, 0x00,
	0x00, 0x7E, 0x36, 0x00, 0x03, 0x00, 0xC0, 0x00, 0x38, 0x78, 0x00, 0x03, 0x06, 0x0C, 0x00, 0x00,
	0x00, 0xE6, 0x67, 0x00, 0x03, 0x00, 0xC0, 0x00, 0x30, 0x78, 0x00, 0x06, 0x0C, 0x0C, 0x00, 0x00,
	0x01, 0xC6, 0xC3, 0x80, 0x7F, 0xF0, 0xC0, 0x00, 0x30, 0xFE, 0x00, 0x0E, 0x0C, 0x1C, 0x00, 0x00,
	0x03, 0x81, 0x83, 0xC0, 0x78, 0xFF, 0x80, 0x00, 0x61, 0xCF, 0x80, 0x1C, 0x08, 0x18, 0x00, 0x00,
	0x06, 0x07, 0x03, 0xE0, 0x00, 0x1F, 0xFF, 0xE0, 0x67, 0x07, 0xE0, 0x38, 0x18, 0xF8, 0x00, 0x00,
	0x08, 0x0E, 0x01, 0xF8, 0x00, 0x07, 0xFF, 0x80, 0xCE, 0x03, 0xF8, 0x60, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x18, 0x01, 0xF8, 0x00, 0x01, 0xFC, 0x00, 0xC0, 0x01, 0xFC, 0xC0, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0C, 0xE0, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0xFF, 0xF0, 0x00, 0x38, 0x00, 0xE0, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x00, 0x0F, 0xFF, 0x80, 0x00, 0xF8, 0xF0, 0x00, 0x38, 0x00, 0xE0, 0x00,
	0x00, 0x00, 0x0C, 0xF0, 0x00, 0x0F, 0xC7, 0x00, 0x00, 0x60, 0xE0, 0x00, 0x38, 0x00, 0xE0, 0x00,
	0x00, 0x00, 0x1F, 0xF0, 0x00, 0x01, 0x8E, 0x00, 0x00, 0x60, 0xC0, 0x00, 0x70, 0x01, 0xC0, 0x00,
	0x00, 0x07, 0xFF, 0x80, 0x00, 0x01, 0x8C, 0x00, 0x00, 0xE1, 0x80, 0x00, 0x70, 0x01, 0xC0, 0x00,
	0x00, 0x07, 0xF8, 0x00, 0x00, 0x01, 0x98, 0x00, 0x00, 0xDF, 0xC0, 0x00, 0x70, 0x01, 0xC0, 0x00,
	0x00, 0x00, 0x78, 0x00, 0x00, 0x01, 0xBF, 0xE0, 0x00, 0xFF, 0x00, 0x00, 0x60, 0x01, 0x80, 0x00,
	0x00, 0x00, 0xFC, 0x00, 0x00, 0x7F, 0xFF, 0xE0, 0x00, 0xC0, 0x00, 0x00, 0xE0, 0x03, 0x80, 0x00,
	0x00, 0x01, 0xFE, 0x00, 0x00, 0x7F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00,
	0x00, 0x03, 0xB7, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0xC0, 0x03, 0x00, 0x00,
	0x00, 0x07, 0x67, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03, 0x9F, 0xFE, 0x00, 0x80, 0x02, 0x00, 0x00,
	0x00, 0x0E, 0x63, 0x80, 0x00, 0xE0, 0x00, 0x01, 0xFF, 0xCF, 0x1E, 0x01, 0x80, 0x06, 0x00, 0x00,
	0x00, 0x1C, 0xE1, 0xE0, 0x00, 0xC0, 0x00, 0x00, 0xE3, 0x9C, 0x38, 0x01, 0x80, 0x06, 0x00, 0x00,
	0x00, 0x38, 0xC1, 0xF8, 0x01, 0xC0, 0x00, 0x00, 0xC3, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xE0, 0xFC, 0xFE, 0x03, 0x80, 0x00, 0x01, 0xC7, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0xDF, 0xF8, 0xFE, 0x03, 0x00, 0x00, 0x01, 0x8E, 0x1B, 0xE0, 0x07, 0x00, 0x1C, 0x00, 0x00,
	0x07, 0x07, 0x80, 0x00, 0x07, 0x00, 0x00, 0x01, 0xFF, 0x3F, 0xE0, 0x07, 0x00, 0x1C, 0x00, 0x00,
	0x0C, 0x03, 0x80, 0x00, 0x0E, 0x00, 0x00, 0x01, 0xF0, 0x38, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x00,
	0x00, 0x03, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x06, 0x00, 0x18, 0x00, 0x00,
	0x00, 0x03, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x03, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// Timer0 interrupt is used for range overflow
void zd0(void) __interrupt(1)
{
	flag = 1;
}

void Delaynms(unsigned int di)
{
	unsigned int da, db;

	for (da = 0; da < di; da++)
		for (db = 0; db < 10; db++);
}

void delayms(unsigned int ms)
{
	unsigned char i = 100, j;

	for (; ms; ms--) {
		while (--i) {
			j = 10;
			while (--j);
		}
	}
}

// Send a byte
void Send_byte(unsigned char bbyte)
{
	unsigned char i;

	for (i = 0; i < 8; i++) {
		LCD_RW = bbyte & 0x80;
		LCD_EP = 1;
		LCD_EP = 0;
		bbyte <<= 1;
	}
}

// Write command or data
void Write_char(unsigned char start, unsigned char ddata)
{
	unsigned char start_data, Hdata, Ldata;

	if (start == 0)
		start_data = 0xf8;	// command
	else
		start_data = 0xfa;	// data
	Hdata = ddata & 0xf0;		// high nybble
	Ldata = (ddata << 4) & 0xf0;	// low nybble
	Send_byte(start_data);
	Delaynms(5);			// wait
	Send_byte(Hdata);		// high nybble
	Delaynms(1);
	Send_byte(Ldata);		// low nybble
	Delaynms(1);
}

void Lcd_init(void)
{
	LCD_PSB = 0;
	Delaynms(10);		// wait for LCM
	LCD_RS = 1;
	Write_char(0, 0x30);	// 8-bit interface, basic set
	Write_char(0, 0x0c);	// enable display, cursor off, reverse white
	Write_char(0, 0x01);	// clear screen reset address counter
}

// 12864 graphics mode display font 221 landscape mode
void Disp_img(unsigned char *img)
{
	unsigned char i, j;
	unsigned int k = 0;

	Write_char(0, 0x36);	// graphics mode
	for (i = 0; i < 32; i++) {
		Write_char(0, 0x80 + i);
		Write_char(0, 0x80);
		for (j = 0; j < 16; j++) {
			Write_char(1, img[k++]);
		}
	}
	for (i = 0; i < 32; i++) {
		Write_char(0, 0x80 + i);
		Write_char(0, 0x88);
		for (j = 0; j < 16; j++) {
			Write_char(1, img[k++]);
		}
	}
}

// Clear screen
void Clr_Scr(void)
{
	Write_char(0, 0x01);
}

// Set start position as row, column
void LCD_set_xy(unsigned char x, unsigned char y)
{
	unsigned char address;

	switch (x) {
	case 0:
		address = 0x80 + y;
		break;
	case 1:
		address = 0x80 + y;
		break;
	case 2:
		address = 0x90 + y;
		break;
	case 3:
		address = 0x88 + y;
		break;
	case 4:
		address = 0x98 + y;
		break;
	default:
		address = 0x80 + y;
		break;
	}
	Write_char(0, address);
}

// Chinese and English string display
void LCD_Write_string(unsigned char X, unsigned char Y, unsigned char *s)
{
	LCD_set_xy(X, Y);
	while (*s) {
		Write_char(1, *s);
		s++;
		Delaynms(1);
	}
}

// Digital display function
void LCD_Write_number(unsigned char s)
{
	Write_char(1, num[s]);
	Delaynms(1);
}

void Lcd_Mark2(void)
{
	Clr_Scr();
	LCD_Write_string(1, 0, znwk);
	LCD_Write_string(2, 0, waves);
	LCD_Write_string(3, 7, CM);
}

void Count(void)
{
	time = TH0 * 256 + TL0;
	TH0 = 0;
	TL0 = 0;
	S = time * 1.87 / 100;	// in cm for 11.0592 MHz crystal
	if (flag == 1) {	// overflow
		flag = 0;
		LCD_set_xy(3, 4);
		LCD_Write_number(13);
		LCD_Write_number(12);
		LCD_set_xy(3, 5);
		LCD_Write_number(13);
		LCD_Write_number(13);

	} else {
		disbuff[1] = S % 1000 / 100;
		disbuff[2] = S % 1000 % 100 / 10;
		disbuff[3] = S % 1000 % 10 % 10;
		LCD_set_xy(3, 4);
		LCD_Write_number(disbuff[1]);
		LCD_Write_number(12);
		LCD_set_xy(3, 5);
		LCD_Write_number(disbuff[2]);
		LCD_Write_number(disbuff[3]);
	}
}

// Timer1 interrupt is used to multiplex display and meter 800 ms start
void StartModule(void)
{
	TX = 1;		// 800 ms starts module once
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	TX = 0;
}

void main(void)
{
	TMOD = 0x01;	// timer0 mode 1, gate = 1
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;
	Lcd_init();	// initialise LCD
	Clr_Scr();	// clear screen
	Disp_img(logo);	// welcome message
	Delaynms(10000);
	Lcd_init();	// back to graphics mode
	Lcd_Mark2();
	while (1) {
		StartModule();	// calculate
		while (!RX);	// wait for RX == 0
		TR0 = 1;	// start counting
		while (RX);	// until RX == 1
		TR0 = 0;	// turn off counting
		Count();
		delayms(80);
	}
}
