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

#ifndef __INPUTCOMMAND_H__
#define __INPUTCOMMAND_H__

#include "../Base/Base.h"
#include "../Base/SmartEnum.h"
#include "InputDirectX.h"

class InputControl
{
public:
	InputControl(){}
	InputControl(int nOffset, bool bInvert):_nOffset(nOffset), _bInvert(bInvert) {}

	int		_nOffset;
	bool	_bInvert;
};

class InputMapper
{
public:
	SMARTENUM_DECLARE(CtrlIdx
		, PAD_BTN_VALIDATE
		, PAD_BTN_CANCEL
		, PAD_BTN_PAUSE

		, PAD_LEFTPAD_AXIS_X
		, PAD_LEFTPAD_AXIS_Y
		, PAD_RIGHTPAD_AXIS_X
		, PAD_RIGHTPAD_AXIS_Y

		, PAD_TIME_SCALE

		, PAD_BTN_UP
		, PAD_BTN_DOWN
		, PAD_BTN_LEFT
		, PAD_BTN_RIGHT

		, PAD_MAX_ENTRIES
		);

	InputMapper()	{}
	~InputMapper()	{}

	void	SetDeviceIdx(Int32 nDeviceIdx)	{ _nDeviceIdx =nDeviceIdx;	}
	Int32	GetDeviceIdx() const			{ return _nDeviceIdx;		}
	void	InitForXbox();
	void	InitForPS3();
	void	InitForOther();

	const InputControl& GetInputControl(CtrlIdx iControl) const { return _Controls[iControl]; }

protected:
	InputControl	_Controls[PAD_MAX_ENTRIES];
	Int32			_nDeviceIdx;
};

class InputMenu
{
public:
			InputMenu() : _rLastValue(0.0f), _rValueCurrent(0.0f) { }
	Float32				  _rLastValue, _rValueCurrent;
};

class InputCore
{
public:
	SMARTENUM_DECLARE(MenuCtl
		, MC_UP
		, MC_DOWN
		, MC_LEFT
		, MC_RIGHT
		, MC_VALIDATE
		, MC_CANCEL

		, MC_MAX
		)

						InputCore();
						~InputCore();

	int					Init(HWND hWnd);
	void				Kill();

	void				Update();

	const CtrlStatus  	GetCtrlState(int iPlayerIdx, InputMapper::CtrlIdx iControl) const;
	const Float32		GetCtrlStateFloat(int iPlayerIdx, InputMapper::CtrlIdx iControl) const;
	const Float32		GetCtrlStateFloatSymetric(int iPlayerIdx, InputMapper::CtrlIdx iControl) const;

	hgeVector			GetAxisLeft(int iPlayerIdx) const;
	hgeVector			GetAxisRight(int iPlayerIdx) const;

	const Float32		GetMenuInput(const MenuCtl iMenuControl) const;

	typedef InputMenu	InputMenuArray[MC_MAX];

protected:
	InputDirectX	_InputDirectX;
	InputMapper		_Mapper[2];
	InputMenuArray	_InputMenuArray;
};

#endif	//__INPUTCOMMAND_H__
