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

#include "PcPad.h"
#include "base.h"

// ********************************************
//	Ctor
// ********************************************
PcPadManager::PcPadManager()
{
	_hWnd		=NULL;
	_pDirectInput		=NULL;
	_pDInputDevice	=NULL;
	_bInited	=false;
	memset(&_JoyState,0,sizeof(DIJOYSTATE2));
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
							(void**)&_pDirectInput,
							NULL);
	RfxAssert(_pDirectInput);
	RfxAssert(!hr);
	if (!_pDirectInput)		return	-1;
	if (hr)					return	-2;
	
	// Look for a simple joystick we can use for this sample program.
	hr =_pDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,(void*)this,DIEDFL_ATTACHEDONLY);
	RfxAssert(!hr);
	if (hr)					return	-3;

	// Make sure we got a joystick
	RfxAssert(_pDInputDevice);
	if (!_pDInputDevice)	return	-4;

	// Set the data format to "simple joystick" - a predefined data format 
	hr =_pDInputDevice->SetDataFormat(&c_dfDIJoystick2);
	RfxAssert(!hr);

	// Set the cooperative level to let DInput know how this device should interact with the system and with other DInput applications.
	hr =_pDInputDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
	RfxAssert(!hr);

	// Enumerate the joystick objects. The callback function enabled user interface elements for objects that are found, and sets the min/max values property for discovered axes.
	hr =_pDInputDevice->EnumObjects(EnumObjectsCallback,(void*)this, DIDFT_ALL);
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
	if (_pDInputDevice) 
	{
		_pDInputDevice->Unacquire();
	}

	// Release any DirectInput objects.
	RfxRelease(_pDInputDevice);
	RfxRelease(_pDirectInput);

	_bInited =false;
}

// ********************************************
//	Update
// ********************************************
void PcPadManager::Update(void)
{
	RfxAssert(_bInited);

	HRESULT hr;
	RfxAssert(_pDInputDevice);
	if (!_pDInputDevice)		return;

	// Poll the device to read the current state
	hr = _pDInputDevice->Poll();
	if (FAILED(hr))
	{
		// DInput is telling us that the input stream has been interrupted. We aren't tracking any state between polls, so
		// we don't have any special reset that needs to be done. We just re-acquire and try again.
		hr = _pDInputDevice->Acquire();
		while( hr == DIERR_INPUTLOST ) 
		{
			hr = _pDInputDevice->Acquire();
		}

		hr = _pDInputDevice->Poll(); 
		if (!FAILED(hr))
		{
			// Get the input's device state
			_pDInputDevice->GetDeviceState(sizeof(DIJOYSTATE2),&_JoyState);
		}

		return;
	}

	// Get the input's device state
	_pDInputDevice->GetDeviceState(sizeof(DIJOYSTATE2),&_JoyState);

	// Update Left
	_AxisLeft.x =float(_JoyState.lX&0xFF)/128.0f-1.0f;
	_AxisLeft.y =float(255-(_JoyState.lX&0xFF))/128.0f-1.0f;
	// deadzone
	if (fabsf(_AxisLeft.x)<0.15f)	_AxisLeft.x =0;
	if (fabsf(_AxisLeft.y)<0.15f)	_AxisLeft.y =0;

	// Update Right
	_AxisRight.x =float(_JoyState.lRx&0xFF)/128.0f-1.0f;
	_AxisRight.y =float(255-(_JoyState.lRy&0xFF))/128.0f-1.0f;
	if (_AxisRight.Length()<0.3f)
	{
		_AxisRight.x =0;
		_AxisRight.y =0;
	}
}

// ********************************************
//	GetCtrlState
// ********************************************
PcPadManager::CtrlStatus PcPadManager::GetCtrlState(CtrlIdx j)const
{
	switch(j)
	{
	case	PAD_BTN_A:			return	(CtrlStatus)_JoyState.rgbButtons[0];
	case	PAD_BTN_B:			return	(CtrlStatus)_JoyState.rgbButtons[1];
	case	PAD_BTN_X:			return	(CtrlStatus)_JoyState.rgbButtons[2];
	case	PAD_BTN_Y:			return	(CtrlStatus)_JoyState.rgbButtons[3];

	case	PAD_BTN_LEFT_BTN:	return	(CtrlStatus)_JoyState.rgbButtons[4];
	case	PAD_BTN_RIGHT_BTN:	return	(CtrlStatus)_JoyState.rgbButtons[5];

	case	PAD_BTN_SELECT:		return	(CtrlStatus)_JoyState.rgbButtons[6];
	case	PAD_BTN_START:		return	(CtrlStatus)_JoyState.rgbButtons[7];
	case	PAD_BTN_THUMB_L:	return	(CtrlStatus)_JoyState.rgbButtons[8];
	case	PAD_BTN_THUMB_R:	return	(CtrlStatus)_JoyState.rgbButtons[9];

	case	PAD_LEFTPAD_AXIS_X:	return	(CtrlStatus)(_JoyState.lX&0xFF);
	case	PAD_LEFTPAD_AXIS_Y:	return	(CtrlStatus)(0xFF-(_JoyState.lY&0xFF));

	case	PAD_RIGHTPAD_AXIS_X:return	(CtrlStatus)(_JoyState.lRx&0xFF);
	case	PAD_RIGHTPAD_AXIS_Y:return	(CtrlStatus)(0xFF-(_JoyState.lRy&0xFF));

	case	PAD_BTN_UP:			return	(CtrlStatus)(_JoyState.rgdwPOV[0]&0xFF);
	case	PAD_BTN_DOWN:		return	(CtrlStatus)(_JoyState.rgdwPOV[1]&0xFF);
	case	PAD_BTN_LEFT:		return	(CtrlStatus)(_JoyState.rgdwPOV[2]&0xFF);
	case	PAD_BTN_RIGHT:		return	(CtrlStatus)(_JoyState.rgdwPOV[3]&0xFF);
	case	PAD_BTN_OVER_AXIS1:	return	(CtrlStatus)_JoyState.rgbButtons[10];
	case	PAD_BTN_OVER_AXIS2:	return	(CtrlStatus)_JoyState.rgbButtons[11];
	case	PAD_AXIS_Z:			return	(CtrlStatus)(_JoyState.lZ&0xFF);
	default:;
	}
	return	0;
}

bool IsXboxPad(const DIDEVICEINSTANCE* pCurrentDevice)
{
	const char* pName =pCurrentDevice->tszInstanceName;
	const char* p=strstr(pName, "XBOX");
	//bool bIsXBOXPad = (pName[0]=='X') && (pName[1]=='B') && (pName[2]=='O') && (pName[3]=='X');
	return (p!=NULL);
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

	PcPadManager* pthis =(PcPadManager*)pContext;

	// Obtain an interface to the enumerated joystick.
	HRESULT hr = pthis->_pDirectInput->CreateDevice(pdidInstance->guidInstance,&(pthis->_pDInputDevice),NULL);
	if (FAILED(hr))
	{
		// If it failed, then we can't use this joystick. (Maybe the user unplugged it while we were in the middle of enumerating it.)
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
		pthis->_pDInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	}

	return DIENUM_CONTINUE;
}
