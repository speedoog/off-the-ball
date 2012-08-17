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

#include "InputPcPadManager.h"
#include "Base/Base.h"

// ********************************************
//	Ctor
// ********************************************
PcPadManager::PcPadManager()
{
	_hWnd			=NULL;
	_pDirectInput	=NULL;
	_bInited		=false;
}

// ********************************************
//	Dtor
// ********************************************
PcPadManager::~PcPadManager()
{
	TAssert(!_bInited);
}

// ********************************************
//	Init
// ********************************************
int	PcPadManager::Init(HWND	hWnd)
{
	TAssert(!_bInited);

	HRESULT hr;

	_hWnd =hWnd;
	// Register with the DirectInput subsystem and get a pointer to a IDirectInput interface we can use.
	hr=DirectInput8Create(	GetModuleHandle(NULL),
							DIRECTINPUT_VERSION,
							IID_IDirectInput8,
							(void**)&_pDirectInput,
							NULL);
	TAssert(_pDirectInput);
	TAssert(!hr);
	if (!_pDirectInput)		return	-1;
	if (hr)					return	-2;
	
	_nPadCurrent =0;

	// Look for a simple joystick we can use for this sample program.
	hr =_pDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,(void*)this,DIEDFL_ATTACHEDONLY);
	TAssert(!hr);
	if (hr)					return	-3;

	_nPadCount =_nPadCurrent;
	_nPadCurrent =0;

	for(PadIdx iPadIdx=0; iPadIdx<_nPadCount; ++iPadIdx)
	{
		PcPad& PadCurrent =_Pad[iPadIdx];

		if (PadCurrent._PadType!=PT_INVALID)
		{
			// Make sure we got a joystick
			TAssert(PadCurrent._pDInputDevice);
			if (!PadCurrent._pDInputDevice)	return	-4;

			// Set the data format to "simple joystick" - a predefined data format 
			hr =PadCurrent._pDInputDevice->SetDataFormat(&c_dfDIJoystick2);
			TAssert(!hr);

			// Set the cooperative level to let DInput know how this device should interact with the system and with other DInput applications.
			hr =PadCurrent._pDInputDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
			TAssert(!hr);

			_nPadCurrent =iPadIdx;
			hr =PadCurrent._pDInputDevice->EnumObjects(EnumObjectsCallback,(void*)this, DIDFT_ALL);						// Enumerate the joystick objects. The callback function enabled user interface elements for objects that are found, and sets the min/max values property for discovered axes.
			TAssert(!hr);
		}
	}

	_bInited =true;

	return	0;
}

// ********************************************
//	Kill
// ********************************************
void PcPadManager::Kill()
{
	TAssert(_bInited);

	for(PadIdx iPadIdx=0; iPadIdx<_nPadCount; ++iPadIdx)
	{
		PcPad& PadCurrent =_Pad[iPadIdx];
		// Unacquire the device one last time just in case  the app tried to exit while the device is still acquired.
		if (PadCurrent._pDInputDevice) 
		{
			PadCurrent._pDInputDevice->Unacquire();
		}

		// Release any DirectInput objects.
		TRelease(PadCurrent._pDInputDevice);
	}

	TRelease(_pDirectInput);

	_bInited =false;
}

// ********************************************
//	Update
// ********************************************
void PcPadManager::Update(void)
{
	TAssert(_bInited);

	for(PadIdx iPadIdx=0; iPadIdx<_nPadCount; ++iPadIdx)
	{
		PcPad& PadCurrent =_Pad[iPadIdx];
		HRESULT hr;
		TAssert(PadCurrent._pDInputDevice);
		if (!PadCurrent._pDInputDevice)
			continue;

		// Poll the device to read the current state
		hr = PadCurrent._pDInputDevice->Poll();
		if (FAILED(hr))
		{
			hr = PadCurrent._pDInputDevice->Acquire();													// DInput is telling us that the input stream has been interrupted.
			while( hr == DIERR_INPUTLOST )																// We aren't tracking any state between polls, 
			{																							// so we don't have any special reset that needs to be done.
				hr = PadCurrent._pDInputDevice->Acquire();												// We just re-acquire and try again.
			}

			hr = PadCurrent._pDInputDevice->Poll(); 
			if (!FAILED(hr))
			{
				PadCurrent._pDInputDevice->GetDeviceState(sizeof(DIJOYSTATE2),&PadCurrent._JoyState);				// Get the input's device state
			}

			continue;
		}

		// Get the input's device state
		PadCurrent._pDInputDevice->GetDeviceState(sizeof(DIJOYSTATE2),&PadCurrent._JoyState);

		PadCurrent._vAxisLeft.x =float(PadCurrent._JoyState.lX&0xFF)/128.0f-1.0f;
		PadCurrent._vAxisLeft.y =float(255-(PadCurrent._JoyState.lY&0xFF))/128.0f-1.0f;

		if (PadCurrent._PadType==PT_XBOX)
		{
			PadCurrent._vAxisRight.x =float(PadCurrent._JoyState.lRx&0xFF)/128.0f-1.0f;
			PadCurrent._vAxisRight.y =float(255-(PadCurrent._JoyState.lRy&0xFF))/128.0f-1.0f;
		}
		else if (PadCurrent._PadType==PT_PS3)
		{
			PadCurrent._vAxisRight.x =float(PadCurrent._JoyState.lZ&0xFF)/128.0f-1.0f;
			PadCurrent._vAxisRight.y =float(255-(PadCurrent._JoyState.lRz&0xFF))/128.0f-1.0f;
		}
		else
		{
			// other
			PadCurrent._vAxisRight.x =float(PadCurrent._JoyState.lRz&0xFF)/128.0f-1.0f;
			PadCurrent._vAxisRight.y =float(255-(PadCurrent._JoyState.rglSlider[0]&0xFF))/128.0f-1.0f;
		}
	}
}

// ********************************************
//	GetCtrlState
// ********************************************
PcPadManager::CtrlStatus PcPadManager::GetCtrlState(PadIdx iPadIdx, CtrlIdx iControl) const
{
	const PcPad& PadCurrent =_Pad[iPadIdx];

	switch(iControl)
	{
	case	PAD_BTN_A:			return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[0];
	case	PAD_BTN_B:			return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[1];
	case	PAD_BTN_X:			return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[2];
	case	PAD_BTN_Y:			return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[3];

	case	PAD_BTN_LEFT_BTN:	return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[4];
	case	PAD_BTN_RIGHT_BTN:	return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[5];

	case	PAD_BTN_SELECT:		return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[6];
	case	PAD_BTN_START:		return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[7];
	case	PAD_BTN_THUMB_L:	return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[8];
	case	PAD_BTN_THUMB_R:	return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[9];

// 	case	PAD_LEFTPAD_AXIS_X:	return	(CtrlStatus)(PadCurrent._JoyState.lX&0xFF);
// 	case	PAD_LEFTPAD_AXIS_Y:	return	(CtrlStatus)(0xFF-(PadCurrent._JoyState.lY&0xFF));
// 
// 	case	PAD_RIGHTPAD_AXIS_X:return	(CtrlStatus)(PadCurrent._JoyState.lRx&0xFF);
// 	case	PAD_RIGHTPAD_AXIS_Y:return	(CtrlStatus)(0xFF-(PadCurrent._JoyState.lRy&0xFF));

	case	PAD_BTN_UP:			return	(CtrlStatus)(PadCurrent._JoyState.rgdwPOV[0]&0xFF);
	case	PAD_BTN_DOWN:		return	(CtrlStatus)(PadCurrent._JoyState.rgdwPOV[1]&0xFF);
	case	PAD_BTN_LEFT:		return	(CtrlStatus)(PadCurrent._JoyState.rgdwPOV[2]&0xFF);
	case	PAD_BTN_RIGHT:		return	(CtrlStatus)(PadCurrent._JoyState.rgdwPOV[3]&0xFF);
	case	PAD_BTN_OVER_AXIS1:	return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[10];
	case	PAD_BTN_OVER_AXIS2:	return	(CtrlStatus)PadCurrent._JoyState.rgbButtons[11];

	// raw axis
	case	PAD_AXIS_X:			return	(CtrlStatus)(PadCurrent._JoyState.lX&0xFF);
	case	PAD_AXIS_Y:			return	(CtrlStatus)(PadCurrent._JoyState.lY&0xFF);
	case	PAD_AXIS_Z:			return	(CtrlStatus)(PadCurrent._JoyState.lZ&0xFF);
	case	PAD_AXIS_RX:		return	(CtrlStatus)(PadCurrent._JoyState.lRx&0xFF);
	case	PAD_AXIS_RY:		return	(CtrlStatus)(PadCurrent._JoyState.lRy&0xFF);
	case	PAD_AXIS_RZ:		return	(CtrlStatus)(PadCurrent._JoyState.lRz&0xFF);

	// sliders
	case	PAD_SLIDER0:		return	(CtrlStatus)(PadCurrent._JoyState.rglSlider[0]&0xFF);
	case	PAD_SLIDER1:		return	(CtrlStatus)(PadCurrent._JoyState.rglSlider[1]&0xFF);

	default:;
	}
	return	0;
}

bool IsXboxPad(const DIDEVICEINSTANCE* pCurrentDevice)
{
	const char* pName =pCurrentDevice->tszInstanceName;
	const char* p=strstr(pName, "XBOX");
	return (p!=NULL);
}

bool IsPs3Pad(const DIDEVICEINSTANCE* pCurrentDevice)
{
	const char* pName =pCurrentDevice->tszInstanceName;
	const char* p=strstr(pName, "PLAYSTATION");
	return (p!=NULL);
}

// ********************************************
//	Name: EnumJoysticksCallback()
//	Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
// ********************************************
BOOL CALLBACK PcPadManager::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
{
	PcPadManager* pthis =(PcPadManager*)pContext;
	PcPad& PadCurrent =pthis->_Pad[pthis->_nPadCurrent];

	if (IsPs3Pad(pdidInstance))
	{
		PadCurrent._PadType =PT_PS3;
	}
	else if (IsXboxPad(pdidInstance))
	{
		PadCurrent._PadType =PT_XBOX;
	}
	else
	{
		PadCurrent._PadType =PT_OTHER;
	}

	// Obtain an interface to the enumerated joystick.
	HRESULT hr = pthis->_pDirectInput->CreateDevice(pdidInstance->guidInstance,&(PadCurrent._pDInputDevice),NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;										// If it failed, then we can't use this joystick. (Maybe the user unplugged it while we were in the middle of enumerating it.)
	}

	++pthis->_nPadCurrent;
	if (pthis->_nPadCount>=MAX_PAD_COUNT)
	{
		return DIENUM_STOP;											// Stop enumeration. Note: we're just taking the first joystick we get. You could store all the enumerated joysticks and let the user pick.
	}

	return DIENUM_CONTINUE;
}

// ********************************************
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
// ********************************************
BOOL CALLBACK PcPadManager::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext)
{
	PcPadManager* pthis=(PcPadManager*)pContext;
	PcPad& PadCurrent =pthis->_Pad[pthis->_nPadCurrent];

	static int nSliderCount =0;		// Number of returned slider controls
	static int nPOVCount	=0;		// Number of returned POV controls

	// For axes that are returned, set the DIPROP_RANGE property for the enumerated axis in order to scale min/max values.
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg; 
		diprg.diph.dwSize       =sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize =sizeof(DIPROPHEADER); 
		diprg.diph.dwHow        =DIPH_BYID; 
		diprg.diph.dwObj        =pdidoi->dwType;										// Specify the enumerated axis
		diprg.lMin              =0; 
		diprg.lMax              =255;

		PadCurrent._pDInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);				// Set the range for the axis
	}

	return DIENUM_CONTINUE;
}
