#ifndef __UTILS_H__
#define __UTILS_H__

#include <8052.h>

#define	E  	P0_5
#define	WRD	P0_6
#define	RS 	P0_7

#define	LCD_RES	P0_3
#define	LCD_PSB	P0_4
#define	LCD_EP	P0_5
#define	LCD_RW	P0_6
#define	LCD_RS	P0_7

#define	IRIN	P3_2	// apparently shares with SW4
#define	K4	P3_2
#define	K3	P3_3
#define	K2	P3_4
#define	K1	P3_5
#define	BEEP	P3_6
#define	DQ	P3_7

typedef unsigned char uchar;
typedef unsigned int uint;

void delay(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 110; j++);  // 12MHz 频率下大约延时 1ms
    }
}

#endif // __UTILS_H__