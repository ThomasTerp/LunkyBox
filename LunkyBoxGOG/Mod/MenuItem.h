#pragma once

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <vector>
#include <sstream>
#include "Menu.h"
#include "../Drawing/Drawing.h"


namespace Mod
{
	class Menu;

	class MenuItem
	{
		public:
		Menu* parentMenu;
		std::string name;
		std::function<void(MenuItem*)> OnSelect;
		std::function<void(MenuItem*)> OnUpdate;
		std::function<void(MenuItem*, LPDIRECT3DDEVICE9, int)> OnDraw;
		bool hasSound;

		MenuItem(Menu* parentMenu, std::string name);

		virtual void Update();

		virtual void Draw(LPDIRECT3DDEVICE9 deviceInterface, int menuItemIndex);

		virtual void Select();

		bool IsInFocus();
	};
}
