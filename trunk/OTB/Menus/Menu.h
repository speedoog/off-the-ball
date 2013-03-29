/*


	Menu


*/

#ifndef __MENU_H__
#define __MENU_H__

#pragma once

class Otb;
class hgeGUI;

class Menu
{
public:
			Menu();
	void	Init(Otb* pOTB);
	void	Kill();

	void	Render();
	void	Update(float dt);

protected:
	Otb*	_pOTB;
	hgeGUI*	_pGUI;
};

#endif //__MENU_H__