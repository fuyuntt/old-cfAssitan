#ifndef KEYBOARD_MOUSE_H_
#define KEYBOARD_MOUSE_H_

#include <windows.h>

BOOL InitDevice();

//¼üÊó²Ù×÷º¯Êý
BOOL KeyDown(LPCSTR key);
BOOL KeyUp(LPCSTR key);
BOOL KeyPress(LPCSTR key, int count);
BOOL LeftUp();
BOOL LeftDown();
BOOL LeftClick(int count);
BOOL RightUp();
BOOL RightDown();
BOOL RightClick(int count);
BOOL WheelsMove(int y);

#endif