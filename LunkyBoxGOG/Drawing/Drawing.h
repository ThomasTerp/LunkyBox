#pragma once

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "../Vector2.h"


namespace Drawing
{
	void DrawRectangle(LPDIRECT3DDEVICE9 deviceInterface, int x, int y, int width, int height, D3DCOLOR color);

	void DrawText(LPD3DXFONT font, LPCSTR text, int x, int y, int width, int height, D3DCOLOR color);

	int GetTextWidth(LPD3DXFONT font, LPCSTR text);
}
