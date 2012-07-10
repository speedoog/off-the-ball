/********************************************************************
	created:	2002/10/15
	created:	15:10:2002   15:37
	filename: 	pcpad.h
	
	purpose:	PC joypad
*********************************************************************/

#ifndef _PCPAD_INCLUDED_
#define _PCPAD_INCLUDED_

#ifndef	WIN32
#error Incorrectly included file. This file should only be included in Win32 projects.
#endif

#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <basetsd.h>

#include "DirectX9Sdk/Include/dinput.h"            //DirectInput header (NEW)

class PcPadManager
{
public:
    typedef int PadIdx;
    enum CtrlIdx
    {
			PAD_BTN_CROSS=0,
			PAD_BTN_CIRCLE,
			PAD_BTN_SQUARE,
			PAD_BTN_TRIANGLE,
			PAD_BTN_START,
			PAD_BTN_SELECT,
			PAD_BTN_R1,
			PAD_BTN_R2,
			PAD_BTN_L1,
			PAD_BTN_L2,
			PAD_BTN_UP,
			PAD_BTN_DOWN,
			PAD_BTN_LEFT,
			PAD_BTN_RIGHT,
			PAD_BTN_OVER_AXIS1,
			PAD_BTN_OVER_AXIS2,
			PAD_AXIS_X,
			PAD_AXIS_Y,
			PAD_AXIS_Z,
			PAD_AXIS_ZROTATION,
			PAD_MAX_ENTRIES,
			PAD_FORCEDWORD=0x7FFFFFFF
    };

	typedef unsigned char		CtrlStatus;

				PcPadManager();
				~PcPadManager();
	int			Init(HWND hWnd);
	void		Kill();
	void		Update();
	CtrlStatus  GetCtrlState(CtrlIdx j) const; 

private:
	static	BOOL CALLBACK	EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext);
	static	BOOL CALLBACK	EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext);

private:
	bool					_bInited;					// Has been inited ?
	LPDIRECTINPUT8			_pDI;						// DInput Device interface
	LPDIRECTINPUTDEVICE8	_pJoystick;					// DInput Joystick
	HWND					_hWnd;						// Main window hWnd
	DIJOYSTATE2				_js;						// DInput joystick state
};

#endif	//_PCPAD_INCLUDED_
