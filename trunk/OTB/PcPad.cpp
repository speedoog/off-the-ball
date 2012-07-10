/********************************************************************
	created:	2002/10/15
	created:	15:10:2002   15:45
	filename: 	pcpad.cpp
	
	purpose:	pc joypad
*********************************************************************/

#ifndef	WIN32
#error Incorrectly included file. This file should only be included in Win32 projects.
#endif

#include "PcPad.h"
#include "base.h"


// ********************************************
//	Ctor
// ********************************************
PcPadManager::PcPadManager()
{
	_hWnd		=NULL;
	_pDI		=NULL;
	_pJoystick	=NULL;
	_bInited	=false;
	memset(&_js,0,sizeof(DIJOYSTATE2));
}

// ********************************************
//	Dtor
// ********************************************
PcPadManager::~PcPadManager()
{
	RfxAssert(!_bInited);
}

// ********************************************
//	Init
// ********************************************
int	PcPadManager::Init(HWND	hWnd)
{
	RfxAssert(!_bInited);

	HRESULT hr;

	_hWnd =hWnd;
	// Register with the DirectInput subsystem and get a pointer to a IDirectInput interface we can use.
	hr=DirectInput8Create(	GetModuleHandle(NULL),
							DIRECTINPUT_VERSION,
							IID_IDirectInput8,
							(void**)&_pDI,
							NULL);
	RfxAssert(_pDI);
	RfxAssert(!hr);
	if (!_pDI)			return	-1;
	if (hr)				return	-2;
	
	// Look for a simple joystick we can use for this sample program.
	hr =_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,(void*)this,DIEDFL_ATTACHEDONLY);
	RfxAssert(!hr);
	if (hr)				return	-3;

	// Make sure we got a joystick
	RfxAssert(_pJoystick);
	if (!_pJoystick)	return	-4;

	// Set the data format to "simple joystick" - a predefined data format 
	hr =_pJoystick->SetDataFormat(&c_dfDIJoystick2);
	RfxAssert(!hr);

	// Set the cooperative level to let DInput know how this device should interact with the system and with other DInput applications.
	hr =_pJoystick->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
	RfxAssert(!hr);

	// Enumerate the joystick objects. The callback function enabled user interface elements for objects that are found, and sets the min/max values property for discovered axes.
	hr =_pJoystick->EnumObjects(EnumObjectsCallback,(void*)this, DIDFT_ALL);
	RfxAssert(!hr);

	_bInited =true;

	return	0;
}

// ********************************************
//	Kill
// ********************************************
void PcPadManager::Kill()
{
	RfxAssert(_bInited);

	// Unacquire the device one last time just in case  the app tried to exit while the device is still acquired.
	if (_pJoystick) 
	{
		_pJoystick->Unacquire();
	}

	// Release any DirectInput objects.
	RfxRelease(_pJoystick);
	RfxRelease(_pDI);

	_bInited =false;
}

// ********************************************
//	Update
// ********************************************
void PcPadManager::Update(void)
{
	RfxAssert(_bInited);

	HRESULT hr;
	RfxAssert(_pJoystick);
	if (!_pJoystick)		return;

	// Poll the device to read the current state
	hr = _pJoystick->Poll();
	if (FAILED(hr))
	{
		// DInput is telling us that the input stream has been interrupted. We aren't tracking any state between polls, so
		// we don't have any special reset that needs to be done. We just re-acquire and try again.
		hr = _pJoystick->Acquire();
		while( hr == DIERR_INPUTLOST ) 
		{
			hr = _pJoystick->Acquire();
		}

		hr = _pJoystick->Poll(); 
		if(!FAILED(hr))
		{
			// Get the input's device state
			_pJoystick->GetDeviceState(sizeof(DIJOYSTATE2),&_js);
		}

		return;
	}

	// Get the input's device state
	_pJoystick->GetDeviceState(sizeof(DIJOYSTATE2),&_js);

}

// ********************************************
//	GetCtrlState
// ********************************************
PcPadManager::CtrlStatus PcPadManager::GetCtrlState(CtrlIdx j)const
{
	switch(j)
	{
	case	PAD_BTN_CROSS:			return	(CtrlStatus)_js.rgbButtons[2];
	case	PAD_BTN_CIRCLE:			return	(CtrlStatus)_js.rgbButtons[1];
	case	PAD_BTN_SQUARE:			return	(CtrlStatus)_js.rgbButtons[3];
	case	PAD_BTN_TRIANGLE:		return	(CtrlStatus)_js.rgbButtons[0];
	case	PAD_BTN_START:			return	(CtrlStatus)_js.rgbButtons[8];
	case	PAD_BTN_SELECT:			return	(CtrlStatus)_js.rgbButtons[9];
	case	PAD_BTN_R1:				return	(CtrlStatus)_js.rgbButtons[7];
	case	PAD_BTN_R2:				return	(CtrlStatus)_js.rgbButtons[5];
	case	PAD_BTN_L1:				return	(CtrlStatus)_js.rgbButtons[6];
	case	PAD_BTN_L2:				return	(CtrlStatus)_js.rgbButtons[4];
	case	PAD_BTN_UP:				return	(CtrlStatus)(_js.rgdwPOV[0]&0xFF);
	case	PAD_BTN_DOWN:			return	(CtrlStatus)(_js.rgdwPOV[1]&0xFF);
	case	PAD_BTN_LEFT:			return	(CtrlStatus)(_js.rgdwPOV[2]&0xFF);
	case	PAD_BTN_RIGHT:			return	(CtrlStatus)(_js.rgdwPOV[3]&0xFF);
	case	PAD_BTN_OVER_AXIS1:		return	(CtrlStatus)_js.rgbButtons[10];
	case	PAD_BTN_OVER_AXIS2:		return	(CtrlStatus)_js.rgbButtons[11];
	case	PAD_AXIS_X:				return	(CtrlStatus)(_js.lX&0xFF);
	case	PAD_AXIS_Y:				return	(CtrlStatus)(0xFF-(_js.lY&0xFF));
	case	PAD_AXIS_Z:				return	(CtrlStatus)(_js.lZ&0xFF);
	case	PAD_AXIS_ZROTATION:		return	(CtrlStatus)(_js.lRz&0xFF);
	default:;
	}
	return	0;
}

bool IsXboxPad(const DIDEVICEINSTANCE* pCurrentDevice)
{
	const char* pName =pCurrentDevice->tszInstanceName;
	bool bIsXBOXPad = (pName[0]=='X') &&
		(pName[1]=='B') &&
		(pName[2]=='O') &&
		(pName[3]=='X');
	return bIsXBOXPad;
}

// ********************************************
//	Name: EnumJoysticksCallback()
//	Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
// ********************************************
BOOL CALLBACK PcPadManager::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
{
	if (IsXboxPad(pdidInstance)==false)
	{
		return DIENUM_CONTINUE;
	}

	HRESULT			hr;
	PcPadManager*	pthis =(PcPadManager*)pContext;

	// Obtain an interface to the enumerated joystick.
	hr = pthis->_pDI->CreateDevice(pdidInstance->guidInstance,&(pthis->_pJoystick),NULL);

	// If it failed, then we can't use this joystick. (Maybe the user unplugged it while we were in the middle of enumerating it.)
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	// Stop enumeration. Note: we're just taking the first joystick we get. You could store all the enumerated joysticks and let the user pick.
	return DIENUM_STOP;
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

	static int nSliderCount =0;		// Number of returned slider controls
	static int nPOVCount	=0;		// Number of returned POV controls

	// For axes that are returned, set the DIPROP_RANGE property for the enumerated axis in order to scale min/max values.
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg; 
		diprg.diph.dwSize       =sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize =sizeof(DIPROPHEADER); 
		diprg.diph.dwHow        =DIPH_BYID; 
		diprg.diph.dwObj        =pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin              =0; 
		diprg.lMax              =255;

		// Set the range for the axis
		pthis->_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
	}

	return DIENUM_CONTINUE;
}
