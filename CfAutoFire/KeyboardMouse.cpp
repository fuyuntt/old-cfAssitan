/****************************
*键鼠操作库函数
*
****************************/
#include "stdafx.h"

#include "KeyboardMouse.h"
#include "stdio.h"

static int(*openDevice)();
static int(*keyDown)(LPCSTR key);
static int(*keyUp)(LPCSTR key);
static int(*keyPress)(LPCSTR key, int count);
static int(*leftDown)();
static int(*leftUp)();
static int(*leftClick)(int count);
static int(*rightDown)();
static int(*rightUp)();
static int(*rightClick)(int count);
static int(*wheelsMove)(int y);

BOOL InitDevice()
{
	HINSTANCE hDllInst = LoadLibrary(L"kmllib.dll");
	if (!hDllInst)
	{
		printf("load library failed.\n");
		return false;
	}
	openDevice = (int (*)())GetProcAddress(hDllInst, "OpenDevice");
	keyDown = (int (*)(LPCSTR key))GetProcAddress(hDllInst, "KeyDown");
	keyUp = (int (*)(LPCSTR key))GetProcAddress(hDllInst, "KeyUp");
	keyPress = (int (*)(LPCSTR key, int count))GetProcAddress(hDllInst, "KeyPress");
	leftDown = (int (*)())GetProcAddress(hDllInst, "LeftDown");
	leftUp = (int (*)())GetProcAddress(hDllInst, "LeftUp");
	leftClick = (int (*)(int count))GetProcAddress(hDllInst, "LeftClick");
	rightDown = (int (*)())GetProcAddress(hDllInst, "rightDown");
	rightUp = (int (*)())GetProcAddress(hDllInst, "rightUp");
	rightClick = (int (*)(int count))GetProcAddress(hDllInst, "rightClick");
	wheelsMove = (int (*)(int y))GetProcAddress(hDllInst, "WheelsMove");

	if (!openDevice())
	{
		printf("初始化设备失败\n");
		return false;
	}
	printf("初始化设备成功\n");
	return true;
}

BOOL KeyDown(LPCSTR key)
{
	return keyDown(key);
}
BOOL KeyUp(LPCSTR key)
{
	return keyUp(key);
}
BOOL KeyPress(LPCSTR key, int count)
{
	return keyPress(key, count);
}
BOOL LeftUp()
{
	return leftUp();
}
BOOL LeftDown()
{
	return leftDown();
}
BOOL LeftClick(int count)
{
	return leftClick(count);
}
BOOL RightUp()
{
	return rightUp();
}
BOOL RightDown()
{
	return rightDown();
}
BOOL RightClick(int count)
{
	return rightClick(count);
}
BOOL WheelsMove(int y)
{
	return wheelsMove(y);
}
