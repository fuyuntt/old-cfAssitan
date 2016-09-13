#ifndef SCREEN_H__
#define SCREEN_H__

#include "windows.h"

#include "weapon.h"

PBYTE CaptureScreen(RECT rect, OUT PINT bufferLineWidth);
BOOL isInRange();
BOOL isZoom();
BOOL isBingo();
WEAPON getWeapon();
void drawFrontSight();
BOOL isSniper();

typedef struct COLOR_BGR
{
	UCHAR B;
	UCHAR G;
	UCHAR R;
};

#endif