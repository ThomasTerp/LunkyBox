#include "MenuItem.h"
#include "../Input/Input.h"
#include "../Config/Config.h"


namespace Mod
{
	MenuItem::MenuItem(Menu* parentMenu, std::string name)
	{
		this->parentMenu = parentMenu;
		this->name = name;
		this->hasSound = true;
	}

	void MenuItem::Update()
	{
		if(OnUpdate != NULL)
		{
			OnUpdate(this);
		}
	}

	void MenuItem::Draw(LPDIRECT3DDEVICE9 deviceInterface, int menuItemIndex)
	{
		Drawing::DrawRectangle(deviceInterface, Config::GetNumber("menu_x"), Config::GetNumber("menu_y") + TITLE_BOX_HEIGHT + MENU_MARGIN + (ITEM_HEIGHT + MENU_MARGIN) * menuItemIndex, Config::GetNumber("menu_width"), ITEM_HEIGHT, IsInFocus() ? color1 : color2);
		Drawing::DrawText(fontMenuItem, name.c_str(), Config::GetNumber("menu_x") + 4, Config::GetNumber("menu_y") + TITLE_BOX_HEIGHT + 6 + (ITEM_HEIGHT + MENU_MARGIN) * menuItemIndex, Config::GetNumber("menu_width") - MENU_MARGIN, ITEM_HEIGHT - MENU_MARGIN, IsInFocus() ? color2 : color1);

		if(OnDraw != NULL)
		{
			OnDraw(this, deviceInterface, menuItemIndex);
		}
	}

	void MenuItem::Select()
	{
		if(OnSelect != NULL)
		{
			OnSelect(this);
		}

		if(hasSound && Config::GetBool("menu_sounds"))
		{
			Mod::player.PlaySound(SOUND_SELECT);
		}
	}

	bool MenuItem::IsInFocus()
	{
		return parentMenu->GetSelectedMenuItem() == this;
	}
}
