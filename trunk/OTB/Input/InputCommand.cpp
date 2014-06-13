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

#include "InputCommand.h"

#include "../OTB.h"

// ****************************************************************************************
//	InitForXbox
// ****************************************************************************************
void InputMapper::InitForXbox()
{
	_Controls[PAD_BTN_VALIDATE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[0]	), false );
	_Controls[PAD_BTN_CANCEL		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[1]	), false );
	_Controls[PAD_BTN_PAUSE			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[7]	), false );
	_Controls[PAD_LEFTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lX			), false );
	_Controls[PAD_LEFTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lY			), true	 );
	_Controls[PAD_RIGHTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lRx			), false );
	_Controls[PAD_RIGHTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lRy			), true  );
	_Controls[PAD_TIME_SCALE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lZ			), true	 );
	_Controls[PAD_BTN_DPAD			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[0]	), false );
}

// ****************************************************************************************
//	InitForPS3
// ****************************************************************************************
void InputMapper::InitForPS3()
{
	_Controls[PAD_BTN_VALIDATE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[2]	), false );
	_Controls[PAD_BTN_CANCEL		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[1]	), false );
	_Controls[PAD_BTN_PAUSE			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[8]	), false );
	_Controls[PAD_LEFTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lX		   	), false );
	_Controls[PAD_LEFTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lY		   	), true	 );
	_Controls[PAD_RIGHTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lZ		   	), false );
	_Controls[PAD_RIGHTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lRz		   	), true  );
	_Controls[PAD_TIME_SCALE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rglSlider[0]	), false );
	_Controls[PAD_BTN_DPAD			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[0]	), false );
}

// ****************************************************************************************
//	InitForOther
// ****************************************************************************************
void InputMapper::InitForOther()
{
	_Controls[PAD_BTN_VALIDATE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[2]	), false );
	_Controls[PAD_BTN_CANCEL		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[1]	), false );
	_Controls[PAD_BTN_PAUSE			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[8]	), false );
	_Controls[PAD_LEFTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lX		   	), false );
	_Controls[PAD_LEFTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lY		   	), true	 );
	_Controls[PAD_RIGHTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lRz		   	), false );
	_Controls[PAD_RIGHTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rglSlider[0]  ), true  );
	_Controls[PAD_TIME_SCALE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rglSlider[0]	), false );
	_Controls[PAD_BTN_DPAD			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[0]	), false );
}

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
InputCore::InputCore()
: _bDebugDirectX(false)
, _bDebugMenu	(false)
{
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
InputCore::~InputCore()
{
}

// ****************************************************************************************
//	Init
// ****************************************************************************************
int	InputCore::Init(HWND hWnd)
{
	int nRes =_InputDirectX.Init(hWnd);
	if (nRes==0)
	{
		for(UInt32 iPad=0; iPad<2; ++iPad)
		{
			const InputDirectX::Device& device =_InputDirectX.GetDevice(iPad);
			_Mapper[iPad].SetDeviceIdx(iPad);
			switch(device._PadType)
			{
			case InputDirectX::PT_XBOX:
				_Mapper[iPad].InitForXbox();
				break;
			case InputDirectX::PT_PS3:
				_Mapper[iPad].InitForPS3();
				break;
			case InputDirectX::PT_PS4:
				_Mapper[iPad].InitForPS3();
				break;
			default:
				_Mapper[iPad].InitForOther();
				break;
			}
		}
	}

	return nRes;
}

// ****************************************************************************************
//	Kill
// ****************************************************************************************
void InputCore::Kill()
{
	_InputDirectX.Kill();
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Float32 UpdateMenuInput(Float32 rCurrentValue, InputMenu& inputMenu)
{
	if (rCurrentValue<0.5f)
	{
		rCurrentValue =0.0f;
	}
	else
	{
		rCurrentValue =1.0f;
	}

	if (rCurrentValue!=inputMenu._rLastValue)
	{
		inputMenu._rValueCurrent =rCurrentValue;
	}
	else
	{
		inputMenu._rValueCurrent =0.0f;
	}

	inputMenu._rLastValue =rCurrentValue;
	return rCurrentValue;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// ****************************************************************************************
//	Update
// ****************************************************************************************
void InputCore::Update()
{
	_InputDirectX.Update();

	// update menu mapper

	// ---- UP ----
	const UInt32 nDpad =GetCtrlStateRaw(0, InputMapper::PAD_BTN_DPAD);
	{
		Float32 rAnalog	 =TClamp(GetCtrlStateFloatSymetric(0, InputMapper::PAD_LEFTPAD_AXIS_Y), 0.0f, 1.0f);
		Float32 rDigital =(nDpad==0)?1.0f:0.0f;
		UpdateMenuInput(TMax(rAnalog, rDigital), _InputMenuArray[MC_UP]);
	}

	// ---- DOWN ----
	{
		Float32 rAnalog	 =TClamp(-GetCtrlStateFloatSymetric(0, InputMapper::PAD_LEFTPAD_AXIS_Y), 0.0f, 1.0f);
		Float32 rDigital =(nDpad==18000)?1.0f:0.0f;
		UpdateMenuInput(TMax(rAnalog, rDigital), _InputMenuArray[MC_DOWN]);
	}

	// ---- LEFT ----
	{
		Float32 rAnalog	 =TClamp(-GetCtrlStateFloatSymetric(0, InputMapper::PAD_LEFTPAD_AXIS_X), 0.0f, 1.0f);
		Float32 rDigital =(nDpad==27000)?1.0f:0.0f;
		UpdateMenuInput(TMax(rAnalog, rDigital), _InputMenuArray[MC_LEFT]);
	}

	// ---- RIGHT ----
	{
		Float32 rAnalog	 =TClamp(GetCtrlStateFloatSymetric(0, InputMapper::PAD_LEFTPAD_AXIS_X), 0.0f, 1.0f);
		Float32 rDigital =(nDpad==9000)?1.0f:0.0f;
		UpdateMenuInput(TMax(rAnalog, rDigital), _InputMenuArray[MC_RIGHT]);
	}

	// ---- VALIDATE ----
	{
		Float32 rAnalog	 =TClamp(GetCtrlStateFloat(0, InputMapper::PAD_BTN_VALIDATE), 0.0f, 1.0f);
		UpdateMenuInput(rAnalog, _InputMenuArray[MC_VALIDATE]);
	}

	// ---- CANCEL ----
	{
		Float32 rAnalog	 =TClamp(GetCtrlStateFloat(0, InputMapper::PAD_BTN_CANCEL), 0.0f, 1.0f);
		UpdateMenuInput(rAnalog, _InputMenuArray[MC_CANCEL]);
	}

	// ---- PAUSE ----
	{
		Float32 rAnalog	 =TClamp(GetCtrlStateFloat(0, InputMapper::PAD_BTN_PAUSE), 0.0f, 1.0f);
		UpdateMenuInput(rAnalog, _InputMenuArray[MC_PAUSE]);
	}

	int t=0;
}

// ****************************************************************************************
//	GetCtrlStateRaw
// ****************************************************************************************
const Int32 InputCore::GetCtrlStateRaw(int iPlayerIdx, InputMapper::CtrlIdx iControl) const
{
	const InputMapper& inputMapper =_Mapper[iPlayerIdx];

	DeviceIdx nDeviceIdx =inputMapper.GetDeviceIdx();

	const InputDirectX::Device& Pad =_InputDirectX.GetDevice(nDeviceIdx);

	const InputControl& InputMap =inputMapper.GetInputControl(iControl);

	const UInt8* pRawDataBase =(const UInt8*)&Pad._JoyState;

	LONG* pData =(LONG*)(pRawDataBase+InputMap._nOffset);
	return *pData;
}

// ****************************************************************************************
//	GetCtrlState
// ****************************************************************************************
const CtrlStatus InputCore::GetCtrlState(int iPlayerIdx, InputMapper::CtrlIdx iControl) const
{
	TAssert(iPlayerIdx<2);

	const InputMapper& inputMapper =_Mapper[iPlayerIdx];

	DeviceIdx nDeviceIdx =inputMapper.GetDeviceIdx();

	const InputDirectX::Device& Pad =_InputDirectX.GetDevice(nDeviceIdx);

	const InputControl& InputMap =inputMapper.GetInputControl(iControl);

	const UInt8* pRawDataBase =(const UInt8*)&Pad._JoyState;

	LONG* pData =(LONG*)(pRawDataBase+InputMap._nOffset);

	LONG FinalData;
	if (InputMap._bInvert==false)
	{
		FinalData =*pData&0xFF;
	}
	else
	{
		FinalData =0xFF-(*pData&0xFF);
	}
	return (CtrlStatus)FinalData;
}

// ****************************************************************************************
//	GetCtrlStateFloat
// ****************************************************************************************
const Float32 InputCore::GetCtrlStateFloat(int iPlayerIdx, InputMapper::CtrlIdx iControl) const
{
	CtrlStatus status =GetCtrlState(iPlayerIdx, iControl);
	return (Float32(status)/255.0f);
}

// ****************************************************************************************
//	GetCtrlStateFloatSymetric
// ****************************************************************************************
const Float32 InputCore::GetCtrlStateFloatSymetric(int iPlayerIdx, InputMapper::CtrlIdx iControl) const
{
	CtrlStatus status =GetCtrlState(iPlayerIdx, iControl);
	return Float32(status)/128.0f-1.0f;
}

// ****************************************************************************************
//	GetAxisLeft
// ****************************************************************************************
hgeVector InputCore::GetAxisLeft(int iPlayerIdx) const
{
	hgeVector vAxisLeft;
	vAxisLeft.x =GetCtrlStateFloatSymetric(iPlayerIdx, InputMapper::PAD_LEFTPAD_AXIS_X);
	vAxisLeft.y =GetCtrlStateFloatSymetric(iPlayerIdx, InputMapper::PAD_LEFTPAD_AXIS_Y);
	return vAxisLeft;
}

// ****************************************************************************************
//	GetAxisRight
// ****************************************************************************************
hgeVector InputCore::GetAxisRight(DeviceIdx iPlayerIdx) const
{
	hgeVector vAxisRight;
	vAxisRight.x =GetCtrlStateFloatSymetric(iPlayerIdx, InputMapper::PAD_RIGHTPAD_AXIS_X);
	vAxisRight.y =GetCtrlStateFloatSymetric(iPlayerIdx, InputMapper::PAD_RIGHTPAD_AXIS_Y);
	return vAxisRight;
}

// ****************************************************************************************
//	GetMenuInput
// ****************************************************************************************
const Float32 InputCore::GetMenuInput(const MenuCtl iMenuControl) const
{
	return _InputMenuArray[iMenuControl]._rValueCurrent;
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void InputCore::Render()
{
	Otb* pOtb =Otb::GetInstance();
	hgeFont* pFont =pOtb->GetResources()._pFontDebug;

	Float32 rTextPosY=7.6f;

	if (_bDebugDirectX)
	{
		for(UInt32 i=0; i<InputMapper::PAD_MAX_ENTRIES; ++i)
		{
			pFont->SetColor(0xFFFFA000);
			pFont->printf(-7.4f, rTextPosY, HGETEXT_LEFT, "%d %s", i, SMARTENUM_GET_STRING(InputMapper::CtrlIdx, i)+4);
			pFont->SetColor(0xFFFF00A0);
			{
				Float32 rValue =GetCtrlStateFloat(0, (InputMapper::CtrlIdx)i);
				pFont->printf(-3, rTextPosY, HGETEXT_LEFT, "%.2f", rValue);
			}
			rTextPosY-=0.3f;
		}
	}

	if (_bDebugMenu)
	{
		for(UInt32 iMc=0; iMc<MC_MAX; ++iMc)
		{
			pFont->SetColor(0xFFFFA000);
			pFont->printf(-7.4f, rTextPosY, HGETEXT_LEFT, "%d %s", iMc, SMARTENUM_GET_STRING(MenuCtl, iMc)+3);

			Float32 rValue =GetMenuInput((MenuCtl)iMc);
			pFont->printf(-3, rTextPosY, HGETEXT_LEFT, "%.2f", rValue);
			rTextPosY-=0.3f;
		}
	}
}
