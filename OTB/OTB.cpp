
/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/


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
	// Here we use global pointer to HGE interface.
	// Instead you may use hgeCreate() every
	// time you need access to HGE. Just be sure to
	// have a corresponding hge->Release()
	// for each call to hgeCreate()
	hge = hgeCreate(HGE_VERSION);

	// Set our frame function
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_SHOWSPLASH, false);

	// Set the window title
	hge->System_SetState(HGE_TITLE, "Off the Ball");

	// Run in windowed mode
	// Default window size is 800x600
	hge->System_SetState(HGE_WINDOWED, true);

	TestInput();

	// Don't use BASS for sound
//	hge->System_SetState(HGE_USESOUND, false);

	// Tries to initiate HGE with the states set.
	// If something goes wrong, "false" is returned
	// and more specific description of what have
	// happened can be read with System_GetErrorMessage().
	if(hge->System_Initiate())
	{
		// Starts running FrameFunc().
		// Note that the execution "stops" here
		// until "true" is returned from FrameFunc().
		hge->System_Start();
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBoxA(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	// Now ESC has been pressed or the user
	// has closed the window by other means.

	// Restore video mode and free
	// all allocated resources
	hge->System_Shutdown();

	// Release the HGE interface.
	// If there are no more references,
	// the HGE object will be deleted.
	hge->Release();

	return 0;
}


// OTB.cpp : Defines the entry point for the application.
//
/*
#include "stdafx.h"
#include "OTB.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OTB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OTB));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OTB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OTB);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
*/

