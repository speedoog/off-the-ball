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

#define RfxRelease(p) { if(p) { (p)->Release(); (p)=NULL; } }


/**
 *	Constructor
 */
RfxPcPadManager::RfxPcPadManager(void)
{
	hWnd_			=	NULL;
	pDI_			=	NULL;
	pJoystick_=	NULL;
	inited_		=	false;
	memset(&js_,0,sizeof(DIJOYSTATE2));
}

/**
 *	Destructor
 */
RfxPcPadManager::~RfxPcPadManager(void)
{
	RfxAssert(!inited_);
}

/**
 *	init PcPad class
 */
int	RfxPcPadManager::init(HWND	a_hWnd)
{
	RfxAssert(!inited_);

	HRESULT hr;

	hWnd_=a_hWnd;
	// Register with the DirectInput subsystem and get a pointer to a IDirectInput interface we can use.
	hr=DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&pDI_,NULL);
	RfxAssert(pDI_);
	RfxAssert(!hr);
	if (!pDI_)					return	-1;
	if (hr)							return	-2;
	
	// Look for a simple joystick we can use for this sample program.
	hr=pDI_->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,(void*)this,DIEDFL_ATTACHEDONLY);
	RfxAssert(!hr);
	if (hr)							return	-3;

	// Make sure we got a joystick
	RfxAssert(pJoystick_);
	if (!pJoystick_)		return	-4;

	// Set the data format to "simple joystick" - a predefined data format 
	hr=pJoystick_->SetDataFormat(&c_dfDIJoystick2);
	RfxAssert(!hr);

	// Set the cooperative level to let DInput know how this device should interact with the system and with other DInput applications.
	hr=pJoystick_->SetCooperativeLevel(hWnd_,DISCL_EXCLUSIVE| DISCL_FOREGROUND);
	RfxAssert(!hr);

	// Enumerate the joystick objects. The callback function enabled user interface elements for objects that are found, and sets the min/max values property for discovered axes.
	hr=pJoystick_->EnumObjects(EnumObjectsCallback,(void*)this, DIDFT_ALL);
	RfxAssert(!hr);

	inited_=true;

	return	0;
}

/**
 *	deinit
 */
void	RfxPcPadManager::kill(void)
{
	RfxAssert(inited_);

	// Unacquire the device one last time just in case  the app tried to exit while the device is still acquired.
	if(pJoystick_) 
		pJoystick_->Unacquire();

	// Release any DirectInput objects.
	RfxRelease(pJoystick_);
	RfxRelease(pDI_);

	inited_=false;
}

/**
 *	
 */
void	RfxPcPadManager::update(void)
{
	RfxAssert(inited_);
	
	HRESULT     hr;
	RfxAssert(pJoystick_);
	if (!pJoystick_)		return;

	// Poll the device to read the current state
	hr = pJoystick_->Poll(); 
  if( FAILED(hr) )  
  {
		// DInput is telling us that the input stream has been interrupted. We aren't tracking any state between polls, so
		// we don't have any special reset that needs to be done. We just re-acquire and try again.

    hr = pJoystick_->Acquire();
    while( hr == DIERR_INPUTLOST ) 
        hr = pJoystick_->Acquire();

		hr = pJoystick_->Poll(); 
		if(!FAILED(hr))
		{
			// Get the input's device state
			pJoystick_->GetDeviceState(sizeof(DIJOYSTATE2),&js_);
		}

		return;
	}

	// Get the input's device state
	pJoystick_->GetDeviceState(sizeof(DIJOYSTATE2),&js_);

}

/**
 *	map PSX pad on Dinput
 */
RfxPcPadManager::CtrlStatus	RfxPcPadManager::getCtrlState(CtrlIdx j)const
{
	switch(j)
	{
	case	PAD_BTN_CROSS:			return	(CtrlStatus)js_.rgbButtons[2];
	case	PAD_BTN_CIRCLE:			return	(CtrlStatus)js_.rgbButtons[1];
	case	PAD_BTN_SQUARE:			return	(CtrlStatus)js_.rgbButtons[3];
	case	PAD_BTN_TRIANGLE:		return	(CtrlStatus)js_.rgbButtons[0];
	case	PAD_BTN_START:			return	(CtrlStatus)js_.rgbButtons[8];
	case	PAD_BTN_SELECT:			return	(CtrlStatus)js_.rgbButtons[9];
	case	PAD_BTN_R1:					return	(CtrlStatus)js_.rgbButtons[7];
	case	PAD_BTN_R2:					return	(CtrlStatus)js_.rgbButtons[5];
	case	PAD_BTN_L1:					return	(CtrlStatus)js_.rgbButtons[6];
	case	PAD_BTN_L2:					return	(CtrlStatus)js_.rgbButtons[4];
	case	PAD_BTN_UP:					return	(CtrlStatus)(js_.rgdwPOV[0]&0xFF);
	case	PAD_BTN_DOWN:				return	(CtrlStatus)(js_.rgdwPOV[1]&0xFF);
	case	PAD_BTN_LEFT:				return	(CtrlStatus)(js_.rgdwPOV[2]&0xFF);
	case	PAD_BTN_RIGHT:			return	(CtrlStatus)(js_.rgdwPOV[3]&0xFF);
	case	PAD_BTN_OVER_AXIS1:	return	(CtrlStatus)js_.rgbButtons[10];
	case	PAD_BTN_OVER_AXIS2:	return	(CtrlStatus)js_.rgbButtons[11];
	case	PAD_AXIS_X:					return	(CtrlStatus)(js_.lX&0xFF);
	case	PAD_AXIS_Y:					return	(CtrlStatus)(0xFF-(js_.lY&0xFF));
	case	PAD_AXIS_Z:					return	(CtrlStatus)(js_.lZ&0xFF);
	case	PAD_AXIS_ZROTATION:	return	(CtrlStatus)(js_.lRz&0xFF);
	default:;
	}
	return	0;
}

/**
 *	Name: EnumJoysticksCallback()
 *	Desc: Called once for each enumerated joystick. If we find one, create a
 *       device interface on it so we can play with it.
 */
BOOL CALLBACK	RfxPcPadManager::EnumJoysticksCallback(const DIDEVICEINSTANCE* a_pdidInstance,void* a_pContext)
{
	HRESULT						hr;
	RfxPcPadManager*	pthis=(RfxPcPadManager*)a_pContext;

	// Obtain an interface to the enumerated joystick.
	hr = pthis->pDI_->CreateDevice(a_pdidInstance->guidInstance,&(pthis->pJoystick_),NULL);

	// If it failed, then we can't use this joystick. (Maybe the user unplugged it while we were in the middle of enumerating it.)
	if	(FAILED(hr)) 
		return DIENUM_CONTINUE;

	// Stop enumeration. Note: we're just taking the first joystick we get. You could store all the enumerated joysticks and let the user pick.
	return DIENUM_STOP;
}

/**
 * Name: EnumObjectsCallback()
 * Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
 *       joystick. This function enables user interface elements for objects
 *       that are found to exist, and scales axes min/max values.
 */
BOOL CALLBACK RfxPcPadManager::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* a_pdidoi,void* a_pContext)
{
	RfxPcPadManager*	pthis=(RfxPcPadManager*)a_pContext;

	static int nSliderCount = 0;  // Number of returned slider controls
	static int nPOVCount = 0;     // Number of returned POV controls

	// For axes that are returned, set the DIPROP_RANGE property for the enumerated axis in order to scale min/max values.
	if(a_pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg; 
		diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		diprg.diph.dwHow        = DIPH_BYID; 
		diprg.diph.dwObj        = a_pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin              = 0; 
		diprg.lMax              = 255;

		// Set the range for the axis
		pthis->pJoystick_->SetProperty(DIPROP_RANGE,&diprg.diph);
	}

	return DIENUM_CONTINUE;
}
