
/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/

#include "..\HGE\hge.h"
#include "..\HGE\hgefont.h"
#include "PcPad.h"

HGE*		hge =0;
hgeFont*	fnt =0;

PcPadManager	PadManager;

bool FrameFunc()
{
	PadManager.Update();

	// By returning "true" we tell HGE
	// to stop running the application.
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	// Continue execution
	return false;
}

bool RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	for (int i=0; i<PcPadManager::PAD_MAX_ENTRIES; ++i)
	{
		fnt->printf(10, i*25.0f, HGETEXT_LEFT, "%d %s", i, SMARTENUM_GET_STRING(PcPadManager::CtrlIdx, i)+4);

		PcPadManager::CtrlStatus status =PadManager.GetCtrlState((PcPadManager::CtrlIdx)i);
		fnt->printf(250, i*25.0f, HGETEXT_LEFT, "%d", status);
	}
	
	hge->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	// init
	hge->System_SetState(HGE_FRAMEFUNC,		FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC,	RenderFunc);
	hge->System_SetState(HGE_SHOWSPLASH,	false);
	hge->System_SetState(HGE_SCREENWIDTH,	1280);
	hge->System_SetState(HGE_SCREENHEIGHT,	720);
	hge->System_SetState(HGE_TITLE,			"Off the Ball");
	hge->System_SetState(HGE_WINDOWED,		true);

	// Tries to initiate HGE with the states set. If something goes wrong, "false" is returned
	// and more specific description of what have happened can be read with System_GetErrorMessage().
	if (hge->System_Initiate())
	{
		HWND hwnd =hge->System_GetState(HGE_HWND);
		PadManager.Init(hwnd);

		// Load a font
		fnt=new hgeFont("Data/font1.fnt");

		hge->System_Start();

		// end of game
		delete fnt;
		PadManager.Kill();
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}


	// Now ESC has been pressed or the user
	// has closed the window by other means.

	// Restore video mode and free all allocated resources
	hge->System_Shutdown();

	// Release the HGE interface. If there are no more references, the HGE object will be deleted.
	hge->Release();

	return 0;
}
