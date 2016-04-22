#pragma once

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <memory>
#include <vector>
#include <sstream>
#include <functional>
#include "Mod.h"
#include "MenuItem.h"
#include "../Drawing/Drawing.h"


namespace Mod
{
	const int MENU_MARGIN = 4;
	const int TITLE_BOX_HEIGHT = 60;
	const int ITEM_HEIGHT = 30;

	const char* const SOUND_OPEN = "pause_in.wav";
	const char* const SOUND_CLOSE = "pause_out.wav";
	const char* const SOUND_SELECTED_ITEM_PREV_NEXT = "menu_ver.wav";
	const char* const SOUND_PAGE_PREV_NEXT = "menu_swipe.wav";
	const char* const SOUND_SELECT = "menu_selection.wav";
	const char* const SOUND_GO_BACK = "menu_ret.wav";
	const char* const SOUND_VALUE_CHANGE = "up.wav";
	const char* const SOUND_ERROR = "uhoh.wav";


	class MenuItem;

	class Menu
	{
		public:
		static Menu* currentMenu;
		static Menu* previousMenu;
		std::string name;
		Menu* referrerMenu;
		int page;
		int selectedMenuItemIndex;
		std::vector<MenuItem*> menuItems;
		bool isInputLocked = false;


		static void ToggleMenu();

		static void OpenMenu();

		static void CloseMenu();

		static void SetCurrentMenu(Menu* menu, Menu* referrerMenu);

		Menu(std::string name);

		void Update();

		void Draw(LPDIRECT3DDEVICE9 deviceInterface);

		void AddMenuItem(MenuItem* menuItem);

		void PreviousMenuItem();

		void NextMenuItem();

		void PreviousPage();

		void NextPage();

		void GoBack();

		std::vector<MenuItem*> GetMenuItemsOnPage(int page);

		MenuItem* GetSelectedMenuItem();

		int Menu::GetMenuItemsOnPageCount(int page);

		int GetPageCount();
	};
}
