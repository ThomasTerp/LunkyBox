#pragma once

#include "MenuItem.h"


namespace Mod
{
	class BoolMenuItem : public MenuItem
	{
		public:
		bool isOn;

		BoolMenuItem(Menu* parentMenu, std::string name, bool isOn = false);

		virtual void Update();

		virtual void Draw(LPDIRECT3DDEVICE9 deviceInterface, int menuItemIndex);

		virtual void Select();
	};
}
