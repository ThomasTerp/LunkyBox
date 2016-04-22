#pragma once

#include "MenuItem.h"


namespace Mod
{
	class IntMenuItem : public MenuItem
	{
		public:
		std::function<void(MenuItem*)> OnValueChanged;
		bool isEditing;
		bool skipNextUpdate;
		int value;
		int minValue;
		int maxValue;
		int bigValueAdd;

		IntMenuItem(Menu* parentMenu, std::string name, int value = 0, int minValue = 0, int maxValue = 100, int bigValueAdd = 10);

		virtual void Update();

		virtual void Draw(LPDIRECT3DDEVICE9 deviceInterface, int menuItemIndex);

		virtual void Select();

		void SetIsEditing(bool isEditing);

		void ValueAdd(int value);
	};
}
