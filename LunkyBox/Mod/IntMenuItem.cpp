#include "IntMenuItem.h"
#include "../Input/Input.h"
#include "../Config/Config.h"


namespace Mod
{
	IntMenuItem::IntMenuItem(Menu* parentMenu, std::string name, int value, int minValue, int maxValue, int bigValueAdd) : MenuItem(parentMenu, name)
	{
		isEditing = false;
		skipNextUpdate = false;
		this->value = value;
		this->minValue = minValue;
		this->maxValue = maxValue;
		this->bigValueAdd = bigValueAdd;
	}

	void IntMenuItem::Update()
	{
		MenuItem::Update();

		if(skipNextUpdate)
		{
			skipNextUpdate = false;

			return;
		}

		if(isEditing)
		{
			//Value decrease

			if(Input::IsKeyPressed(Config::GetNumber("input_left"), Input::INPUT_MODE::CONTINUOUS))
			{
				ValueAdd(-1);

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_VALUE_CHANGE);
				}
			}


			//Value increase

			if(Input::IsKeyPressed(Config::GetNumber("input_right"), Input::INPUT_MODE::CONTINUOUS))
			{
				ValueAdd(1);

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_VALUE_CHANGE);
				}
			}


			//Big value decrease

			if(Input::IsKeyPressed(Config::GetNumber("input_down"), Input::INPUT_MODE::CONTINUOUS))
			{
				ValueAdd(-bigValueAdd);

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_VALUE_CHANGE);
				}
			}


			//Big value increase

			if(Input::IsKeyPressed(Config::GetNumber("input_up"), Input::INPUT_MODE::CONTINUOUS))
			{
				ValueAdd(bigValueAdd);

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_VALUE_CHANGE);
				}
			}


			//Stop editing

			if(Input::IsKeyPressed(Config::GetNumber("input_select"), Input::INPUT_MODE::CONTINUOUS) || Input::IsKeyPressed(Config::GetNumber("input_back"), Input::INPUT_MODE::CONTINUOUS))
			{
				SetIsEditing(false);

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_GO_BACK);
				}
			}
		}
	}

	void IntMenuItem::Draw(LPDIRECT3DDEVICE9 deviceInterface, int menuItemIndex)
	{
		MenuItem::Draw(deviceInterface, menuItemIndex);


		std::stringstream valueStringStream;

		if(isEditing)
		{
			valueStringStream << "< " << value << " >";
		}
		else
		{
			valueStringStream << value;
		}

		std::string valueString = valueStringStream.str();
		LPCSTR valueLPCSTR = valueString.c_str();
		int textWidth = Drawing::GetTextWidth(fontMenuItem, valueLPCSTR);

		Drawing::DrawText(fontMenuItem, valueLPCSTR, Config::GetNumber("menu_x") + Config::GetNumber("menu_width") - textWidth - 4, Config::GetNumber("menu_y") + TITLE_BOX_HEIGHT + 6 + (ITEM_HEIGHT + MENU_MARGIN) * menuItemIndex, Config::GetNumber("menu_width") - MENU_MARGIN, ITEM_HEIGHT - MENU_MARGIN, parentMenu->selectedMenuItemIndex == menuItemIndex ? color2 : color1);
	}

	void IntMenuItem::Select()
	{
		MenuItem::Select();

		SetIsEditing(!isEditing);
	}

	void IntMenuItem::SetIsEditing(bool isEditing)
	{
		skipNextUpdate = true;
		this->isEditing = isEditing;
		parentMenu->isInputLocked = isEditing;
	}

	void IntMenuItem::ValueAdd(int value)
	{
		int newValue = this->value + value;

		if(newValue < minValue)
		{
			this->value = minValue;
		}
		else if(newValue > maxValue)
		{
			this->value = maxValue;
		}
		else
		{
			this->value = newValue;
		}

		if(OnValueChanged != NULL)
		{
			OnValueChanged(this);
		}
	}
}
