
/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/

test commit


#include "..\HGE\hge.h"

HGE *hge = 0;

// This function will be called by HGE once per frame.
// Put your game loop code here. In this example we
// just check whether ESC key has been pressed.
bool FrameFunc()
{
	// By returning "true" we tell HGE
	// to stop running the application.
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	// Continue execution
	return false;
}

#include "DirectX9Sdk/Include/d3d9.h"
#include "DirectX9Sdk/Include/d3dx9.h"
#define DIRECTINPUT_VERSION 0x0800
#include "DirectX9Sdk/Include/dinput.h"            //DirectInput header (NEW)

LPDIRECTINPUT8 _pDIObject=0;           //DirectInput main object
int _nNbDevices = 0;

bool IsXboxPad(const DIDEVICEINSTANCE* pCurrentDevice)
{
	const WCHAR* pName =pCurrentDevice->tszInstanceName;
  	bool bIsXBOXPad = (pName[0]=='X') &&
					  (pName[1]=='B') &&
					  (pName[2]=='O') &&
					  (pName[3]=='X');
	return bIsXBOXPad;
}

BOOL CALLBACK GetNbDevicesCallback(const DIDEVICEINSTANCE* pCurrentDevice, void* pContext) 
{
	if (IsXboxPad(pCurrentDevice))
	{
		++_nNbDevices;
	}
	return DIENUM_CONTINUE;
}

BOOL CALLBACK GetDevicesCallback(const DIDEVICEINSTANCE* pCurrentDevice, void* pContext) 
{
	return DIENUM_CONTINUE;
}

/*
BOOL CALLBACK FOR_INPUT_DX::GetDevicesCallback(const DIDEVICEINSTANCE *	pCurrentDevice,
											   void *					pContext) 
{
	if(!strstr(pCurrentDevice->tszInstanceName, "PLAYSTATION") && !strstr(pCurrentDevice->tszInstanceName, "XBOX"))
	{
		FOR_INPUT_DX*		pForInputDX;
		INPUT_INIT_INFO_DX	initInfo;

		pForInputDX = (FOR_INPUT_DX*)pContext;

		LPDIRECTINPUTDEVICE8 pDID8;
		HRESULT nHr;
		nHr = pForInputDX->_pDI->CreateDevice(pCurrentDevice->guidInstance, &pDID8, NULL ); 
		if (!(FAILED( nHr )))
		{ 
			initInfo.SetDeviceName(pCurrentDevice->tszInstanceName);
			initInfo.SetInputDevice(pDID8);

			if (pForInputDX->GetOwnerForInput()->GetInputDevice(pForInputDX->_nCurrentDevices)->Init(&initInfo) == false)
			{
				QDT::KCORE::QDT_Warning("Can't Init Input device of %s", initInfo.GetDeviceName().GetBuffer());
			}
		}

		pForInputDX->_nCurrentDevices++;
	}
	return	(DIENUM_CONTINUE); 
}
*/

bool TestInput()
{
	// Create DirectInput object:
	HRESULT nHr = DirectInput8Create( GetModuleHandle(NULL),
										DIRECTINPUT_VERSION,
										IID_IDirectInput8,
										(void**)&_pDIObject,
										NULL ); 

	
	if (FAILED(nHr))
	{
		return false;
	}

	// Count DirectInput devices and creates an INPUT_DEVICE for each ones
	void* pthis=NULL;
	_pDIObject->EnumDevices( DI8DEVCLASS_ALL, GetNbDevicesCallback, (void*)pthis, DIEDFL_ATTACHEDONLY );
	if (_nNbDevices == 0)
	{
		return false;
	}

	_pDIObject->EnumDevices( DI8DEVCLASS_ALL, GetDevicesCallback, (void*)pthis, DIEDFL_ATTACHEDONLY );
	
	return true;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	// init
	hge->System_SetState(HGE_FRAMEFUNC,		FrameFunc);
	hge->System_SetState(HGE_SHOWSPLASH,	false);
	hge->System_SetState(HGE_SCREENWIDTH,	1280);
	hge->System_SetState(HGE_SCREENHEIGHT,	720);
	hge->System_SetState(HGE_TITLE,			"Off the Ball");
	hge->System_SetState(HGE_WINDOWED,		true);

	TestInput();

	// Tries to initiate HGE with the states set. If something goes wrong, "false" is returned
	// and more specific description of what have happened can be read with System_GetErrorMessage().
	if (hge->System_Initiate())
	{
		hge->System_Start();
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBoxA(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	// Now ESC has been pressed or the user
	// has closed the window by other means.

	// Restore video mode and free all allocated resources
	hge->System_Shutdown();

	// Release the HGE interface. If there are no more references, the HGE object will be deleted.
	hge->Release();

	return 0;
}
