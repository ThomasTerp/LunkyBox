#include "BoolMenuItem.h"
#include "../Config/Config.h"


namespace Mod
{
	BoolMenuItem::BoolMenuItem(Menu* parentMenu, std::string name, bool isOn) : MenuItem(parentMenu, name)
	{
		this->isOn = isOn;
	}

	void BoolMenuItem::Update()
	{
		MenuItem::Update();
	}

	void BoolMenuItem::Draw(LPDIRECT3DDEVICE9 deviceInterface, int menuItemIndex)
	{
		MenuItem::Draw(deviceInterface, menuItemIndex);

		LPCSTR onOffText = isOn ? "ON" : "OFF";
		int textWidth = Drawing::GetTextWidth(fontMenuItem, onOffText);

		Drawing::DrawText(fontMenuItem, onOffText, Config::GetNumber("menu_x") + Config::GetNumber("menu_width") - textWidth - 4, Config::GetNumber("menu_y") + TITLE_BOX_HEIGHT + 6 + (ITEM_HEIGHT + MENU_MARGIN) * menuItemIndex, Config::GetNumber("menu_width") - MENU_MARGIN, ITEM_HEIGHT - MENU_MARGIN, IsInFocus() ? color2 : color1);
	}

	void BoolMenuItem::Select()
	{
		isOn = !isOn;

		MenuItem::Select();
	}
}
