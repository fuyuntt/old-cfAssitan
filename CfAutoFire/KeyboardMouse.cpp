/****************************
*¼üÊó²Ù×÷¿âº¯Êý
*
****************************/
#include "stdafx.h"

#include "KeyboardMouse.h"
#include "stdio.h"

extern "C" {
#include "setupapi.h" //Must link in setupapi.lib
#include "hidsdi.h" //Must link in hid.lib
}

//¼üÊó²Ù×÷»º³åÇø
unsigned char KeyboardBuffer[10] = {0, 1};
unsigned char MouseBuffer[10] = {0, 2};

static HANDLE Hdevice;

PSP_DEVICE_INTERFACE_DETAIL_DATA HID_FindDevices(USHORT vid, USHORT pid, USHORT* FIFO_length);

BOOL InitDevice()
{
	USHORT length;
	PSP_DEVICE_INTERFACE_DETAIL_DATA myDev;
	myDev = HID_FindDevices(VID, PID, &length);
	if(myDev == NULL)
	{
		printf("please plugin device. \n");
		return false;
	}
	printf("successful find device. \n");

	Hdevice = CreateFile(myDev->DevicePath, GENERIC_READ | GENERIC_WRITE, 
						   0,(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 0, NULL);
	if (Hdevice == INVALID_HANDLE_VALUE)
    {
        free(myDev);
        myDev = NULL;
		printf("can't creat files \n");
		return false;
    }
	printf("has creat file. \n");
	return true;
}

BOOL KeyDown(UCHAR vKey)
{
	int i;
	BOOL ok;
	DWORD len;
	if(Hdevice == NULL) return false;
	if(vKey >= VK_LSHIFT && vKey <= VK_RCONTROL)
	{
		switch (vKey)
		{
		case VK_LCONTROL:
			KeyboardBuffer[2] |= 0x01;
			break;
		case VK_LSHIFT:
			KeyboardBuffer[2] |= 0x02;
			break;
		case VK_RCONTROL:
			KeyboardBuffer[2] |= 0x10;
			break;
		case VK_RSHIFT:
			KeyboardBuffer[2] |= 0x20;
			break;
		}
	}
	else
	{
		if(vKey >= 'A' && vKey <= 'Z')
		{
			vKey = vKey - 'A' + 0x04;
		}
		else if(vKey >= '0' && vKey <= '9')
		{
			vKey = vKey - '0' + 0x1E;
		}
		else
		{
			switch (vKey)
			{
			case VK_SPACE:
				vKey = 0x2C;
				break;
			default:
				return false;
			}
		}
		for(i = 4; KeyboardBuffer[i] != 0 && i < 10; i++)
		{}
		if(i == 10) return false;
		KeyboardBuffer[i] = vKey;
	}
	ok = WriteFile(Hdevice, KeyboardBuffer, 10, &len, NULL);
	if(!ok) 
	{
		return false;
	}
	return true;
}
BOOL KeyUp(UCHAR vKey)
{
	int i;
	BOOL ok;
	DWORD len;
	if(Hdevice == NULL) return false;
	if(vKey >= VK_LSHIFT && vKey <= VK_RCONTROL)
	{
		switch (vKey)
		{
		case VK_LCONTROL:
			KeyboardBuffer[2] &= ~0x01;
			break;
		case VK_LSHIFT:
			KeyboardBuffer[2] &= ~0x02;
			break;
		case VK_RCONTROL:
			KeyboardBuffer[2] &= ~0x10;
			break;
		case VK_RSHIFT:
			KeyboardBuffer[2] &= ~0x20;
			break;
		}
	}
	else
	{
		if(vKey >= 'A' && vKey <= 'Z')
		{
			vKey = vKey - 'A' + 0x04;
		}
		else if(vKey >= '0' && vKey <= '9')
		{
			vKey = vKey - '0' + 0x1E;
		}
		else
		{
			switch (vKey)
			{
			case VK_SPACE:
				vKey = 0x2C;
				break;
			default:
				return false;
			}
		}
		for(i = 4; KeyboardBuffer[i] != vKey && i < 10; i++)
		{}
		if(i == 10) return false;
		KeyboardBuffer[i] = 0;
	}
	ok = WriteFile(Hdevice, KeyboardBuffer, 10, &len, NULL);
	if(!ok) 
	{
		return false;
	}
	return true;
}
BOOL KeyPress(UCHAR vKey)
{
	if(!KeyDown(vKey)) return false;
	Sleep(40);
	if(!KeyUp(vKey)) return false;
	return true;
}
BOOL MouseDown(BUTTON btn)
{
	int i;
	BOOL ok;
	DWORD len;
	if(Hdevice == NULL) return false;
	if(btn == LEFT)
	{
		MouseBuffer[2] |= 0x01;
	}
	else if(btn == RIGHT)
	{
		MouseBuffer[2] |= 0x02;
	}
	for(i = 3; i < 6; i++)
		MouseBuffer[i] = 0;
	ok = WriteFile(Hdevice, MouseBuffer, 10, &len, NULL);
	if(!ok) 
	{
		return false;
	}
	return true;
}
BOOL MouseUp(BUTTON btn)
{
	int i;
	BOOL ok;
	DWORD len;
	if(Hdevice == NULL) return false;
	if(btn == LEFT)
	{
		MouseBuffer[2] &= 0xFE;
	}
	else if(btn == RIGHT)
	{
		MouseBuffer[2] &= 0xFD;
	}
	for(i = 3; i < 6; i++)
		MouseBuffer[i] = 0;
	ok = WriteFile(Hdevice, MouseBuffer, 10, &len, NULL);
	if(!ok) 
	{
		return false;
	}
	return true;
}
BOOL MouseClick(BUTTON btn)
{
	if(!MouseDown(btn)) return false;
	Sleep(40);
	if(!MouseUp(btn)) return false;
	return true;
}
BOOL MouseMove(int x, int y)
{
	BOOL ok;
	DWORD len;
	if(Hdevice == NULL) return false;
	MouseBuffer[3] = x;
	MouseBuffer[4] = y;
	MouseBuffer[5] = 0;
	ok = WriteFile(Hdevice, MouseBuffer, 10, &len, NULL);
	if(!ok) 
	{
		return false;
	}
	return true;
}
BOOL MouseWheel(int vol)
{
	BOOL ok;
	DWORD len;
	if(Hdevice == NULL) return false;
	MouseBuffer[3] = 0;
	MouseBuffer[4] = 0;
	MouseBuffer[5] = vol;
	ok = WriteFile(Hdevice, MouseBuffer, 10, &len, NULL);
	if(!ok) 
	{
		return false;
	}
	return true;
}
//BOOL MouseMoveEx(int x, int y)
//{
//	int abs_x,abs_y;
//	int max_xy;
//	abs_x = (x > 0) ? x : -1 * x;
//	abs_y = (y > 0) ? y : -1 * y;
//	max_xy = (abs_x > abs_y) ? abs_x : abs_y;
//	for(int i = 0; i < max_xy; i++)
//	{
//		MouseMove(
//}

PSP_DEVICE_INTERFACE_DETAIL_DATA HID_FindDevices(USHORT vid, USHORT pid, USHORT* FIFO_length)
{
	GUID HidGuid;
	HDEVINFO  DevInfo;
	HIDD_ATTRIBUTES DevAttributes;
	SP_DEVICE_INTERFACE_DATA DevData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA DevDetail;
	PHIDP_PREPARSED_DATA PreparsedData;
	HIDP_CAPS Capabilities;
	ULONG Length;
	int Index;
	BOOL ok;
	HANDLE DevHandle;
	//Get GUID for all system hids
	HidD_GetHidGuid(&HidGuid);
	//Get Device Information for all present devices 
	DevInfo = SetupDiGetClassDevs(&HidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	DevData.cbSize = sizeof(DevData);
	DevDetail = NULL;
	Index = -1;
	*FIFO_length = 0;

	/* Scan all Devices */
	while(1)
	{
        Index++;
        /* Device Interface Element of a Device Information set */
        ok = SetupDiEnumDeviceInterfaces(DevInfo, 0,  &HidGuid,  Index,  &DevData);
        if (!ok) break;
        /* Get Device Interface Details - Get Length */
        ok = SetupDiGetDeviceInterfaceDetail(DevInfo,  &DevData,  NULL,  0,  &Length,  NULL);
        /* Allocate memory for Device Detailed Data */
        DevDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(Length);
		if(DevDetail == NULL) break;
        /* Set cbSize in the DevDetail structure */
        DevDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        /* Get Device Interface Details */
        ok = SetupDiGetDeviceInterfaceDetail(DevInfo, &DevData,  DevDetail,  Length,  NULL,  NULL);
        if (!ok) 
        {
            free(DevDetail);
            DevDetail = NULL;
            continue;
        }
		/* Create File for Device Read/Write */
        DevHandle = CreateFile(DevDetail->DevicePath, GENERIC_READ | GENERIC_WRITE, 
                                FILE_SHARE_READ | FILE_SHARE_WRITE, 
								(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,  0, NULL);
        if (DevHandle == INVALID_HANDLE_VALUE)
        {
            free(DevDetail);
            DevDetail = NULL;
            continue;
        }
        DevAttributes.Size = sizeof(DevAttributes);
        ok = HidD_GetAttributes(DevHandle,&DevAttributes);
        if(!ok) 
        {
            free(DevDetail);
            CloseHandle(DevHandle);
            DevDetail = NULL;
            continue;
        }
        if(DevAttributes.VendorID!=vid||DevAttributes.ProductID != pid)
        {
            free(DevDetail);
            CloseHandle(DevHandle);
            DevDetail = NULL;
            continue;
        }
        /* Get Preparsed Data */
        ok = HidD_GetPreparsedData(DevHandle, &PreparsedData);
        if (!ok) 
        {
            free(DevDetail);
            CloseHandle(DevHandle);
            DevDetail = NULL;
            continue;
        }
        /* Get Device's Capabilities */
        HidP_GetCaps(PreparsedData, &Capabilities);
        *FIFO_length = Capabilities.InputReportByteLength;
          
        CloseHandle(DevHandle);
        break;
    }
     
    SetupDiDestroyDeviceInfoList (DevInfo);
    return DevDetail;
}
