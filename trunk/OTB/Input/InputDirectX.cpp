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

#include "InputDirectX.h"
#include "../Base/Base.h"


static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
{
	InputDirectX* pThis =(InputDirectX*)pContext;
	return pThis->EnumJoysticks(pdidInstance);
}

static BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext)
{
	InputDirectX* pThis =(InputDirectX*)pContext;
	return pThis->EnumObjects(pdidoi);
}

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
InputDirectX::InputDirectX()
{
	_hWnd			=NULL;
	_pDirectInput	=NULL;
	_bInited		=false;
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
InputDirectX::~InputDirectX()
{
	TAssert(!_bInited);
}

// ****************************************************************************************
//	Init
// ****************************************************************************************
int	InputDirectX::Init(HWND	hWnd)
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

	for(DeviceIdx iPadIdx=0; iPadIdx<_nPadCount; ++iPadIdx)
	{
		Device& PadCurrent =_Device[iPadIdx];

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

// ****************************************************************************************
//	Kill
// ****************************************************************************************
void InputDirectX::Kill()
{
	TAssert(_bInited);

	for(DeviceIdx iPadIdx=0; iPadIdx<_nPadCount; ++iPadIdx)
	{
		Device& PadCurrent =_Device[iPadIdx];
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

// ****************************************************************************************
//	Update
// ****************************************************************************************
void InputDirectX::Update(void)
{
	TAssert(_bInited);

	for(DeviceIdx iPadIdx=0; iPadIdx<_nPadCount; ++iPadIdx)
	{
		Device& PadCurrent =_Device[iPadIdx];
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
	}
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

bool IsPs4Pad(const DIDEVICEINSTANCE* pCurrentDevice)
{
	const char* pName =pCurrentDevice->tszInstanceName;
	const char* p=strstr(pName, "Wireless Controller");
	return (p!=NULL);
}

// ****************************************************************************************
//	Name: EnumJoysticks()
//	Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
// ****************************************************************************************
Bool InputDirectX::EnumJoysticks(const DIDEVICEINSTANCE* pdidInstance)
{
	Device& PadCurrent =_Device[_nPadCurrent];

	if (IsPs3Pad(pdidInstance))
	{
		PadCurrent._PadType =PT_PS3;
	}
	else if (IsPs4Pad(pdidInstance))
	{
		PadCurrent._PadType =PT_PS4;
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
	HRESULT hr = _pDirectInput->CreateDevice(pdidInstance->guidInstance,&(PadCurrent._pDInputDevice),NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;										// If it failed, then we can't use this joystick. (Maybe the user unplugged it while we were in the middle of enumerating it.)
	}

	++_nPadCurrent;
	if (_nPadCount>=MAX_DEVICE_COUNT)
	{
		return DIENUM_STOP;											// Stop enumeration. Note: we're just taking the first joystick we get. You could store all the enumerated joysticks and let the user pick.
	}

	return DIENUM_CONTINUE;
}

// ****************************************************************************************
// Name: EnumObjects()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
// ****************************************************************************************
Bool  InputDirectX::EnumObjects(const DIDEVICEOBJECTINSTANCE* pdidoi)
{
	Device& PadCurrent =_Device[_nPadCurrent];

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
