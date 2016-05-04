#include "Mod.h"
#include <limits>
#include <algorithm>
#include "Memory.h"
#include "MenuItem.h"
#include "BoolMenuItem.h"
#include "IntMenuItem.h"
#include "../Vector2.h"
#include "../Window/Window.h"
#include "../Input/Input.h"
#include "../Spelunky/ITEM_CLASS.h"
#include "../Spelunky/ENTITY_CLASS.h"
#include "../Config/Config.h"


namespace Mod
{
	bool isFontsCreated = false;
	bool isMenusCreated = false;

	Spelunky::Player player = NULL;
	bool hasInfiniteHearts = false;
	bool hasInfiniteBombs = false;
	bool hasInfiniteRopes = false;
	bool hasInfiniteAnkhs = false;
	bool hasInfiniteParachutes = false;
	bool isTotalTimeFrozen = false;
	bool isStageTimeFrozen = false;
	byte frozenTotalMinutes = 0;
	byte frozenTotalSeconds = 0;
	byte frozenStageMinutes = 0;
	byte frozenStageSeconds = 0;
	LPD3DXFONT fontMenuTitle = NULL;
	LPD3DXFONT fontMenuPageNumber = NULL;
	LPD3DXFONT fontMenuItem = NULL;
	D3DCOLOR color1 = D3DCOLOR_ARGB(255, 244, 244, 244);
	D3DCOLOR color2 = D3DCOLOR_ARGB(255, 26, 26, 26);


	//Called when the dll is injected
	DWORD WINAPI Start(LPVOID param)
	{
		//Read config

		if(!Config::Parse(CONFIG_FILE))
		{
			MessageBox(NULL, "Failed to parse config file.", TITLE, NULL);

			return NULL;
		}


		//Create main loop

		while(true)
		{
			Update();
		}

		return NULL;
	}

	//Called every tick
	void Update()
	{
		player = Spelunky::Player::GetLocal();

		CreateMenus();

		Input::Update();
		InfiniteStatsUpdate();
		FrozenTimeUpdate();
		MenuUpdate();
	}

	void D3D9Reset(LPDIRECT3DDEVICE9 deviceInterface, D3DPRESENT_PARAMETERS* presentationParameters)
	{

	}

	//Called every frame
	void D3D9EndScene(LPDIRECT3DDEVICE9 deviceInterface)
	{
		CreateFonts(deviceInterface);

		if(player.address != NULL && Menu::currentMenu != NULL)
		{
			Menu::currentMenu->Draw(deviceInterface);
		}
	}

	void MenuUpdate()
	{
		if(player.address != NULL)
		{
			if(Input::IsKeyPressed(Config::GetNumber("input_toggle_menu"), Input::INPUT_MODE::CONTINUOUS))
			{
				Menu::ToggleMenu();
			}

			if(Menu::currentMenu != NULL)
			{
				Menu::currentMenu->Update();
			}
		}
	}

	void InfiniteStatsUpdate()
	{
		if(hasInfiniteHearts)
		{
			player.SetHearts(99);
		}

		if(hasInfiniteBombs)
		{
			player.SetBombs(99);
		}

		if(hasInfiniteRopes)
		{
			player.SetRopes(99);
		}

		if(hasInfiniteAnkhs)
		{
			player.SetHasItem(Spelunky::ITEM_CLASS::ANKH, true);
		}

		if(hasInfiniteParachutes)
		{
			player.SetHasItem(Spelunky::ITEM_CLASS::PARACHUTE, true);
		}
	}

	void FrozenTimeUpdate()
	{
		if(isTotalTimeFrozen)
		{
			Spelunky::SetTotalMinutes(frozenTotalMinutes);
			Spelunky::SetTotalSeconds(frozenTotalSeconds);
			Spelunky::SetTotalMilliseconds(0);
		}

		if(isStageTimeFrozen)
		{
			Spelunky::SetStageMinutes(frozenStageMinutes);
			Spelunky::SetStageSeconds(frozenStageSeconds);
			Spelunky::SetStageMilliseconds(0);
		}
	}

	void CreateFonts(LPDIRECT3DDEVICE9 deviceInterface)
	{
		if(!isFontsCreated)
		{
			isFontsCreated = true;

			D3DXCreateFont(deviceInterface, 32, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT(FONT), &fontMenuTitle);
			D3DXCreateFont(deviceInterface, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT(FONT), &fontMenuPageNumber);
			D3DXCreateFont(deviceInterface, 26, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT(FONT), &fontMenuItem);
		}
	}

	void CreateMenus()
	{
		if(!isMenusCreated)
		{
			isMenusCreated = true;


			//Add a MenuItem for going to another menu
			std::function<void(Menu*, std::string, Menu*)> AddLinkMenuItem = [](Menu* menu, std::string name, Menu* destinationMenu)
			{
				MenuItem* linkMenuItem = new MenuItem(menu, name);
				linkMenuItem->OnSelect = [menu, destinationMenu](MenuItem* menuItem)
				{
					Menu::SetCurrentMenu(destinationMenu, menu);
				};
				menu->AddMenuItem(linkMenuItem);
			};


			//Add a BoolMenuItem for turning a player item on/off
			std::function<void(Menu*, std::string, Spelunky::ITEM_CLASS)> AddItemBoolMenuItem = [](Menu* menu, std::string name, Spelunky::ITEM_CLASS itemClass)
			{
				BoolMenuItem* itemBoolMenuItem = new BoolMenuItem(menu, name);
				itemBoolMenuItem->OnSelect = [itemClass](MenuItem* menuItem)
				{
					BoolMenuItem* itemBoolMenuItem = (BoolMenuItem*)menuItem;

					player.SetHasItem(itemClass, itemBoolMenuItem->isOn);
				};
				itemBoolMenuItem->OnUpdate = [itemClass](MenuItem* menuItem)
				{
					BoolMenuItem* itemBoolMenuItem = (BoolMenuItem*)menuItem;

					itemBoolMenuItem->isOn = player.HasItem(itemClass);
				};
				menu->AddMenuItem(itemBoolMenuItem);
			};


			//Add a BoolMenuItem for turning all player items to one state
			std::function<void(Menu*, std::string, bool)> AddAllItemsMenuItem = [](Menu* menu, std::string name, bool hasItem)
			{
				MenuItem* allItemsBoolMenuItem = new MenuItem(menu, name);
				allItemsBoolMenuItem->OnSelect = [hasItem](MenuItem* menuItem)
				{
					player.SetHasItem(Spelunky::ITEM_CLASS::ANKH, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::BOOK_OF_THE_DEAD, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::HEDJET, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::UDJAT_EYE, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::CLIMBING_GLOVES, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::COMPASS, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::CRYSKNIFE, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::KAPALA, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::PARACHUTE, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::PASTE, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::PITCHERS_MITT, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::SPECTACLES, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::SPIKE_SHOES, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::SPRING_SHOES, hasItem);
					player.SetHasItem(Spelunky::ITEM_CLASS::VLADS_AMULET, hasItem);
				};
				menu->AddMenuItem(allItemsBoolMenuItem);
			};


			float* moveDistance = new float(1);

			//Add a MenuItem for teleporting relative to the player
			std::function<void(Menu*, std::string, Vector2)> AddRelativeMoveMenuItem = [moveDistance](Menu* menu, std::string name, Vector2 relativePosition)
			{
				MenuItem* relativeMoveMenuItem = new MenuItem(menu, name);
				relativeMoveMenuItem->OnSelect = [moveDistance, relativePosition](MenuItem* menuItem)
				{
					float moveDistanceValue = *moveDistance;

					Vector2 newPosition = player.GetPosition();
					newPosition.x += relativePosition.x * moveDistanceValue;
					newPosition.y += relativePosition.y * moveDistanceValue;

					player.Teleport(newPosition);
				};
				menu->AddMenuItem(relativeMoveMenuItem);
			};


			bool* isCursorControlled = new bool(true);
			bool* useCursorVelocity = new bool(false);
			Vector2* pressedPosition = new Vector2(0, 0);

			//Add a MenuItem for spawning an entity
			std::function<void(Menu*, std::string, Spelunky::ENTITY_CLASS)> AddSpawnEntityMenuItem = [isCursorControlled, useCursorVelocity, pressedPosition](Menu* menu, std::string name, Spelunky::ENTITY_CLASS entityClass)
			{
				std::stringstream nameStream;
				nameStream << (DWORD)entityClass << " " << name;

				MenuItem* spawnEntityMenuItem = new MenuItem(menu, nameStream.str());
				spawnEntityMenuItem->OnUpdate = [isCursorControlled, useCursorVelocity, pressedPosition, entityClass](MenuItem* menuItem)
				{
					if(*isCursorControlled && menuItem->IsInFocus())
					{
						Vector2 cursorPosition = Spelunky::ScreenToWorld(Window::GetRelativeCursorPosition());

						if(*useCursorVelocity)
						{
							if(Input::IsKeyPressed(VK_LBUTTON, Input::INPUT_MODE::ONCE))
							{
								(*pressedPosition).x = cursorPosition.x;
								(*pressedPosition).y = cursorPosition.y;
							}

							if(Input::IsKeyReleased(VK_LBUTTON, Input::INPUT_MODE::ONCE))
							{
								Vector2 velocity = Vector2(
									(cursorPosition.x - (*pressedPosition).x) * 0.1f,
									(cursorPosition.y - (*pressedPosition).y) * 0.1f
								);

								Spelunky::Entity entity = Spelunky::SpawnEntity(Vector2((*pressedPosition).x, (*pressedPosition).y), entityClass);
								entity.SetIsFlipped(player.IsFlipped());
								entity.SetVelocity(velocity);

								if(Config::GetBool("menu_sounds"))
								{
									player.PlaySound(SOUND_SELECT);
								}
							}
						}
						else if(Input::IsKeyPressed(VK_LBUTTON, Input::INPUT_MODE::CONTINUOUS))
						{
							Spelunky::Entity entity = Spelunky::SpawnEntity(cursorPosition, entityClass);
							entity.SetIsFlipped(player.IsFlipped());

							if(Config::GetBool("menu_sounds"))
							{
								player.PlaySound(SOUND_SELECT);
							}
						}
					}
				};
				spawnEntityMenuItem->OnDraw = [isCursorControlled, useCursorVelocity, pressedPosition, entityClass](MenuItem* menuItem, LPDIRECT3DDEVICE9 deviceInterface, int menuItemIndex)
				{
					if((*pressedPosition).x != 0 && (*pressedPosition).y != 0)
					{
						Vector2 cursorPosition = Spelunky::ScreenToWorld(Window::GetRelativeCursorPosition());
					}
				};
				spawnEntityMenuItem->OnSelect = [isCursorControlled, entityClass](MenuItem* menuItem)
				{
					Vector2 position = player.GetPosition();
					position.x += player.IsFlipped() ? -2 : 2;
					position.y += 0.6f;

					Spelunky::Entity entity = Spelunky::SpawnEntity(position, entityClass);
					entity.SetIsFlipped(player.IsFlipped());
				};
				menu->AddMenuItem(spawnEntityMenuItem);
			};


			//Add a MenuItem for playing a sound
			std::function<void(Menu*, char*)> AddSoundMenuItem = [](Menu* menu, char* sound)
			{
				MenuItem* soundMenuItem = new MenuItem(menu, sound);
				soundMenuItem->hasSound = false;
				soundMenuItem->OnSelect = [sound](MenuItem* menuItem)
				{
					player.PlaySound(sound);
				};
				menu->AddMenuItem(soundMenuItem);
			};


			//Player inventory menu

			Menu* playerItemsMenu = new Menu("Player Items");

			AddAllItemsMenuItem(playerItemsMenu, "Get All", true);
			AddAllItemsMenuItem(playerItemsMenu, "Remove All", false);
			AddItemBoolMenuItem(playerItemsMenu, "Ankh", Spelunky::ITEM_CLASS::ANKH);
			AddItemBoolMenuItem(playerItemsMenu, "Book of the Dead", Spelunky::ITEM_CLASS::BOOK_OF_THE_DEAD);
			AddItemBoolMenuItem(playerItemsMenu, "Hedjet", Spelunky::ITEM_CLASS::HEDJET);
			AddItemBoolMenuItem(playerItemsMenu, "Udjat Eye", Spelunky::ITEM_CLASS::UDJAT_EYE);
			AddItemBoolMenuItem(playerItemsMenu, "Climbing Gloves", Spelunky::ITEM_CLASS::CLIMBING_GLOVES);
			AddItemBoolMenuItem(playerItemsMenu, "Compass", Spelunky::ITEM_CLASS::COMPASS);
			AddItemBoolMenuItem(playerItemsMenu, "Crysknife", Spelunky::ITEM_CLASS::CRYSKNIFE);
			AddItemBoolMenuItem(playerItemsMenu, "Kapala", Spelunky::ITEM_CLASS::KAPALA);
			AddItemBoolMenuItem(playerItemsMenu, "Parachute", Spelunky::ITEM_CLASS::PARACHUTE);
			AddItemBoolMenuItem(playerItemsMenu, "Paste", Spelunky::ITEM_CLASS::PASTE);
			AddItemBoolMenuItem(playerItemsMenu, "Pitcher's Mitt", Spelunky::ITEM_CLASS::PITCHERS_MITT);
			AddItemBoolMenuItem(playerItemsMenu, "Spectacles", Spelunky::ITEM_CLASS::SPECTACLES);
			AddItemBoolMenuItem(playerItemsMenu, "Spike Shoes", Spelunky::ITEM_CLASS::SPIKE_SHOES);
			AddItemBoolMenuItem(playerItemsMenu, "Spring Shoes", Spelunky::ITEM_CLASS::SPRING_SHOES);
			AddItemBoolMenuItem(playerItemsMenu, "Vlad's Amulet", Spelunky::ITEM_CLASS::VLADS_AMULET);


			//Player menu

			Menu* playerMenu = new Menu("Player");

			AddLinkMenuItem(playerMenu, "Items >", playerItemsMenu);

			IntMenuItem* goldIntMenuItem = new IntMenuItem(playerMenu, "Gold", 0, 0, (std::numeric_limits<int>::max)(), 5000);
			goldIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* goldIntMenuItem = (IntMenuItem*)menuItem;

				player.SetGold(goldIntMenuItem->value);
			};
			goldIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* goldIntMenuItem = (IntMenuItem*)menuItem;

				goldIntMenuItem->value = player.GetGold();
			};
			playerMenu->AddMenuItem(goldIntMenuItem);

			IntMenuItem* heartsIntMenuItem = new IntMenuItem(playerMenu, "Hearts", 0, 1, 99, 10);
			heartsIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* heartsIntMenuItem = (IntMenuItem*)menuItem;

				player.SetHearts(heartsIntMenuItem->value);
			};
			heartsIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* heartsIntMenuItem = (IntMenuItem*)menuItem;

				heartsIntMenuItem->value = player.GetHearts();
			};
			playerMenu->AddMenuItem(heartsIntMenuItem);

			IntMenuItem* bombsIntMenuItem = new IntMenuItem(playerMenu, "Bombs", 0, 0, 99, 10);
			bombsIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* bombsIntMenuItem = (IntMenuItem*)menuItem;

				player.SetBombs(bombsIntMenuItem->value);
			};
			bombsIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* bombsIntMenuItem = (IntMenuItem*)menuItem;

				bombsIntMenuItem->value = player.GetBombs();
			};
			playerMenu->AddMenuItem(bombsIntMenuItem);

			IntMenuItem* ropesIntMenuItem = new IntMenuItem(playerMenu, "Ropes", 0, 0, 99, 10);
			ropesIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* ropesIntMenuItem = (IntMenuItem*)menuItem;

				player.SetRopes(ropesIntMenuItem->value);
			};
			ropesIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* ropesIntMenuItem = (IntMenuItem*)menuItem;

				ropesIntMenuItem->value = player.GetRopes();
			};
			playerMenu->AddMenuItem(ropesIntMenuItem);

			IntMenuItem* favorIntMenuItem = new IntMenuItem(playerMenu, "Kali Favor", 0, 0, 100, 10);
			favorIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* favorIntMenuItem = (IntMenuItem*)menuItem;

				player.SetFavor(favorIntMenuItem->value);
			};
			favorIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* favorIntMenuItem = (IntMenuItem*)menuItem;

				favorIntMenuItem->value = player.GetFavor();
			};
			playerMenu->AddMenuItem(favorIntMenuItem);

			BoolMenuItem* infiniteHeartsBoolMenuItem = new BoolMenuItem(playerMenu, "Infinite Hearts", hasInfiniteHearts);
			infiniteHeartsBoolMenuItem->OnSelect = [](MenuItem* menuItem)
			{
				BoolMenuItem* infiniteHeartsBoolMenuItem = (BoolMenuItem*)menuItem;

				hasInfiniteHearts = infiniteHeartsBoolMenuItem->isOn;
			};
			playerMenu->AddMenuItem(infiniteHeartsBoolMenuItem);

			BoolMenuItem* infiniteBombsBoolMenuItem = new BoolMenuItem(playerMenu, "Infinite Bombs", hasInfiniteBombs);
			infiniteBombsBoolMenuItem->OnSelect = [](MenuItem* menuItem)
			{
				BoolMenuItem* infiniteBombsBoolMenuItem = (BoolMenuItem*)menuItem;

				hasInfiniteBombs = infiniteBombsBoolMenuItem->isOn;
			};
			playerMenu->AddMenuItem(infiniteBombsBoolMenuItem);

			BoolMenuItem* infiniteRopesBoolMenuItem = new BoolMenuItem(playerMenu, "Infinite Ropes", hasInfiniteRopes);
			infiniteRopesBoolMenuItem->OnSelect = [](MenuItem* menuItem)
			{
				BoolMenuItem* infiniteRopesBoolMenuItem = (BoolMenuItem*)menuItem;

				hasInfiniteRopes = infiniteRopesBoolMenuItem->isOn;
			};
			playerMenu->AddMenuItem(infiniteRopesBoolMenuItem);

			BoolMenuItem* infiniteAnkhsBoolMenuItem = new BoolMenuItem(playerMenu, "Infinite Ankhs", hasInfiniteAnkhs);
			infiniteAnkhsBoolMenuItem->OnSelect = [](MenuItem* menuItem)
			{
				BoolMenuItem* infiniteAnkhsBoolMenuItem = (BoolMenuItem*)menuItem;

				hasInfiniteAnkhs = infiniteAnkhsBoolMenuItem->isOn;
			};
			playerMenu->AddMenuItem(infiniteAnkhsBoolMenuItem);

			BoolMenuItem* infiniteParachutesBoolMenuItem = new BoolMenuItem(playerMenu, "Infinite Parachutes", hasInfiniteParachutes);
			infiniteParachutesBoolMenuItem->OnSelect = [](MenuItem* menuItem)
			{
				BoolMenuItem* infiniteParachutesBoolMenuItem = (BoolMenuItem*)menuItem;

				hasInfiniteParachutes = infiniteParachutesBoolMenuItem->isOn;
			};
			playerMenu->AddMenuItem(infiniteParachutesBoolMenuItem);


			//World objects menu

			Menu* worldObjectsMenu = new Menu("World Objects");

			AddSpawnEntityMenuItem(worldObjectsMenu, "Small Spider Web", Spelunky::ENTITY_CLASS::SMALL_SPIDER_WEB);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Spider Web", Spelunky::ENTITY_CLASS::SPIDER_WEB);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Boulder", Spelunky::ENTITY_CLASS::BOULDER);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Pushable Stone Block", Spelunky::ENTITY_CLASS::PUSHABLE_STONE_BLOCK);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Powder Box", Spelunky::ENTITY_CLASS::POWDER_BOX);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Falling Platform", Spelunky::ENTITY_CLASS::FALLING_PLATFORM);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Ice Platform", Spelunky::ENTITY_CLASS::ICE_PLATFORM);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Flying Trap Block", Spelunky::ENTITY_CLASS::FLYING_TRAP_BLOCK);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Flying Lava", Spelunky::ENTITY_CLASS::FLYING_LAVA);
			AddSpawnEntityMenuItem(worldObjectsMenu, "The Worm Entrance", Spelunky::ENTITY_CLASS::WORM_ENTRANCE);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Worm Block", Spelunky::ENTITY_CLASS::WORM_BLOCK);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Honey", Spelunky::ENTITY_CLASS::HONEY);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Spike Ball", Spelunky::ENTITY_CLASS::SPIKE_BALL);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Static Chest", Spelunky::ENTITY_CLASS::STATIC_CHEST);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Prize Wheel", Spelunky::ENTITY_CLASS::PRIZE_WHEEL);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Prize Wheel Flipper", Spelunky::ENTITY_CLASS::PRIZE_WHEEL_FLIPPER);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Prize Door", Spelunky::ENTITY_CLASS::PRIZE_DOOR);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Falling Ice", Spelunky::ENTITY_CLASS::FALLING_ICE);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Force Field Beam", Spelunky::ENTITY_CLASS::FORCE_FIELD_BEAM);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Coffin", Spelunky::ENTITY_CLASS::COFFIN);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Spaceship Platform", Spelunky::ENTITY_CLASS::SPACESHIP_PLATFORM);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Spaceship Elevator", Spelunky::ENTITY_CLASS::SPACESHIP_ELEVATOR);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Spaceship Light", Spelunky::ENTITY_CLASS::SPACESHIP_LIGHT);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Spider Web Ball", Spelunky::ENTITY_CLASS::SPIDER_WEB_BALL);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Ankh Glow", Spelunky::ENTITY_CLASS::ANKH_GLOW);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Unlit Wall Torch", Spelunky::ENTITY_CLASS::UNLIT_WALL_TORCH);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Invisable Block", Spelunky::ENTITY_CLASS::INVISABLE_BLOCK);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Tiny Fire", Spelunky::ENTITY_CLASS::TINY_FIRE);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Small Fire", Spelunky::ENTITY_CLASS::SMALL_FIRE);
			AddSpawnEntityMenuItem(worldObjectsMenu, "Torch Fire", Spelunky::ENTITY_CLASS::TORCH_FIRE);


			//AI objects menu

			Menu* aiObjectsMenu = new Menu("AI Objects");

			AddSpawnEntityMenuItem(aiObjectsMenu, "Rat", Spelunky::ENTITY_CLASS::RAT);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Spiderling", Spelunky::ENTITY_CLASS::SPIDERLING);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Baby Frog", Spelunky::ENTITY_CLASS::BABY_FROG);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Fish", Spelunky::ENTITY_CLASS::FISH);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Penguin", Spelunky::ENTITY_CLASS::PENGUIN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Locust", Spelunky::ENTITY_CLASS::LOCUST);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Maggot", Spelunky::ENTITY_CLASS::MAGGOT);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Snake", Spelunky::ENTITY_CLASS::SNAKE);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Cobra", Spelunky::ENTITY_CLASS::COBRA);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Spider", Spelunky::ENTITY_CLASS::SPIDER);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Spinner Spider", Spelunky::ENTITY_CLASS::SPINNER_SPIDER);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Bat", Spelunky::ENTITY_CLASS::BAT);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Skeleton", Spelunky::ENTITY_CLASS::SKELETON);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Monkey", Spelunky::ENTITY_CLASS::MONKEY);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Snail", Spelunky::ENTITY_CLASS::SNAIL);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Scorpion", Spelunky::ENTITY_CLASS::SCORPION);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Scorpion Fly", Spelunky::ENTITY_CLASS::SCORPION_FLY);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Mantrap", Spelunky::ENTITY_CLASS::MANTRAP);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Bee", Spelunky::ENTITY_CLASS::BEE);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Frog", Spelunky::ENTITY_CLASS::FROG);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Fire Frog", Spelunky::ENTITY_CLASS::FIRE_FROG);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Piranha", Spelunky::ENTITY_CLASS::PIRANHA);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Shopkeeper", Spelunky::ENTITY_CLASS::SHOPKEEPER);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Caveman", Spelunky::ENTITY_CLASS::CAVEMAN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Tiki Man", Spelunky::ENTITY_CLASS::TIKI_MAN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Green Knight", Spelunky::ENTITY_CLASS::GREEN_KNIGHT);
			AddSpawnEntityMenuItem(aiObjectsMenu, "The Black Knight", Spelunky::ENTITY_CLASS::THE_BLACK_KNIGHT);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Hawk Man", Spelunky::ENTITY_CLASS::HAWK_MAN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Croc Man", Spelunky::ENTITY_CLASS::CROC_MAN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Yeti", Spelunky::ENTITY_CLASS::YETI);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Giant Spider", Spelunky::ENTITY_CLASS::GIANT_SPIDER);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Queen Bee", Spelunky::ENTITY_CLASS::QUEEN_BEE);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Giant Frog", Spelunky::ENTITY_CLASS::GIANT_FROG);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Old Bitey", Spelunky::ENTITY_CLASS::OLD_BITEY);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Yeti King", Spelunky::ENTITY_CLASS::YETI_KING);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Jiang Shi", Spelunky::ENTITY_CLASS::JIAN_SHI);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Vampire", Spelunky::ENTITY_CLASS::VAMPIRE);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Vlad", Spelunky::ENTITY_CLASS::VLAD);
			AddSpawnEntityMenuItem(aiObjectsMenu, "UFO", Spelunky::ENTITY_CLASS::UFO);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Alien", Spelunky::ENTITY_CLASS::ALIEN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Alien Tank", Spelunky::ENTITY_CLASS::ALIEN_TANK);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Alien Lord", Spelunky::ENTITY_CLASS::ALIEN_LORD);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Alien Queen", Spelunky::ENTITY_CLASS::ALIEN_QUEEN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Background Alien", Spelunky::ENTITY_CLASS::BACKGROUND_ALIEN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Turret", Spelunky::ENTITY_CLASS::TURRET);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Golden Monkey", Spelunky::ENTITY_CLASS::GOLDEN_MONKEY);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Damsel", Spelunky::ENTITY_CLASS::DAMSEL);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Succubus", Spelunky::ENTITY_CLASS::SUCCUBUS);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Ghost", Spelunky::ENTITY_CLASS::GHOST);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Fly", Spelunky::ENTITY_CLASS::FLY);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Mummy", Spelunky::ENTITY_CLASS::MUMMY);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Mammoth", Spelunky::ENTITY_CLASS::MAMMOTH);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Scarab", Spelunky::ENTITY_CLASS::SCARAB);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Unknown 2", Spelunky::ENTITY_CLASS::UNKNOWN_2);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Imp", Spelunky::ENTITY_CLASS::IMP);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Devil", Spelunky::ENTITY_CLASS::DEVIL);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Bacterium", Spelunky::ENTITY_CLASS::BACTERIUM);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Worm Egg", Spelunky::ENTITY_CLASS::WORM_EGG);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Worm Baby", Spelunky::ENTITY_CLASS::WORM_BABY);
			AddSpawnEntityMenuItem(aiObjectsMenu, "The Worm Activated", Spelunky::ENTITY_CLASS::WORM_ACTIVATED);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Anubis", Spelunky::ENTITY_CLASS::ANUBIS);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Anubis II", Spelunky::ENTITY_CLASS::ANUBIS_II);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Horse Head", Spelunky::ENTITY_CLASS::HORSE_HEAD);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Ox Face", Spelunky::ENTITY_CLASS::OX_FACE);
			AddSpawnEntityMenuItem(aiObjectsMenu, "King Yama", Spelunky::ENTITY_CLASS::KING_YAMA);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Olmec", Spelunky::ENTITY_CLASS::OLMEC);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Watching Ball", Spelunky::ENTITY_CLASS::WATCHING_BALL);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Camel", Spelunky::ENTITY_CLASS::CAMEL);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Tunnel Man", Spelunky::ENTITY_CLASS::TUNNEL_MAN);
			AddSpawnEntityMenuItem(aiObjectsMenu, "Yang", Spelunky::ENTITY_CLASS::YANG);


			//Throwable objects menu

			Menu* throwableObjectsMenu = new Menu("Throwable Objects");

			AddSpawnEntityMenuItem(throwableObjectsMenu, "Live Bomb", Spelunky::ENTITY_CLASS::LIVE_BOMB);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Stone", Spelunky::ENTITY_CLASS::STONE);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Skull", Spelunky::ENTITY_CLASS::SKULL);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Pot", Spelunky::ENTITY_CLASS::POT);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Arrow", Spelunky::ENTITY_CLASS::ARROW);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Crate", Spelunky::ENTITY_CLASS::CRATE);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Pot", Spelunky::ENTITY_CLASS::POT);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Chest", Spelunky::ENTITY_CLASS::CHEST);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Gold Chest", Spelunky::ENTITY_CLASS::GOLD_CHEST);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Gold Key", Spelunky::ENTITY_CLASS::GOLD_KEY);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Lamp", Spelunky::ENTITY_CLASS::LAMP);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Flare", Spelunky::ENTITY_CLASS::FLARE);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Snowball", Spelunky::ENTITY_CLASS::SNOWBALL);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Piranha Skeleton", Spelunky::ENTITY_CLASS::FISH_SKELETON);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Imp Couldron", Spelunky::ENTITY_CLASS::IMP_COULDRON);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Paper", Spelunky::ENTITY_CLASS::PAPER);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Broken Mattock", Spelunky::ENTITY_CLASS::BROKEN_MATTOCK);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Broken Arrow", Spelunky::ENTITY_CLASS::BROKEN_ARROW);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Chain Ball", Spelunky::ENTITY_CLASS::CHAIN_BALL);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Unlit Torch", Spelunky::ENTITY_CLASS::UNLIT_TORCH);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Mystery Box", Spelunky::ENTITY_CLASS::MYSTERY_BOX);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Skull Crown", Spelunky::ENTITY_CLASS::SKULL_CROWN);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Eggplant", Spelunky::ENTITY_CLASS::EGGPLANT);
			AddSpawnEntityMenuItem(throwableObjectsMenu, "Idol", Spelunky::ENTITY_CLASS::IDOL);


			//Weapon objects menu

			Menu* weaponObjectsMenu = new Menu("Weapon Objects");

			AddSpawnEntityMenuItem(weaponObjectsMenu, "Mattock", Spelunky::ENTITY_CLASS::MATTOCK);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Boomerang", Spelunky::ENTITY_CLASS::BOOMERANG);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Machete", Spelunky::ENTITY_CLASS::MACHETE);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Crysknife", Spelunky::ENTITY_CLASS::CRYSKNIFE);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Web Gun", Spelunky::ENTITY_CLASS::WEB_GUN);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Shotgun", Spelunky::ENTITY_CLASS::SHOTGUN);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Freeze Ray", Spelunky::ENTITY_CLASS::FREEZE_RAY);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Plasma Cannon", Spelunky::ENTITY_CLASS::PLASMA_CANNON);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Scepter", Spelunky::ENTITY_CLASS::SCEPTER);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Camera", Spelunky::ENTITY_CLASS::CAMERA);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Teleporter", Spelunky::ENTITY_CLASS::TELEPORTER);
			AddSpawnEntityMenuItem(weaponObjectsMenu, "Shield", Spelunky::ENTITY_CLASS::SHIELD);


			//Accessory objects menu

			Menu* accessoryObjectsMenu = new Menu("Accessory Objects");

			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Black Box Accessory", Spelunky::ENTITY_CLASS::BLACK_BOX_ACCESSORY);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Ropes", Spelunky::ENTITY_CLASS::ROPES);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Bomb Bag", Spelunky::ENTITY_CLASS::BOMB_BAG);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Bomb Box", Spelunky::ENTITY_CLASS::BOMB_BOX);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Bomb Paste", Spelunky::ENTITY_CLASS::BOMB_PASTE);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Spectacles", Spelunky::ENTITY_CLASS::SPECTACLE);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Climbing Gloves", Spelunky::ENTITY_CLASS::CLIMBING_GLOVES);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Pitcher's Mitt", Spelunky::ENTITY_CLASS::PITCHERS_MITT);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Spring Shoes", Spelunky::ENTITY_CLASS::SPRING_SHOES);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Spike Shoes", Spelunky::ENTITY_CLASS::SPIKE_SHOES);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Compass", Spelunky::ENTITY_CLASS::COMPASS);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Crysknife", Spelunky::ENTITY_CLASS::CRYSKNIFE);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Parachute", Spelunky::ENTITY_CLASS::PARACHUTE);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Cape", Spelunky::ENTITY_CLASS::CAPE);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Jetpack", Spelunky::ENTITY_CLASS::JETPACK);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Royal Jelly", Spelunky::ENTITY_CLASS::ROYAL_JELLY);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Kapala", Spelunky::ENTITY_CLASS::KAPALA);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Udjat Eye", Spelunky::ENTITY_CLASS::UDJAT_EYE);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Ankh", Spelunky::ENTITY_CLASS::ANKH);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Hedjet", Spelunky::ENTITY_CLASS::HEDJET);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Book of the Dead", Spelunky::ENTITY_CLASS::BOOK_OF_THE_DEAD);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Vlad's Cape", Spelunky::ENTITY_CLASS::VLADS_CAPE);
			AddSpawnEntityMenuItem(accessoryObjectsMenu, "Vlad's Amulet", Spelunky::ENTITY_CLASS::VLADS_AMULET);


			//Gold objects menu

			Menu* goldObjectsMenu = new Menu("Gold Objects");

			AddSpawnEntityMenuItem(goldObjectsMenu, "Coin", Spelunky::ENTITY_CLASS::COIN);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Small Gold Nugget", Spelunky::ENTITY_CLASS::SMALL_GOLD_NUGGET);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Large Gold Nugget", Spelunky::ENTITY_CLASS::LARGE_GOLD_NUGGET);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Gold Bar", Spelunky::ENTITY_CLASS::GOLD_BAR);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Stack of Gold Bars", Spelunky::ENTITY_CLASS::STACK_OF_GOLD_BARS);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Small Emerald", Spelunky::ENTITY_CLASS::SMALL_EMERALD);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Large Emerald", Spelunky::ENTITY_CLASS::LARGE_EMERALD);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Small Sapphire", Spelunky::ENTITY_CLASS::SMALL_SAPPHIRE);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Large Sapphire", Spelunky::ENTITY_CLASS::LARGE_SAPPHIRE);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Small Ruby", Spelunky::ENTITY_CLASS::SMALL_RUBY);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Large Ruby", Spelunky::ENTITY_CLASS::LARGE_RUBY);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Natural Diamond", Spelunky::ENTITY_CLASS::NATURAL_DIAMOND);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Skull Crown", Spelunky::ENTITY_CLASS::SKULL_CROWN);
			AddSpawnEntityMenuItem(goldObjectsMenu, "Scarab", Spelunky::ENTITY_CLASS::SCARAB);


			//Player objects menu

			Menu* playerObjectsMenu = new Menu("Player Objects");

			AddSpawnEntityMenuItem(playerObjectsMenu, "Live Bomb", Spelunky::ENTITY_CLASS::LIVE_BOMB);
			AddSpawnEntityMenuItem(playerObjectsMenu, "Attached Rope", Spelunky::ENTITY_CLASS::ATTACHED_ROPE);
			AddSpawnEntityMenuItem(playerObjectsMenu, "Used Parachute", Spelunky::ENTITY_CLASS::USED_PARACHUTE);
			AddSpawnEntityMenuItem(playerObjectsMenu, "White Flag", Spelunky::ENTITY_CLASS::WHITE_FLAG);
			AddSpawnEntityMenuItem(playerObjectsMenu, "Chain Ball", Spelunky::ENTITY_CLASS::CHAIN_BALL);


			//Container objects menu

			Menu* containerObjectsMenu = new Menu("Container Objects");

			AddSpawnEntityMenuItem(containerObjectsMenu, "Crate", Spelunky::ENTITY_CLASS::CRATE);
			AddSpawnEntityMenuItem(containerObjectsMenu, "Pot", Spelunky::ENTITY_CLASS::POT);
			AddSpawnEntityMenuItem(containerObjectsMenu, "Chest", Spelunky::ENTITY_CLASS::CHEST);
			AddSpawnEntityMenuItem(containerObjectsMenu, "Gold Chest", Spelunky::ENTITY_CLASS::GOLD_CHEST);
			AddSpawnEntityMenuItem(containerObjectsMenu, "Mystery Box", Spelunky::ENTITY_CLASS::MYSTERY_BOX);


			//Effect objects menu

			Menu* effectObjectsMenu = new Menu("Effect Objects");

			AddSpawnEntityMenuItem(effectObjectsMenu, "Blood", Spelunky::ENTITY_CLASS::BLOOD);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Leaf", Spelunky::ENTITY_CLASS::LEAF);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Dirt Break", Spelunky::ENTITY_CLASS::DIRT_BREAK);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Metal Break", Spelunky::ENTITY_CLASS::METAL_BREAK);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Ice Break", Spelunky::ENTITY_CLASS::ICE_BREAK);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Smoke 1", Spelunky::ENTITY_CLASS::SMOKE_1);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Smoke 2", Spelunky::ENTITY_CLASS::SMOKE_2);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Unknown 1", Spelunky::ENTITY_CLASS::UNKNOWN_1);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Flying Lava", Spelunky::ENTITY_CLASS::FLYING_LAVA);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Falling Water", Spelunky::ENTITY_CLASS::FALLING_WATER);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Bee Parts", Spelunky::ENTITY_CLASS::BEE_PARTS);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Fire", Spelunky::ENTITY_CLASS::FIRE);
			AddSpawnEntityMenuItem(effectObjectsMenu, "String", Spelunky::ENTITY_CLASS::STRING);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Firework", Spelunky::ENTITY_CLASS::FIREWORK);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Alien Queen Ring", Spelunky::ENTITY_CLASS::ALIEN_QUEEN_RING);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Explosion", Spelunky::ENTITY_CLASS::EXPLOSION);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Red Ball", Spelunky::ENTITY_CLASS::RED_BALL);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Spring Rings", Spelunky::ENTITY_CLASS::SPRING_RINGS);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Spelunker Teleport", Spelunky::ENTITY_CLASS::SPELUNKER_TELEPORT);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Tiny Fire", Spelunky::ENTITY_CLASS::TINY_FIRE);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Small Fire", Spelunky::ENTITY_CLASS::SMALL_FIRE);
			AddSpawnEntityMenuItem(effectObjectsMenu, "Torch Fire", Spelunky::ENTITY_CLASS::TORCH_FIRE);


			//Lighting objets menu

			Menu* lightingObjectsMenu = new Menu("Lighting Objects");

			AddSpawnEntityMenuItem(lightingObjectsMenu, "Lamp", Spelunky::ENTITY_CLASS::LAMP);
			AddSpawnEntityMenuItem(lightingObjectsMenu, "Flare", Spelunky::ENTITY_CLASS::FLARE);
			AddSpawnEntityMenuItem(lightingObjectsMenu, "Flying Lava", Spelunky::ENTITY_CLASS::FLYING_LAVA);
			AddSpawnEntityMenuItem(lightingObjectsMenu, "Bright Light", Spelunky::ENTITY_CLASS::BRIGHT_LIGHT);
			AddSpawnEntityMenuItem(lightingObjectsMenu, "Spaceship Light", Spelunky::ENTITY_CLASS::SPACESHIP_LIGHT);
			AddSpawnEntityMenuItem(lightingObjectsMenu, "Ankh Glow", Spelunky::ENTITY_CLASS::ANKH_GLOW);
			AddSpawnEntityMenuItem(lightingObjectsMenu, "Unlit Wall Torch", Spelunky::ENTITY_CLASS::UNLIT_WALL_TORCH);
			AddSpawnEntityMenuItem(lightingObjectsMenu, "Unlit Torch", Spelunky::ENTITY_CLASS::UNLIT_TORCH);


			//Bullet objects menu

			Menu* bulletObjectsMenu = new Menu("Bullet Objects");

			AddSpawnEntityMenuItem(bulletObjectsMenu, "Shotgun Shot", Spelunky::ENTITY_CLASS::SHOTGUN_SHOT);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Spider Web Shot", Spelunky::ENTITY_CLASS::SPIDER_WEB_SHOT);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Scratch", Spelunky::ENTITY_CLASS::SCRATCH);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Alien Shot", Spelunky::ENTITY_CLASS::ALIEN_SHOT);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "UFO Shot", Spelunky::ENTITY_CLASS::UFO_SHOT);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Acid Bobble", Spelunky::ENTITY_CLASS::ACID_BOBBLE);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Acid Drop", Spelunky::ENTITY_CLASS::ACID_DROP);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Ice Energy Shot", Spelunky::ENTITY_CLASS::ICE_ENERGY_SHOT);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Turret Shot", Spelunky::ENTITY_CLASS::TURRET_SHOT);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Olmec Orb", Spelunky::ENTITY_CLASS::OLMEC_ORB);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Alien Target", Spelunky::ENTITY_CLASS::ALIEN_TARGET);
			AddSpawnEntityMenuItem(bulletObjectsMenu, "Alien Target Shot", Spelunky::ENTITY_CLASS::ALIEN_TARGET_SHOT);


			//Broken objects menu

			Menu* brokenObjectsMenu = new Menu("Broken Objects");

			AddSpawnEntityMenuItem(brokenObjectsMenu, "Spritesheet 1", Spelunky::ENTITY_CLASS::SPRITESHEET_1);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Spritesheet 2", Spelunky::ENTITY_CLASS::SPRITESHEET_2);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Spritesheet 3", Spelunky::ENTITY_CLASS::SPRITESHEET_3);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Olmec", Spelunky::ENTITY_CLASS::OLMEC);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Balloon", Spelunky::ENTITY_CLASS::BALLOON);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Black Box", Spelunky::ENTITY_CLASS::BLACK_BOX);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Win Game", Spelunky::ENTITY_CLASS::WIN_GAME);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Coffin", Spelunky::ENTITY_CLASS::COFFIN);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Unknown 1", Spelunky::ENTITY_CLASS::UNKNOWN_1);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Black Box Pickup", Spelunky::ENTITY_CLASS::BLACK_BOX_PICKUP);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Ankh Respawn", Spelunky::ENTITY_CLASS::ANKH_RESPAWN);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Black Box Rope", Spelunky::ENTITY_CLASS::BLACK_BOX_ROPE);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Mystery Box", Spelunky::ENTITY_CLASS::MYSTERY_BOX);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "King Yama's Hand", Spelunky::ENTITY_CLASS::KING_YAMAS_HAND);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Force Field (crash)", Spelunky::ENTITY_CLASS::FORCE_FIELD);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Journal (crash)", Spelunky::ENTITY_CLASS::JOURNAL);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Olmec Intro (crash)", Spelunky::ENTITY_CLASS::OLMEC_INTRO);
			AddSpawnEntityMenuItem(brokenObjectsMenu, "Glass Block (crash)", Spelunky::ENTITY_CLASS::GLASS_BLOCK);


			//All objects menu

			Menu* allObjectsMenu = new Menu("All Objects (Unstable)");

			for(int i = 100; i < 1100; i++)
			{
				AddSpawnEntityMenuItem(allObjectsMenu, "Object", (Spelunky::ENTITY_CLASS)i);
			}


			//Object spawner menu

			Menu* objectSpawnerMenu = new Menu("Object Spawner");

			BoolMenuItem* cursorControlBoolMenuItem = new BoolMenuItem(objectSpawnerMenu, "Mouse Control", *isCursorControlled);
			cursorControlBoolMenuItem->OnSelect = [isCursorControlled](MenuItem* menuItem)
			{
				BoolMenuItem* cursorControlBoolMenuItem = (BoolMenuItem*)menuItem;

				*isCursorControlled = cursorControlBoolMenuItem->isOn;
			};
			objectSpawnerMenu->AddMenuItem(cursorControlBoolMenuItem);

			BoolMenuItem* useCursorVelocityBoolMenuItem = new BoolMenuItem(objectSpawnerMenu, "Use Mouse Velocity", *useCursorVelocity);
			useCursorVelocityBoolMenuItem->OnSelect = [useCursorVelocity](MenuItem* menuItem)
			{
				BoolMenuItem* useCursorVelocityBoolMenuItem = (BoolMenuItem*)menuItem;

				*useCursorVelocity = useCursorVelocityBoolMenuItem->isOn;
			};
			objectSpawnerMenu->AddMenuItem(useCursorVelocityBoolMenuItem);

			AddLinkMenuItem(objectSpawnerMenu, "World Objects >", worldObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "AI Objects >", aiObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Throwable Objects >", throwableObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Weapon Objects >", weaponObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Accessory Objects >", accessoryObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Gold Objects >", goldObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Player Objects >", playerObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Container Objects >", containerObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Effect Objects >", effectObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Lighting Objects >", lightingObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Bullet Objects >", bulletObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "Broken Objects >", brokenObjectsMenu);
			AddLinkMenuItem(objectSpawnerMenu, "All Objects (unstable) >", allObjectsMenu);


			//Teleporting menu

			Menu* teleportingMenu = new Menu("Teleporting");

			IntMenuItem* moveDistanceMenuItem = new IntMenuItem(teleportingMenu, "Move Distance", (int)*moveDistance, 1, 20, 5);
			moveDistanceMenuItem->OnValueChanged = [moveDistance](MenuItem* menuItem)
			{
				IntMenuItem* moveDistanceMenuItem = (IntMenuItem*)menuItem;

				*moveDistance = (float)moveDistanceMenuItem->value;
			};
			teleportingMenu->AddMenuItem(moveDistanceMenuItem);

			AddRelativeMoveMenuItem(teleportingMenu, "Move Right", Vector2(1, 0));
			AddRelativeMoveMenuItem(teleportingMenu, "Move Left", Vector2(-1, 0));
			AddRelativeMoveMenuItem(teleportingMenu, "Move Up", Vector2(0, 1));
			AddRelativeMoveMenuItem(teleportingMenu, "Move Down", Vector2(0, -1));

			MenuItem* teleportToCursorMenuItem = new MenuItem(teleportingMenu, "Teleport To Mouse");
			teleportToCursorMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				if(menuItem->IsInFocus() && Input::IsKeyPressed(VK_LBUTTON, Input::INPUT_MODE::CONTINUOUS))
				{
					menuItem->Select();
				}
			};
			teleportToCursorMenuItem->OnSelect = [](MenuItem* menuItem)
			{
				player.Teleport(Spelunky::ScreenToWorld(Window::GetRelativeCursorPosition()));
			};
			teleportingMenu->AddMenuItem(teleportToCursorMenuItem);


			//Time menu

			Menu* timeMenu = new Menu("Time");

			BoolMenuItem* freezeTotalTimeBoolMenuItem = new BoolMenuItem(timeMenu, "Freeze Total Time", isTotalTimeFrozen);
			freezeTotalTimeBoolMenuItem->OnSelect = [](MenuItem* menuItem)
			{
				BoolMenuItem* freezeTotalTimeBoolMenuItem = (BoolMenuItem*)menuItem;

				isTotalTimeFrozen = freezeTotalTimeBoolMenuItem->isOn;
				frozenTotalMinutes = Spelunky::GetTotalMinutes();
				frozenTotalSeconds = Spelunky::GetTotalSeconds();
			};
			timeMenu->AddMenuItem(freezeTotalTimeBoolMenuItem);

			BoolMenuItem* freezeStageTimeBoolMenuItem = new BoolMenuItem(timeMenu, "Freeze Stage Time", isStageTimeFrozen);
			freezeStageTimeBoolMenuItem->OnSelect = [](MenuItem* menuItem)
			{
				BoolMenuItem* freezeStageTimeBoolMenuItem = (BoolMenuItem*)menuItem;

				isStageTimeFrozen = freezeStageTimeBoolMenuItem->isOn;
				frozenStageMinutes = Spelunky::GetStageMinutes();
				frozenStageSeconds = Spelunky::GetStageSeconds();
			};
			timeMenu->AddMenuItem(freezeStageTimeBoolMenuItem);

			IntMenuItem* totalMinutesIntMenuItem = new IntMenuItem(timeMenu, "Total Minutes", 0, 0, 99, 10);
			totalMinutesIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* totalMinutesIntMenuItem = (IntMenuItem*)menuItem;

				Spelunky::SetTotalMinutes(totalMinutesIntMenuItem->value);

				if(isTotalTimeFrozen)
				{
					frozenTotalMinutes = totalMinutesIntMenuItem->value;
				}
			};
			totalMinutesIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* totalMinutesIntMenuItem = (IntMenuItem*)menuItem;

				totalMinutesIntMenuItem->value = Spelunky::GetTotalMinutes();
			};
			timeMenu->AddMenuItem(totalMinutesIntMenuItem);

			IntMenuItem* totalSecondsIntMenuItem = new IntMenuItem(timeMenu, "Total Seconds", 0, 0, 59, 5);
			totalSecondsIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* totalSecondsIntMenuItem = (IntMenuItem*)menuItem;

				Spelunky::SetTotalSeconds(totalSecondsIntMenuItem->value);

				if(isTotalTimeFrozen)
				{
					frozenTotalSeconds = totalSecondsIntMenuItem->value;
				}
			};
			totalSecondsIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* totalSecondsIntMenuItem = (IntMenuItem*)menuItem;

				totalSecondsIntMenuItem->value = Spelunky::GetTotalSeconds();
			};
			timeMenu->AddMenuItem(totalSecondsIntMenuItem);

			IntMenuItem* stageMinutesIntMenuItem = new IntMenuItem(timeMenu, "Stage Minutes", 0, 0, 99, 10);
			stageMinutesIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* stageMinutesIntMenuItem = (IntMenuItem*)menuItem;

				Spelunky::SetStageMinutes(stageMinutesIntMenuItem->value);

				if(isStageTimeFrozen)
				{
					frozenStageMinutes = stageMinutesIntMenuItem->value;
				}
			};
			stageMinutesIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* stageMinutesIntMenuItem = (IntMenuItem*)menuItem;

				stageMinutesIntMenuItem->value = Spelunky::GetStageMinutes();
			};
			timeMenu->AddMenuItem(stageMinutesIntMenuItem);

			IntMenuItem* stageSecondsIntMenuItem = new IntMenuItem(timeMenu, "Stage Seconds", 0, 0, 59, 5);
			stageSecondsIntMenuItem->OnValueChanged = [](MenuItem* menuItem)
			{
				IntMenuItem* stageSecondsIntMenuItem = (IntMenuItem*)menuItem;

				Spelunky::SetStageSeconds(stageSecondsIntMenuItem->value);

				if(isStageTimeFrozen)
				{
					frozenStageSeconds = stageSecondsIntMenuItem->value;
				}
			};
			stageSecondsIntMenuItem->OnUpdate = [](MenuItem* menuItem)
			{
				IntMenuItem* stageSecondsIntMenuItem = (IntMenuItem*)menuItem;

				stageSecondsIntMenuItem->value = Spelunky::GetStageSeconds();
			};
			timeMenu->AddMenuItem(stageSecondsIntMenuItem);


			//Sound player menu

			Menu* soundPlayerMenu = new Menu("Sound Player");

			AddSoundMenuItem(soundPlayerMenu, "alien_jump.wav");
			AddSoundMenuItem(soundPlayerMenu, "alienexplosion.wav");
			AddSoundMenuItem(soundPlayerMenu, "angry_kali.wav");
			AddSoundMenuItem(soundPlayerMenu, "ani_squeak.wav");
			AddSoundMenuItem(soundPlayerMenu, "ankhbreak.wav");
			AddSoundMenuItem(soundPlayerMenu, "ankhflash.wav");
			AddSoundMenuItem(soundPlayerMenu, "ankhflashback.wav");
			AddSoundMenuItem(soundPlayerMenu, "anubisII_appear.wav");
			AddSoundMenuItem(soundPlayerMenu, "anubisII_summons.wav");
			AddSoundMenuItem(soundPlayerMenu, "armor_break.wav");
			AddSoundMenuItem(soundPlayerMenu, "arrowhitwall.wav");
			AddSoundMenuItem(soundPlayerMenu, "arrowshot.wav");
			AddSoundMenuItem(soundPlayerMenu, "batflap.wav");
			AddSoundMenuItem(soundPlayerMenu, "batoneflap.wav");
			AddSoundMenuItem(soundPlayerMenu, "bee.wav");
			AddSoundMenuItem(soundPlayerMenu, "bee2.wav");
			AddSoundMenuItem(soundPlayerMenu, "bee3.wav");
			AddSoundMenuItem(soundPlayerMenu, "blockfall.wav");
			AddSoundMenuItem(soundPlayerMenu, "blocksmash.wav");
			AddSoundMenuItem(soundPlayerMenu, "bomb_glue.wav");
			AddSoundMenuItem(soundPlayerMenu, "bomb_timer.wav");
			AddSoundMenuItem(soundPlayerMenu, "bone_shatter.wav");
			AddSoundMenuItem(soundPlayerMenu, "boomerang_loop.wav");
			AddSoundMenuItem(soundPlayerMenu, "bouldercoming.wav");
			AddSoundMenuItem(soundPlayerMenu, "boulderhit.wav");
			AddSoundMenuItem(soundPlayerMenu, "boulderhit2.wav");
			AddSoundMenuItem(soundPlayerMenu, "boulderhit3.wav");
			AddSoundMenuItem(soundPlayerMenu, "boulderhit4.wav");
			AddSoundMenuItem(soundPlayerMenu, "bounce.wav");
			AddSoundMenuItem(soundPlayerMenu, "bounce_light.wav");
			AddSoundMenuItem(soundPlayerMenu, "camera.wav");
			AddSoundMenuItem(soundPlayerMenu, "cape.wav");
			AddSoundMenuItem(soundPlayerMenu, "catch_boomerang.wav");
			AddSoundMenuItem(soundPlayerMenu, "chaching.wav");
			AddSoundMenuItem(soundPlayerMenu, "char_unlock.wav");
			AddSoundMenuItem(soundPlayerMenu, "chestopen.wav");
			AddSoundMenuItem(soundPlayerMenu, "chime.wav");
			AddSoundMenuItem(soundPlayerMenu, "chime3.wav");
			AddSoundMenuItem(soundPlayerMenu, "chimp_attack.wav");
			AddSoundMenuItem(soundPlayerMenu, "chimp_bounce.wav");
			AddSoundMenuItem(soundPlayerMenu, "chute.wav");
			AddSoundMenuItem(soundPlayerMenu, "coinsdrop.wav");
			AddSoundMenuItem(soundPlayerMenu, "collect.wav");
			AddSoundMenuItem(soundPlayerMenu, "crateopen.wav");
			AddSoundMenuItem(soundPlayerMenu, "cricket.wav");
			AddSoundMenuItem(soundPlayerMenu, "crushhit.wav");
			AddSoundMenuItem(soundPlayerMenu, "crysknife.wav");
			AddSoundMenuItem(soundPlayerMenu, "damsel_dog.wav");
			AddSoundMenuItem(soundPlayerMenu, "damsel_female.wav");
			AddSoundMenuItem(soundPlayerMenu, "damsel_male.wav");
			AddSoundMenuItem(soundPlayerMenu, "damsel_sloth.wav");
			AddSoundMenuItem(soundPlayerMenu, "damsel_water.wav");
			AddSoundMenuItem(soundPlayerMenu, "demon.wav");
			AddSoundMenuItem(soundPlayerMenu, "deposit.wav");
			AddSoundMenuItem(soundPlayerMenu, "dm_go.wav");
			AddSoundMenuItem(soundPlayerMenu, "dm_jump.wav");
			AddSoundMenuItem(soundPlayerMenu, "dm_point.wav");
			AddSoundMenuItem(soundPlayerMenu, "dm_ready.wav");
			AddSoundMenuItem(soundPlayerMenu, "dm_winner.wav");
			AddSoundMenuItem(soundPlayerMenu, "doorcrack.wav");
			AddSoundMenuItem(soundPlayerMenu, "doorcrack2.wav");
			AddSoundMenuItem(soundPlayerMenu, "doorglow.wav");
			AddSoundMenuItem(soundPlayerMenu, "down.wav");
			AddSoundMenuItem(soundPlayerMenu, "eggplant.wav");
			AddSoundMenuItem(soundPlayerMenu, "end_chest.wav");
			AddSoundMenuItem(soundPlayerMenu, "eruption.wav");
			AddSoundMenuItem(soundPlayerMenu, "eyeblink.wav");
			AddSoundMenuItem(soundPlayerMenu, "fadein.wav");
			AddSoundMenuItem(soundPlayerMenu, "fadeout.wav");
			AddSoundMenuItem(soundPlayerMenu, "fly_loop.wav");
			AddSoundMenuItem(soundPlayerMenu, "forcefield.wav");
			AddSoundMenuItem(soundPlayerMenu, "freezeray.wav");
			AddSoundMenuItem(soundPlayerMenu, "frog1.wav");
			AddSoundMenuItem(soundPlayerMenu, "frog2.wav");
			AddSoundMenuItem(soundPlayerMenu, "frog3.wav");
			AddSoundMenuItem(soundPlayerMenu, "frog_big_land.wav");
			AddSoundMenuItem(soundPlayerMenu, "frog_bomb_charge.wav");
			AddSoundMenuItem(soundPlayerMenu, "frog_mini.wav");
			AddSoundMenuItem(soundPlayerMenu, "frozen.wav");
			AddSoundMenuItem(soundPlayerMenu, "fwboom.wav");
			AddSoundMenuItem(soundPlayerMenu, "fwshot.wav");
			AddSoundMenuItem(soundPlayerMenu, "gem1.wav");
			AddSoundMenuItem(soundPlayerMenu, "gem2.wav");
			AddSoundMenuItem(soundPlayerMenu, "gem3.wav");
			AddSoundMenuItem(soundPlayerMenu, "gem4.wav");
			AddSoundMenuItem(soundPlayerMenu, "gem5.wav");
			AddSoundMenuItem(soundPlayerMenu, "ghostloop.wav");
			AddSoundMenuItem(soundPlayerMenu, "gold_poop.wav");
			AddSoundMenuItem(soundPlayerMenu, "grab.wav");
			AddSoundMenuItem(soundPlayerMenu, "grunt01.wav");
			AddSoundMenuItem(soundPlayerMenu, "grunt02.wav");
			AddSoundMenuItem(soundPlayerMenu, "grunt03.wav");
			AddSoundMenuItem(soundPlayerMenu, "grunt04.wav");
			AddSoundMenuItem(soundPlayerMenu, "grunt05.wav");
			AddSoundMenuItem(soundPlayerMenu, "grunt06.wav");
			AddSoundMenuItem(soundPlayerMenu, "heartbeat.wav");
			AddSoundMenuItem(soundPlayerMenu, "hit.wav");
			AddSoundMenuItem(soundPlayerMenu, "homing1.wav");
			AddSoundMenuItem(soundPlayerMenu, "homing2.wav");
			AddSoundMenuItem(soundPlayerMenu, "horsehead.wav");
			AddSoundMenuItem(soundPlayerMenu, "ice_crack1.wav");
			AddSoundMenuItem(soundPlayerMenu, "ice_crack2.wav");
			AddSoundMenuItem(soundPlayerMenu, "ice_crack3.wav");
			AddSoundMenuItem(soundPlayerMenu, "idol_get6.wav");
			AddSoundMenuItem(soundPlayerMenu, "ihear_water.wav");
			AddSoundMenuItem(soundPlayerMenu, "immortal_bounce.wav");
			AddSoundMenuItem(soundPlayerMenu, "imp_flap.wav");
			AddSoundMenuItem(soundPlayerMenu, "intodoor.wav");
			AddSoundMenuItem(soundPlayerMenu, "item_drop.wav");
			AddSoundMenuItem(soundPlayerMenu, "itemsplash.wav");
			AddSoundMenuItem(soundPlayerMenu, "jelly_get.wav");
			AddSoundMenuItem(soundPlayerMenu, "jetpack_loop.wav");
			AddSoundMenuItem(soundPlayerMenu, "jump.wav");
			AddSoundMenuItem(soundPlayerMenu, "kaboom.wav");
			AddSoundMenuItem(soundPlayerMenu, "kaboombass.wav");
			AddSoundMenuItem(soundPlayerMenu, "kiss.wav");
			AddSoundMenuItem(soundPlayerMenu, "knifeattack.wav");
			AddSoundMenuItem(soundPlayerMenu, "knifepickup.wav");
			AddSoundMenuItem(soundPlayerMenu, "land.wav");
			AddSoundMenuItem(soundPlayerMenu, "lasergun.wav");
			AddSoundMenuItem(soundPlayerMenu, "lava_splash.wav");
			AddSoundMenuItem(soundPlayerMenu, "lavastream.wav");
			AddSoundMenuItem(soundPlayerMenu, "lick.wav");
			AddSoundMenuItem(soundPlayerMenu, "lobbydrum.wav");
			AddSoundMenuItem(soundPlayerMenu, "maindooropen.wav");
			AddSoundMenuItem(soundPlayerMenu, "mantrapbite.wav");
			AddSoundMenuItem(soundPlayerMenu, "match.wav");
			AddSoundMenuItem(soundPlayerMenu, "menu_enter.wav");
			AddSoundMenuItem(soundPlayerMenu, "menu_hor_l.wav");
			AddSoundMenuItem(soundPlayerMenu, "menu_hor_r.wav");
			AddSoundMenuItem(soundPlayerMenu, "menu_ret.wav");
			AddSoundMenuItem(soundPlayerMenu, "menu_selection.wav");
			AddSoundMenuItem(soundPlayerMenu, "menu_selection2.wav");
			AddSoundMenuItem(soundPlayerMenu, "menu_swipe.wav");
			AddSoundMenuItem(soundPlayerMenu, "menu_ver.wav");
			AddSoundMenuItem(soundPlayerMenu, "metal_clank.wav");
			AddSoundMenuItem(soundPlayerMenu, "mine_timer.wav");
			AddSoundMenuItem(soundPlayerMenu, "mm_amb.wav");
			AddSoundMenuItem(soundPlayerMenu, "mm_click.wav");
			AddSoundMenuItem(soundPlayerMenu, "mm_door1.wav");
			AddSoundMenuItem(soundPlayerMenu, "mm_door2.wav");
			AddSoundMenuItem(soundPlayerMenu, "mm_door3.wav");
			AddSoundMenuItem(soundPlayerMenu, "monkey_stealing.wav");
			AddSoundMenuItem(soundPlayerMenu, "msgup.wav");
			AddSoundMenuItem(soundPlayerMenu, "newshatter.wav");
			AddSoundMenuItem(soundPlayerMenu, "oxface.wav");
			AddSoundMenuItem(soundPlayerMenu, "pageget.wav");
			AddSoundMenuItem(soundPlayerMenu, "pageturn.wav");
			AddSoundMenuItem(soundPlayerMenu, "pause_in.wav");
			AddSoundMenuItem(soundPlayerMenu, "pause_out.wav");
			AddSoundMenuItem(soundPlayerMenu, "penguin.wav");
			AddSoundMenuItem(soundPlayerMenu, "pickup.wav");
			AddSoundMenuItem(soundPlayerMenu, "pushblock.wav");
			AddSoundMenuItem(soundPlayerMenu, "queenblast.wav");
			AddSoundMenuItem(soundPlayerMenu, "ropecatch.wav");
			AddSoundMenuItem(soundPlayerMenu, "ropetoss.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble2.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble3.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_bone1.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_bone2.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_bone3.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_ice1.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_ice2.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_ice3.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_metal1.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_metal2.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_metal3.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_vase1.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_vase2.wav");
			AddSoundMenuItem(soundPlayerMenu, "rubble_vase3.wav");
			AddSoundMenuItem(soundPlayerMenu, "sacrifice.wav");
			AddSoundMenuItem(soundPlayerMenu, "scarab_get.wav");
			AddSoundMenuItem(soundPlayerMenu, "scorpion.wav");
			AddSoundMenuItem(soundPlayerMenu, "scrollhit.wav");
			AddSoundMenuItem(soundPlayerMenu, "secret.wav");
			AddSoundMenuItem(soundPlayerMenu, "shatter.wav");
			AddSoundMenuItem(soundPlayerMenu, "shop_bells.wav");
			AddSoundMenuItem(soundPlayerMenu, "shopwheel.wav");
			AddSoundMenuItem(soundPlayerMenu, "shotgun.wav");
			AddSoundMenuItem(soundPlayerMenu, "shotgunpump.wav");
			AddSoundMenuItem(soundPlayerMenu, "skeleton_arise.wav");
			AddSoundMenuItem(soundPlayerMenu, "snail_bubble.wav");
			AddSoundMenuItem(soundPlayerMenu, "snail_bubble_burst.wav");
			AddSoundMenuItem(soundPlayerMenu, "snakebite.wav");
			AddSoundMenuItem(soundPlayerMenu, "snowball.wav");
			AddSoundMenuItem(soundPlayerMenu, "spider_jump.wav");
			AddSoundMenuItem(soundPlayerMenu, "spidershot.wav");
			AddSoundMenuItem(soundPlayerMenu, "spike_hit.wav");
			AddSoundMenuItem(soundPlayerMenu, "splash.wav");
			AddSoundMenuItem(soundPlayerMenu, "splat.wav");
			AddSoundMenuItem(soundPlayerMenu, "spring.wav");
			AddSoundMenuItem(soundPlayerMenu, "squish.wav");
			AddSoundMenuItem(soundPlayerMenu, "sr_frogburp.wav");
			AddSoundMenuItem(soundPlayerMenu, "succubus.wav");
			AddSoundMenuItem(soundPlayerMenu, "talkbutton.wav");
			AddSoundMenuItem(soundPlayerMenu, "tank.wav");
			AddSoundMenuItem(soundPlayerMenu, "teleport.wav");
			AddSoundMenuItem(soundPlayerMenu, "throw_item.wav");
			AddSoundMenuItem(soundPlayerMenu, "tikifire.wav");
			AddSoundMenuItem(soundPlayerMenu, "tikispike.wav");
			AddSoundMenuItem(soundPlayerMenu, "torchgust.wav");
			AddSoundMenuItem(soundPlayerMenu, "torchlight.wav");
			AddSoundMenuItem(soundPlayerMenu, "torchlightshort.wav");
			AddSoundMenuItem(soundPlayerMenu, "turretlaser.wav");
			AddSoundMenuItem(soundPlayerMenu, "ufo_loop.wav");
			AddSoundMenuItem(soundPlayerMenu, "ufo_shot.wav");
			AddSoundMenuItem(soundPlayerMenu, "uhoh.wav");
			AddSoundMenuItem(soundPlayerMenu, "up.wav");
			AddSoundMenuItem(soundPlayerMenu, "vanish.wav");
			AddSoundMenuItem(soundPlayerMenu, "volcanoshot.wav");
			AddSoundMenuItem(soundPlayerMenu, "vomitflies.wav");
			AddSoundMenuItem(soundPlayerMenu, "vsnake_sizzle.wav");
			AddSoundMenuItem(soundPlayerMenu, "waterstream.wav");
			AddSoundMenuItem(soundPlayerMenu, "webshot.wav");
			AddSoundMenuItem(soundPlayerMenu, "whip.wav");
			AddSoundMenuItem(soundPlayerMenu, "worm_block_destroy.wav");
			AddSoundMenuItem(soundPlayerMenu, "worm_block_regen.wav");
			AddSoundMenuItem(soundPlayerMenu, "worm_contact.wav");
			AddSoundMenuItem(soundPlayerMenu, "worm_contact2.wav");
			AddSoundMenuItem(soundPlayerMenu, "worm_contact3.wav");
			AddSoundMenuItem(soundPlayerMenu, "worm_eats.wav");
			AddSoundMenuItem(soundPlayerMenu, "worm_tounge_wiggle.wav");
			AddSoundMenuItem(soundPlayerMenu, "yama_faceoff.wav");
			AddSoundMenuItem(soundPlayerMenu, "yama_slam.wav");
			AddSoundMenuItem(soundPlayerMenu, "yamaspew.wav");
			AddSoundMenuItem(soundPlayerMenu, "yeti_roar.wav");
			AddSoundMenuItem(soundPlayerMenu, "yeti_toss.wav");
			AddSoundMenuItem(soundPlayerMenu, "zap.wav");
			AddSoundMenuItem(soundPlayerMenu, "zombie_jump.wav");

			
			//Main menu

			Menu* mainMenu = new Menu(TITLE);

			AddLinkMenuItem(mainMenu, "Player >", playerMenu);
			AddLinkMenuItem(mainMenu, "Object Spawner >", objectSpawnerMenu);
			AddLinkMenuItem(mainMenu, "Teleporting >", teleportingMenu);
			AddLinkMenuItem(mainMenu, "Time >", timeMenu);
			AddLinkMenuItem(mainMenu, "Sound Player >", soundPlayerMenu);

			Menu::previousMenu = mainMenu;
		}
	}
}
