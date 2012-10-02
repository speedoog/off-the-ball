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

#ifndef __INPUTDIRECTX_H__
#define __INPUTDIRECTX_H__

#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <basetsd.h>

#include "DirectX10Sdk/Include/dinput.h"            //DirectInput header (NEW)
#include "Base/Base.h"
#include "Base/SmartEnum.h"
#include "../hge/hgevector.h"

typedef unsigned char	CtrlStatus;
typedef int				DeviceIdx;

class InputDirectX
{
public:

	enum
	{
		MAX_DEVICE_COUNT =4
	};

	SMARTENUM_DECLARE(PadType,
		PT_INVALID,
		PT_XBOX,
		PT_PS3,
		PT_OTHER,
		PT_MAX,
		);

	class Device
	{
	public:
		Device()
		{
			_pDInputDevice =NULL;
			_PadType =PT_INVALID;
			memset(&_JoyState,0,sizeof(DIJOYSTATE2));
		}

		LPDIRECTINPUTDEVICE8	_pDInputDevice;				// DInput Joystick
		DIJOYSTATE2				_JoyState;					// DInput joystick state
		PadType					_PadType;
	};

						InputDirectX();
						~InputDirectX();
	int					Init(HWND hWnd);
	void				Kill();
	void				Update();
	const Device&		GetDevice(DeviceIdx iDeviceIdx) const			{ return _Device[iDeviceIdx]; }

private:
	static	BOOL CALLBACK	EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext);
	static	BOOL CALLBACK	EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext);

private:
	bool			_bInited;					// Has been inited ?
	LPDIRECTINPUT8	_pDirectInput;				// DInput Device interface
	HWND			_hWnd;						// Main window hWnd
	Device			_Device[MAX_DEVICE_COUNT];
	DeviceIdx		_nPadCount;					// Managed Pads
	DeviceIdx		_nPadCurrent;				// used in enumeration
};

#endif	//__INPUTDIRECTX_H__
