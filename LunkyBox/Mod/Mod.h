#pragma once

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "Menu.h"
#include "../Spelunky/Spelunky.h"
#include "../Spelunky/Player.h"
#include "../Memory/Offsets.h"


namespace Mod
{
	const char* const TITLE = "LunkyBox v1.4";
	const char* const FONT = "Arial";
	const char* const CONFIG_FILE = "config.txt";

	extern Spelunky::Player player;
	extern LPD3DXFONT fontMenuTitle;
	extern LPD3DXFONT fontMenuPageNumber;
	extern LPD3DXFONT fontMenuItem;
	extern D3DCOLOR color1;
	extern D3DCOLOR color2;


	DWORD WINAPI Start(LPVOID param);

	void Update();

	void D3D9Reset(LPDIRECT3DDEVICE9 deviceInterface, D3DPRESENT_PARAMETERS* presentationParameters);

	void D3D9EndScene(LPDIRECT3DDEVICE9 deviceInterface);

	void MenuUpdate();

	void InfiniteStatsUpdate();

	void FrozenTimeUpdate();

	void CreateFonts(LPDIRECT3DDEVICE9 deviceInterface);

	void CreateMenus();
}
