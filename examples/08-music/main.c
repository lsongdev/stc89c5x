// Sample program for the QX-mini51 STC89C52 development board
//
// Play a song called August Laurel
// J1 must be closed to supply the buzzer
//
#include <utils.h>

unsigned char n = 0;

// Format is pitch constant, duration constant, etc.
__code unsigned char music_tab[] = {
	0x18, 0x30, 0x1C, 0x10,
	0x20, 0x40, 0x1C, 0x10,
	0x18, 0x10, 0x20, 0x10,
	0x1C, 0x10, 0x18, 0x40,
	0x1C, 0x20, 0x20, 0x20,
	0x1C, 0x20, 0x18, 0x20,
	0x20, 0x80, 0xFF, 0x20,
	0x30, 0x1C, 0x10, 0x18,
	0x20, 0x15, 0x20, 0x1C,
	0x20, 0x20, 0x20, 0x26,
	0x40, 0x20, 0x20, 0x2B,
	0x20, 0x26, 0x20, 0x20,
	0x20, 0x30, 0x80, 0xFF,
	0x20, 0x20, 0x1C, 0x10,
	0x18, 0x10, 0x20, 0x20,
	0x26, 0x20, 0x2B, 0x20,
	0x30, 0x20, 0x2B, 0x40,
	0x20, 0x20, 0x1C, 0x10,
	0x18, 0x10, 0x20, 0x20,
	0x26, 0x20, 0x2B, 0x20,
	0x30, 0x20, 0x2B, 0x40,
	0x20, 0x30, 0x1C, 0x10,
	0x18, 0x20, 0x15, 0x20,
	0x1C, 0x20, 0x20, 0x20,
	0x26, 0x40, 0x20, 0x20,
	0x2B, 0x20, 0x26, 0x20,
	0x20, 0x20, 0x30, 0x80,
	0x20, 0x30, 0x1C, 0x10,
	0x20, 0x10, 0x1C, 0x10,
	0x20, 0x20, 0x26, 0x20,
	0x2B, 0x20, 0x30, 0x20,
	0x2B, 0x40, 0x20, 0x15,
	0x1F, 0x05, 0x20, 0x10,
	0x1C, 0x10, 0x20, 0x20,
	0x26, 0x20, 0x2B, 0x20,
	0x30, 0x20, 0x2B, 0x40,
	0x20, 0x30, 0x1C, 0x10,
	0x18, 0x20, 0x15, 0x20,
	0x1C, 0x20, 0x20, 0x20,
	0x26, 0x40, 0x20, 0x20,
	0x2B, 0x20, 0x26, 0x20,
	0x20, 0x20, 0x30, 0x30,
	0x20, 0x30, 0x1C, 0x10,
	0x18, 0x40, 0x1C, 0x20,
	0x20, 0x20, 0x26, 0x40,
	0x13, 0x60, 0x18, 0x20,
	0x15, 0x40, 0x13, 0x40,
	0x18, 0x80, 0x00
};

void int0(void) __interrupt(1)
{
	TH0 = 0xd8;
	TL0 = 0xef;
	n--;
}

// void delay(unsigned char m)
// {
// 	unsigned i = 3 * m;
// 	while (--i);
// }

void delayms(unsigned char a)
{
	while (--a);
}

void main(void)
{
	unsigned char p, m;
	unsigned char i = 0;

	TMOD &= 0x0f;
	TMOD |= 0x01;
	TH0 = 0xd8;
	TL0 = 0xef;
	IE = 0x82;
      play:
	while (1) {
	      a:p = music_tab[i];
		if (p == 0x00) {	// At end, delay 1s, then play again
			i = 0;
			delayms(1000);
			goto play;
		} else if (p == 0xff) {	// At rest, delay 100ms, then continue
			i = i + 1;
			delayms(100);
			TR0 = 0;
			goto a;
		} else {
			m = music_tab[i++];	// Read pitch and duration
			n = music_tab[i++];
		}
		TR0 = 1;
		while (n != 0) {	// Play note for indicated duration
			BEEP ^= 1;
			delay(m);
		}
		TR0 = 0;
	}
}
