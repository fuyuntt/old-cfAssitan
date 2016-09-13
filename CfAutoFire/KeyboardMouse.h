#ifndef KEYBOARD_MOUSE_H_
#define KEYBOARD_MOUSE_H_

#include <windows.h>

#define VID 0x0483
#define PID 0x5750

typedef enum BUTTON
{
	LEFT = 1,
	RIGHT,
	MEDDLE
};

BOOL InitDevice();

//¼üÊó²Ù×÷º¯Êý
BOOL KeyDown(UCHAR vKey);
BOOL KeyUp(UCHAR vKey);
BOOL KeyPress(UCHAR vKey);
BOOL MouseUp(BUTTON btn);
BOOL MouseDown(BUTTON btn);
BOOL MouseClick(BUTTON btn);
BOOL MouseMove(int x, int y);
BOOL MouseWheel(int vol);

#endif