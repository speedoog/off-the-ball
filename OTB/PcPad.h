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
#include "base.h"

class PcPadManager
{
public:
    typedef int PadIdx;

    SMARTENUM_DECLARE(CtrlIdx,
		PAD_BTN_A,
		PAD_BTN_B,
		PAD_BTN_X,
		PAD_BTN_Y,

		PAD_BTN_LEFT_BTN,
		PAD_BTN_RIGHT_BTN,

		PAD_BTN_SELECT,
		PAD_BTN_START,
		PAD_BTN_THUMB_L,
		PAD_BTN_THUMB_R,

		PAD_LEFTPAD_AXIS_X,
		PAD_LEFTPAD_AXIS_Y,

		PAD_RIGHTPAD_AXIS_X,
		PAD_RIGHTPAD_AXIS_Y,

		PAD_BTN_UP,
		PAD_BTN_DOWN,
		PAD_BTN_LEFT,
		PAD_BTN_RIGHT,
		PAD_BTN_OVER_AXIS1,
		PAD_BTN_OVER_AXIS2,
		PAD_AXIS_Z,

		PAD_MAX_ENTRIES,
    );

	typedef unsigned char CtrlStatus;

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
	LPDIRECTINPUT8			_pDirectInput;				// DInput Device interface
	LPDIRECTINPUTDEVICE8	_pDInputDevice;				// DInput Joystick
	HWND					_hWnd;						// Main window hWnd
	DIJOYSTATE2				_JoyState;					// DInput joystick state
};

#endif	//_PCPAD_INCLUDED_
