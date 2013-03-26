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


// ********************************************
//	InitForXbox
// ********************************************
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
	_Controls[PAD_BTN_UP			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[0]	), false );
	_Controls[PAD_BTN_DOWN			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[1]	), false );
	_Controls[PAD_BTN_LEFT			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[2]	), false );
	_Controls[PAD_BTN_RIGHT			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[3]	), false );
}

// ********************************************
//	InitForPS3
// ********************************************
void InputMapper::InitForPS3()
{
	_Controls[PAD_BTN_VALIDATE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[0]	), false );
	_Controls[PAD_BTN_CANCEL		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[1]	), false );
	_Controls[PAD_BTN_PAUSE			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[7]	), false );
	_Controls[PAD_LEFTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lX		   	), false );
	_Controls[PAD_LEFTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lY		   	), true	 );
	_Controls[PAD_RIGHTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lZ		   	), false );
	_Controls[PAD_RIGHTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lRz		   	), true  );
	_Controls[PAD_TIME_SCALE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rglSlider[0]	), false );
	_Controls[PAD_BTN_UP			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[0]   	), false );
	_Controls[PAD_BTN_DOWN			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[1]   	), false );
	_Controls[PAD_BTN_LEFT			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[2]   	), false );
	_Controls[PAD_BTN_RIGHT			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[3]   	), false );
}

// ********************************************
//	InitForOther
// ********************************************
void InputMapper::InitForOther()
{
	_Controls[PAD_BTN_VALIDATE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[0]	), false );
	_Controls[PAD_BTN_CANCEL		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[1]	), false );
	_Controls[PAD_BTN_PAUSE			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgbButtons[7]	), false );
	_Controls[PAD_LEFTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lX		   	), false );
	_Controls[PAD_LEFTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lY		   	), true	 );
	_Controls[PAD_RIGHTPAD_AXIS_X	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, lRz		   	), false );
	_Controls[PAD_RIGHTPAD_AXIS_Y	] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rglSlider[0]  ), true  );
	_Controls[PAD_TIME_SCALE		] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rglSlider[0]	), false );
	_Controls[PAD_BTN_UP			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[0]   	), false );
	_Controls[PAD_BTN_DOWN			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[1]   	), false );
	_Controls[PAD_BTN_LEFT			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[2]   	), false );
	_Controls[PAD_BTN_RIGHT			] =InputControl( GET_ATTR_OFFSET(DIJOYSTATE2, rgdwPOV[3]   	), false );
}

// ********************************************
//	Ctor
// ********************************************
InputCore::InputCore()
{
}

// ********************************************
//	Dtor
// ********************************************
InputCore::~InputCore()
{
}

// ********************************************
//	Init
// ********************************************
int	InputCore::Init(HWND	hWnd)
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
			default:
				_Mapper[iPad].InitForOther();
				break;
			}
		}
	}

	return nRes;
}

// ********************************************
//	Kill
// ********************************************
void InputCore::Kill()
{
	_InputDirectX.Kill();
}

// ********************************************
//	Update
// ********************************************
void InputCore::Update()
{
	_InputDirectX.Update();
}

// ********************************************
//	GetCtrlState
// ********************************************
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

// ********************************************
//	GetCtrlStateFloat
// ********************************************
const Float32 InputCore::GetCtrlStateFloat(int iPlayerIdx, InputMapper::CtrlIdx iControl) const
{
	CtrlStatus status =GetCtrlState(iPlayerIdx, iControl);
	return Float32(status)/255.0f;
}

// ********************************************
//	GetCtrlStateFloatSymetric
// ********************************************
const Float32 InputCore::GetCtrlStateFloatSymetric(int iPlayerIdx, InputMapper::CtrlIdx iControl) const
{
	CtrlStatus status =GetCtrlState(iPlayerIdx, iControl);
	return Float32(status)/128.0f-1.0f;
}

// ********************************************
//	GetAxisLeft
// ********************************************
hgeVector InputCore::GetAxisLeft(int iPlayerIdx) const
{
	hgeVector vAxisLeft;
	vAxisLeft.x =GetCtrlStateFloatSymetric(iPlayerIdx, InputMapper::PAD_LEFTPAD_AXIS_X);
	vAxisLeft.y =GetCtrlStateFloatSymetric(iPlayerIdx, InputMapper::PAD_LEFTPAD_AXIS_Y);
	return vAxisLeft;
}

// ********************************************
//	GetAxisRight
// ********************************************
hgeVector InputCore::GetAxisRight(DeviceIdx iPlayerIdx) const
{
	hgeVector vAxisRight;
	vAxisRight.x =GetCtrlStateFloatSymetric(iPlayerIdx, InputMapper::PAD_RIGHTPAD_AXIS_X);
	vAxisRight.y =GetCtrlStateFloatSymetric(iPlayerIdx, InputMapper::PAD_RIGHTPAD_AXIS_Y);
	return vAxisRight;
}
