#include "stdafx.h"

#include "Screen.h"

#if SCREEN_WIDTH == 800
RECT rectAim = {397, 297, 403, 303};
RECT rectPeopleName = {370, 344, 427, 355};
RECT rectWeaponName = {590, 523, 779, 539};
RECT rectFrontSight = {370, 300, 400, 300};
RECT rectSnip = {325, 301, 327, 301};
#elif SCREEN_WIDTH == 1024
RECT rectAim = {507, 379, 517, 389};
RECT rectPeopleName = {479, 425, 546, 440};
RECT rectWeaponName = {816, 695, 1000, 704};
RECT rectFrontSight = {380, 384, 511, 384};
RECT rectSnip = {470, 385, 472, 385};
#endif

PBYTE CaptureScreen(RECT rect, OUT PINT bufferLineWidth)
{
	int Width, Height, lineWidth;
    HWND Wnd;
	int size;
	PBYTE pBits;
	HDC DC, MemDC;
	HBITMAP Bitmap,OldBitmap;
	BITMAPINFO bitmapInfo;
	
    Width = rect.right - rect.left + 1;
    Height = rect.bottom - rect.top + 1;
	//有BUG后注意这句话
	lineWidth = (Width * 3 + 3) & 0x7FFFFFFC;

	size = lineWidth * Height;
	if(bufferLineWidth != NULL)
	{
		*bufferLineWidth = lineWidth;
	}
	Wnd = GetDesktopWindow();
	DC = GetWindowDC(Wnd);
	MemDC = CreateCompatibleDC(DC);
	Bitmap = CreateCompatibleBitmap(DC, Width, Height);
	OldBitmap = (HBITMAP)SelectObject(MemDC, Bitmap);
	BitBlt(MemDC, 0, 0, Width, Height, DC, rect.left, rect.top, SRCCOPY);
	Bitmap = (HBITMAP)SelectObject(MemDC,OldBitmap);

	bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
	bitmapInfo.bmiHeader.biWidth = Width;
	bitmapInfo.bmiHeader.biHeight = Height;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	
	pBits = (UCHAR*) malloc(size);
	if(pBits == NULL) return NULL;
	GetDIBits(DC, Bitmap, 0, Height, pBits, &bitmapInfo, DIB_RGB_COLORS);
	
    DeleteDC(MemDC);
    DeleteObject(Bitmap);
    DeleteObject(OldBitmap);
    ReleaseDC(Wnd,DC);

	return pBits;
}

BOOL isInRange()
{	
	UCHAR *pBuf;
	COLOR_BGR *pColor;
	int lineWidth;
	int RCount,BCount;
	RCount = 0;
	BCount = 0;
	pBuf = (UCHAR*) CaptureScreen(rectPeopleName, &lineWidth);
	for (int x = 0; x < rectPeopleName.right - rectPeopleName.left + 1; x++)
	{
		for (int y = 0; y < rectPeopleName.bottom - rectPeopleName.top + 1; y++)
		{
			pColor = (COLOR_BGR*)&pBuf[y * lineWidth + x * 3];
			if(pColor->B == 0 && pColor->G == 0 && pColor->R == 0) BCount++;
			if(pColor->B < 60 && pColor->G < 80 && pColor->R > 150) RCount++;
		}
	}
	free(pBuf);
	if(BCount > 15 && RCount > 20) return true;
	return false;
}

BOOL isZoom()
{
	UCHAR *pBuf;
	int lineWidth;
	int i;
	pBuf = (UCHAR*)CaptureScreen(rectSnip, &lineWidth);
	for(i = 0; i < lineWidth; i++)
	{
		if(pBuf[i])
		{
			free(pBuf);
			return false;
		}
	}
	free(pBuf);
	return true;
}
BOOL isSniper()
{
	UCHAR *pBuf;
	int i,count;
	count = 0;
	pBuf = (UCHAR*)CaptureScreen(rectFrontSight, NULL);
	for(i = 0; i < rectFrontSight.right - rectFrontSight.left; i++)
	{	
		if(pBuf[3*i] == pBuf[3*(i+1)] && pBuf[3*i + 1] == pBuf[3*(i+1) + 1] && pBuf[3*i + 2] == pBuf[3*(i+1) + 2])
		{
			count++;
		}
		else
		{
			count = 0;
		}
		if(count >= 4)
		{
			free(pBuf);
			return false;
		}
	}
	free(pBuf);
	return true;
}
BOOL isBingo()
{
	UCHAR *pBuf;
	COLOR_BGR* pColor;
	int i,count;
	count = 0;
	pBuf = (UCHAR*)CaptureScreen(rectFrontSight, NULL);
	for(i = 0; i < rectFrontSight.right - rectFrontSight.left + 1; i++)
	{	
		pColor = (COLOR_BGR*)&pBuf[3 * i];
		if(pColor->R == 0 && pColor->G == 0xFF && pColor->B == 0)
		{
			count++;
		}
	}
	free(pBuf);
	if(count >= 5)
	{
		return false;
	}
	return true;
}

WEAPON getWeapon()
{
	UCHAR *pBuf;
	INT lineWidth;
	COLOR_BGR* pColor;
	UINT value;
	value = 0;
	pBuf = (UCHAR*)CaptureScreen(rectWeaponName, &lineWidth);
	for(int x = 0; x < rectWeaponName.right - rectWeaponName.left + 1; x++)
	{
		for(int y = 0; y < rectWeaponName.bottom - rectWeaponName.top + 1; y++)
		{
			pColor = (COLOR_BGR*)&pBuf[y * lineWidth + x * 3];
			if(pColor->B == 0xC1 && pColor->G == 0xC1 && pColor->R == 0x99)
			{
				value += x ^ y;
			}
		}
	}
	free(pBuf);
	return (WEAPON)value;
}

//void drawFrontSight()
//{
//	HDC hdc;
//	HWND wnd;
////	HBRUSH brush;
//#if SCREEN_WIDTH == 1024
//	wnd = FindWindow(NULL, L"crossfire");
//	hdc = GetWindowDC(wnd);
////	brush
//	MoveToEx(hdc, 499, 384, NULL);
//	LineTo(hdc, 525, 384);
//	MoveToEx(hdc, 512, 371, NULL);
//	LineTo(hdc, 512, 397);
//	ReleaseDC(wnd, hdc);
//#endif	
//}
//BOOL isSelfMove()
//{
//	//红名出现的方框
//	RECT rect = {250, 200, 550, 200};
//	UCHAR * pBuf;
//	COLOR_BGR* pColor;
//	pBuf = (UCHAR*)CaptureScreen(rect, NULL);
//	for(int x = 0; x < rect.right - rect.left + 1; x++)
//	{
//	}
//	free(pBuf);
//	if()
//	{
//		return true;
//	}
//	return false;
//}