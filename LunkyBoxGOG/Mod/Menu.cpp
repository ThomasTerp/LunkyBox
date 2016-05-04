#include "Menu.h"
#include "../Input/Input.h"
#include "../Config/Config.h"


namespace Mod
{
	Menu* Menu::currentMenu = NULL;
	Menu* Menu::previousMenu = NULL;


	void Menu::ToggleMenu()
	{
		if(Menu::currentMenu == NULL)
		{
			OpenMenu();
		}
		else
		{
			CloseMenu();
		}
	}

	void Menu::OpenMenu()
	{
		if(Menu::currentMenu == NULL)
		{
			Menu::currentMenu = Menu::previousMenu;

			if(Config::GetBool("menu_sounds"))
			{
				Mod::player.PlaySound(SOUND_OPEN);
			}
		}
	}

	void Menu::CloseMenu()
	{
		if(Menu::currentMenu != NULL)
		{
			Menu::previousMenu = Menu::currentMenu;
			Menu::currentMenu = NULL;

			if(Config::GetBool("menu_sounds"))
			{
				Mod::player.PlaySound(SOUND_CLOSE);
			}
		}
	}

	void Menu::SetCurrentMenu(Menu* menu, Menu* referrerMenu)
	{
		Menu::currentMenu = menu;
		menu->referrerMenu = referrerMenu;
	}

	Menu::Menu(std::string name)
	{
		this->name = name;
		page = 0;
		selectedMenuItemIndex = 0;
	}

	void Menu::Update()
	{
		if(!isInputLocked)
		{
			//Select previous MenuItem

			if(Input::IsKeyPressed(Config::GetNumber("input_up"), Input::INPUT_MODE::CONTINUOUS))
			{
				PreviousMenuItem();

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_SELECTED_ITEM_PREV_NEXT);
				}
			}


			//Select next MenuItem

			if(Input::IsKeyPressed(Config::GetNumber("input_down"), Input::INPUT_MODE::CONTINUOUS))
			{
				NextMenuItem();

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_SELECTED_ITEM_PREV_NEXT);
				}
			}


			//previous page

			if(Input::IsKeyPressed(Config::GetNumber("input_left"), Input::INPUT_MODE::CONTINUOUS))
			{
				PreviousPage();

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_PAGE_PREV_NEXT);
				}
			}


			//Next page

			if(Input::IsKeyPressed(Config::GetNumber("input_right"), Input::INPUT_MODE::CONTINUOUS))
			{
				NextPage();

				if(Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_PAGE_PREV_NEXT);
				}
			}


			//Select

			if(Input::IsKeyPressed(Config::GetNumber("input_select"), Input::INPUT_MODE::CONTINUOUS))
			{
				GetSelectedMenuItem()->Select();
			}


			//Go back

			if(Input::IsKeyPressed(Config::GetNumber("input_back"), Input::INPUT_MODE::CONTINUOUS))
			{
				GoBack();

				if(currentMenu != NULL && Config::GetBool("menu_sounds"))
				{
					Mod::player.PlaySound(SOUND_GO_BACK);
				}
			}
		}


		//Go back if this menu contains no MenuItem's

		if(menuItems.size() == 0)
		{
			GoBack();

			Mod::player.PlaySound(SOUND_ERROR);
		}


		//Fix selected selectedMenuItemIndex being outside the menu

		int menuItemsOnPageCount = GetMenuItemsOnPageCount(page);

		if(selectedMenuItemIndex >= menuItemsOnPageCount)
		{
			selectedMenuItemIndex = menuItemsOnPageCount - 1;
		}


		//Call OnUpdate on all MenuItem's

		std::vector<MenuItem*> menuItemsOnPage = GetMenuItemsOnPage(page);

		for(int menuItemIndex = 0; menuItemIndex < (int)menuItemsOnPage.size(); menuItemIndex++)
		{
			MenuItem* menuItem = menuItemsOnPage[menuItemIndex];

			menuItem->Update();
		}
	}

	void Menu::Draw(LPDIRECT3DDEVICE9 deviceInterface)
	{
		//Draw title box

		int pageCount = GetPageCount();

		Drawing::DrawRectangle(deviceInterface, Config::GetNumber("menu_x"), Config::GetNumber("menu_y"), Config::GetNumber("menu_width"), TITLE_BOX_HEIGHT, color1);
		Drawing::DrawText(fontMenuTitle, name.c_str(), Config::GetNumber("menu_x") + MENU_MARGIN, Config::GetNumber("menu_y") + 4, Config::GetNumber("menu_width") - MENU_MARGIN, TITLE_BOX_HEIGHT - MENU_MARGIN, color2);
		
		if(pageCount > 1)
		{
			std::stringstream pageNumberText;
			pageNumberText << "Page " << page + 1 << " of " << pageCount;

			Drawing::DrawText(fontMenuPageNumber, pageNumberText.str().c_str(), Config::GetNumber("menu_x") + MENU_MARGIN, Config::GetNumber("menu_y") + 38, Config::GetNumber("menu_width") - MENU_MARGIN, TITLE_BOX_HEIGHT - MENU_MARGIN, color2);
		}


		//Draw items

		std::vector<MenuItem*> menuItemsOnPage = GetMenuItemsOnPage(page);

		for(int menuItemIndex = 0; menuItemIndex < (int)menuItemsOnPage.size(); menuItemIndex++)
		{
			MenuItem* menuItem = menuItemsOnPage[menuItemIndex];

			menuItem->Draw(deviceInterface, menuItemIndex);
		}
	}

	void Menu::AddMenuItem(MenuItem* menuItem)
	{
		menuItems.push_back(menuItem);
	}

	void Menu::PreviousMenuItem()
	{

		if(selectedMenuItemIndex <= 0)
		{
			PreviousPage();
			selectedMenuItemIndex = GetMenuItemsOnPageCount(page) - 1;
		}
		else
		{
			selectedMenuItemIndex -= 1;
		}
	}

	void Menu::NextMenuItem()
	{
		if(selectedMenuItemIndex >= GetMenuItemsOnPageCount(page) - 1)
		{
			NextPage();
			selectedMenuItemIndex = 0;
		}
		else
		{
			selectedMenuItemIndex += 1;
		}
	}

	void Menu::PreviousPage()
	{
		if(page <= 0)
		{
			page = GetPageCount() - 1;
		}
		else
		{
			page -= 1;
		}
	}

	void Menu::NextPage()
	{
		if(page >= GetPageCount() -1)
		{
			page = 0;
		}
		else
		{
			page += 1;
		}
	}

	void Menu::GoBack()
	{
		if(referrerMenu == NULL)
		{
			CloseMenu();
		}
		else
		{
			currentMenu = referrerMenu;
		}
	}

	std::vector<MenuItem*> Menu::GetMenuItemsOnPage(int page)
	{
		std::vector<MenuItem*> menuItemsOnPage;

		for(int menuItemIndex = Config::GetNumber("menu_items_per_page") * page; menuItemIndex < min((int)menuItems.size(), Config::GetNumber("menu_items_per_page") * (page + 1)); menuItemIndex++)
		{
			MenuItem* menuItem = menuItems[menuItemIndex];
			menuItemsOnPage.push_back(menuItem);
		}

		return menuItemsOnPage;
	}

	MenuItem* Menu::GetSelectedMenuItem()
	{
		return menuItems[Config::GetNumber("menu_items_per_page") * page + selectedMenuItemIndex];
	}

	int Menu::GetMenuItemsOnPageCount(int page)
	{
		return min((int)menuItems.size(), Config::GetNumber("menu_items_per_page") * (page + 1)) - (Config::GetNumber("menu_items_per_page") * page);
	}

	int Menu::GetPageCount()
	{
		return (int)ceil((menuItems.size() - 1) / Config::GetNumber("menu_items_per_page")) + 1;
	}
}
