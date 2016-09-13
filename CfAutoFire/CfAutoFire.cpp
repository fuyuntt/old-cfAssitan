// CfAutoFire.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>
#include <stdio.h>

#include "KeyboardMouse.h"
#include "Screen.h"
#include "time.h"
#include "weapon.h"

extern RECT rectAim;

#if SCREEN_WIDTH == 800


#elif SCREEN_WIDTH == 1024

const WEAPON SNIPE_LIST[] = {AWM};
const WEAPON LIGHT_SNIPE_LIST[] = {FN_NUPAO, WA2000};
const WEAPON RIFLE_LIST[] = {AK47, AUG};
const WEAPON SUBMACHINE_LIST[] = {_9A91, MP5KA4, MP5, G11ANJIN, SHITAIER, P90};
const WEAPON SHOOT_GUN_LIST[] = {DESERT_EAGLE, BEILEITA687, XM1014, USP, GLOCK};
const WEAPON SHOOT_GUN2_LIST[] = {DAJIZHE_TIANYU};

#define LIGHT_SNIPE_NUM (sizeof(LIGHT_SNIPE_LIST) / 4)
#define SUBMACHINE_NUM (sizeof(SUBMACHINE_LIST) / 4)
#define SHOOT_GUN_NUM (sizeof(SHOOT_GUN_LIST) / 4)
#define SHOOT_GUN2_NUM (sizeof(SHOOT_GUN2_LIST) / 4)

#endif

void StickingPoint();

void monitorWeapon();
void moveDetect();
void jump();

volatile SHOOT_KIND shoot_kind = EMPTY;
volatile BOOL ISMOVE = true;

void testKey()
{
	while (true)
	{
		if(GetKeyState(VK_TAB) & 0x8000)
		{
			printf("TAB\n");
			while(GetKeyState(VK_TAB));
		}
		if(GetKeyState('E') & 0x8000)
		{
			printf("E\n");
			while(GetKeyState('E'));
		}
		if(GetKeyState(VK_SPACE) & 0x8000)
		{
			printf("SPACE\n");
			while(GetKeyState(VK_SPACE));
		}
		if(GetKeyState(VK_SHIFT) & 0x8000)
		{
			printf("SHIFT\n");
			while(GetKeyState(VK_SHIFT));
		}
		if(GetKeyState('1') & 0x8000)
		{
			printf("1\n");
			while(GetKeyState('1'));
		}
		if(GetKeyState(VK_RBUTTON) & 0x8000)
		{
			printf("RBUTTON\n");
			while(GetKeyState(VK_RBUTTON));
		}
		if(GetKeyState(VK_LBUTTON) & 0x8000)
		{
			printf("VK_LBUTTON\n");
			while(GetKeyState(VK_LBUTTON));
		}
		if(GetKeyState(VK_MBUTTON) & 0x8000)
		{
			printf("VK_MBUTTON\n");
			while(GetKeyState(VK_MBUTTON));
		}
		if(GetKeyState(VK_MENU) & 0x8000)
		{
			printf("VK_MENU\n");
			while(GetKeyState(VK_MENU));
		}
		if(GetKeyState(VK_LCONTROL) & 0x8000)
		{
			printf("VK_LCONTROL\n");
			while(GetKeyState(VK_LCONTROL));
		}
		Sleep(20);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	DWORD preTime;
	DWORD ThreadID;
	BOOL isButtonDown = false;

	if(!InitDevice())
	{
		system("pause");
		exit(0);
	}
	srand((unsigned int)(time(NULL)));

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)monitorWeapon, NULL, 0, &ThreadID);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveDetect, NULL, 0, &ThreadID);
//	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)drawFrontSight, NULL, 0, &ThreadID);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)jump, NULL, 0, &ThreadID);
//	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)testKey, NULL, 0, &ThreadID);

	preTime = GetTickCount();
	while(true)
	{
		//狙击步枪
		while(shoot_kind == SNIPE)
		{
			if(isInRange() && isZoom())
			{
				if (!ISMOVE)
				{
					StickingPoint();
				}
				MouseClick(LEFT);
				Sleep(150);
				MouseWheel(-1);
				Sleep(200);
				if(!(GetKeyState(VK_CAPITAL) & 0x01))
				{
					MouseWheel(1);
				}
				Sleep(500);
			}
			else if (GetKeyState(VK_LBUTTON) & 0x8000)
			{
				Sleep(150);
				MouseWheel(-1);
				Sleep(200);
				if(!(GetKeyState(VK_CAPITAL) & 0x01))
				{
					MouseWheel(1);
				}
				Sleep(500);
			}
		}	
		//轻狙
		while(shoot_kind == LIGHT_SNIPE)
		{
			if(isInRange() && isZoom())
			{
				MouseClick(LEFT);
				Sleep(50);
			}
		}
		//冲锋枪
		while(shoot_kind == SUBMACHINE || isButtonDown)
		{
			if (!isButtonDown && isInRange() && (GetKeyState(VK_RBUTTON) & 0x8000))
			{
				MouseDown(LEFT);
				isButtonDown = true;
			}
			if(isButtonDown && !isInRange())
			{
				MouseUp(LEFT);
				isButtonDown = false;
				Sleep(180);
			}
		}
		//普通步枪
		int i = 0;
		while (shoot_kind == RIFLE)
		{
			if(isInRange() && (GetKeyState(VK_RBUTTON) & 0x8000))
			{
				MouseClick(LEFT);
				if(GetTickCount() - preTime < 250)
				{
					i++;
				}
				else
				{
					i = 0;
				}
				Sleep(60);
				preTime = GetTickCount();
			}
			if(i > 5)
			{
				i = 0;
				Sleep(300);
			}

		}
		//单发类枪支如散弹枪或手枪
		while (shoot_kind == SHOOT_GUN)
		{
			if(isInRange() && (GetKeyState(VK_RBUTTON) & 0x8000))
			{
				MouseClick(LEFT);
				Sleep(30);
				if(!isBingo())
				{
					Sleep(250);
				}
				else
				{
					Sleep(150);
				}
			}
		}
		//打击者类散弹枪
		bool isFull = false;
		while (shoot_kind == SHOOT_GUN2)
		{
			if(isInRange())
			{
				MouseClick(LEFT);
				isFull = false;
				preTime = GetTickCount();
				Sleep(100);
			}
			if(GetTickCount() - preTime > 1000 && isFull == false)
			{
				KeyPress('R');
				isFull = true;
			}
		}
		Sleep(100);
	}
}

void monitorWeapon()
{
	WEAPON weapon,preWeapon;
	int i;
	while(true)
	{
		if(!(GetKeyState(VK_CAPITAL) & 0x01))
		{
			weapon = getWeapon();
			for(i = 0; i < LIGHT_SNIPE_NUM; i++)
			{
				if(LIGHT_SNIPE_LIST[i] == weapon)
				{
					shoot_kind = LIGHT_SNIPE;
					break;
				}
			}
			if(i != LIGHT_SNIPE_NUM) continue;

			for(i = 0; i < SUBMACHINE_NUM; i++)
			{
				if(SUBMACHINE_LIST[i] == weapon)
				{
					shoot_kind = SUBMACHINE;
					break;
				}
			}
			if(i != SUBMACHINE_NUM) continue;
		
			for(i = 0; i < SHOOT_GUN_NUM; i++)
			{
				if(SHOOT_GUN_LIST[i] == weapon)
				{
					shoot_kind = SHOOT_GUN;
					break;
				}
			}
			if(i != SHOOT_GUN_NUM) continue;

			for(i = 0; i < SHOOT_GUN2_NUM; i++)
			{
				if(SHOOT_GUN2_LIST[i] == weapon)
				{
					shoot_kind = SHOOT_GUN2;
					break;
				}
			}
			if(i != SHOOT_GUN2_NUM) continue;

			if (weapon != 0)
			{
				if(isBingo())
				{
					shoot_kind = SNIPE;
				}
				else
				{
					shoot_kind = RIFLE;
				}
			}
			else
			{
				shoot_kind = EMPTY;
			}
			preWeapon = weapon;
			while ((weapon = getWeapon()) == preWeapon)
			{
				if(!isBingo()) 
					shoot_kind = RIFLE;
				preWeapon = weapon;
				Sleep(100);
			}
		}
		else
		{
			shoot_kind = EMPTY;
		}
	}
}

void moveDetect()
{
	BYTE *pPreBuf, *pCurBuf;
	int lineWidth;
	int i, j;
	pPreBuf = CaptureScreen(rectAim, &lineWidth);
	while(true)
	{
		for (i = 0; i < 10; i++)
		{
			//移动监测时间间隔
			Sleep(100);
			pCurBuf = CaptureScreen(rectAim, NULL);
			for(j = 0; j < lineWidth * (rectAim.bottom - rectAim.top + 1); j++)
			{
				if(pCurBuf[j] - pPreBuf[j] != 0) break;
			}
			if(j < lineWidth * (rectAim.bottom - rectAim.top + 1)) 
			{
				free(pPreBuf);
				pPreBuf = pCurBuf;
				break;
			}
			free(pPreBuf);
			pPreBuf = pCurBuf;
		}
		if(i == 10)
		{
			ISMOVE = false;
		}
		else
		{
			ISMOVE = true;
		}
	}
}

void jump()
{
	DWORD preTime;

	while (true)
	{
		if(GetKeyState(VK_MENU) & 0x8000)
		{
			preTime = GetTickCount();
			while ((GetKeyState(VK_MENU) & 0x8000) && GetTickCount() - preTime < 400)
			{
				Sleep(5);
			}
			if(GetKeyState(VK_MENU) & 0x8000)
			{
				KeyPress(VK_SPACE);
				Sleep(300);
				KeyDown(VK_LCONTROL);
				Sleep(310);
				while(GetKeyState(VK_MENU) & 0x8000)
				{
					KeyPress(VK_SPACE);
					Sleep(600);
				}
				KeyUp(VK_LCONTROL);
			}
			else
			{
				KeyPress(VK_SPACE);
				Sleep(580);
				KeyPress(VK_SPACE);
			}
		}
		Sleep(20);
	}
}

void StickingPoint()
{
	PBYTE pPreBuf, pCurBuf;
	int lineWidth;
	int i;

	pPreBuf = CaptureScreen(rectAim, &lineWidth);
	while (true)
	{
		pCurBuf = CaptureScreen(rectAim, NULL);
		for (i = 0; i < lineWidth * (rectAim.bottom - rectAim.top + 1); i++)
		{
			if(pCurBuf[i] - pPreBuf[i] != 0) break;
		}
		free(pPreBuf);
		pPreBuf = pCurBuf;
		if(i != lineWidth * (rectAim.bottom - rectAim.top + 1) ) 
		{
			free(pCurBuf);
			return;
		}
	}
}

void drawFrontSight()
{
	HDC hdc,memDC;
	HWND wnd;
//	wnd = FindWindow(NULL, L"crossfire");
	wnd = GetDesktopWindow();
	hdc = GetWindowDC(wnd);
	memDC = CreateCompatibleDC(hdc);
	MoveToEx(memDC, 499, 384, NULL);
	LineTo(memDC, 525, 384);
	MoveToEx(memDC, 512, 371, NULL);
	LineTo(memDC, 512, 397);
	while (true)
	{
	}
	ReleaseDC(wnd, hdc);
}