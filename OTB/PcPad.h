//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//     ________   _____  _____    __  .__             __________        .__  .__        //
//     \_____  \_/ ____\/ ____\ _/  |_|  |__   ____   \______   \_____  |  | |  |       //
//      /   |   \   __\\   __\  \   __\  |  \_/ __ \   |    |  _/\__  \ |  | |  |       //
//     /    |    \  |   |  |     |  | |   Y  \  ___/   |    |   \ / __ \|  |_|  |__     //
//     \_______  /__|   |__|     |__| |___|  /\___  >  |______  /(____  /____/____/     //
//             \/                          \/     \/          \/      \/                //
//                                                                                      //
//                          .o                                                          //
//                   ¨>   .                                      <¨                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PCPAD_H__
#define __PCPAD_H__

#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <basetsd.h>

#include "DirectX9Sdk/Include/dinput.h"            //DirectInput header (NEW)
#include "Base.h"
#include "SmartEnum.h"
#include "../hge/hgevector.h"


class PcPadManager
{
public:
    typedef int PadIdx;

	enum
	{
		MAX_PAD_COUNT =4
	};

	SMARTENUM_DECLARE(PadType,
		PT_INVALID,
		PT_XBOX,
		PT_PS3,
		PT_OTHER,
		PT_MAX,
		);

	class PcPad
	{
	public:
		PcPad()
		{
			_pDInputDevice =NULL;
			_PadType =PT_INVALID;
			memset(&_JoyState,0,sizeof(DIJOYSTATE2));
		}

		LPDIRECTINPUTDEVICE8	_pDInputDevice;				// DInput Joystick
		DIJOYSTATE2				_JoyState;					// DInput joystick state
		hgeVector				_vAxisLeft, _vAxisRight;
		PadType					_PadType;
	};

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

// 		PAD_LEFTPAD_AXIS_X,
// 		PAD_LEFTPAD_AXIS_Y,
// 		PAD_RIGHTPAD_AXIS_X,
// 		PAD_RIGHTPAD_AXIS_Y,

		PAD_BTN_UP,
		PAD_BTN_DOWN,
		PAD_BTN_LEFT,
		PAD_BTN_RIGHT,
		PAD_BTN_OVER_AXIS1,
		PAD_BTN_OVER_AXIS2,

		PAD_AXIS_X,
		PAD_AXIS_Y,
		PAD_AXIS_Z,
		PAD_AXIS_RX,
		PAD_AXIS_RY,
		PAD_AXIS_RZ,

		PAD_SLIDER0,
		PAD_SLIDER1,

		PAD_MAX_ENTRIES,
    );

	typedef unsigned char CtrlStatus;

						PcPadManager();
						~PcPadManager();
	int					Init(HWND hWnd);
	void				Kill();
	void				Update();
	CtrlStatus  		GetCtrlState(PadIdx iPadIdx, CtrlIdx iControl) const;
	const hgeVector&	GetAxisLeft(PadIdx iPadIdx) const		{ return _Pad[iPadIdx]._vAxisLeft;  }
	const hgeVector&	GetAxisRight(PadIdx iPadIdx) const		{ return _Pad[iPadIdx]._vAxisRight; }

private:
	static	BOOL CALLBACK	EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext);
	static	BOOL CALLBACK	EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext);

private:
	bool			_bInited;					// Has been inited ?
	LPDIRECTINPUT8	_pDirectInput;				// DInput Device interface
	HWND			_hWnd;						// Main window hWnd
	PcPad			_Pad[MAX_PAD_COUNT];
	PadIdx			_nPadCount;					// Managed Pads
	PadIdx			_nPadCurrent;				// used in enumeration
};

#endif	//__PCPAD_H__
