#include "Drawing.h"


namespace Drawing
{
	void DrawRectangle(LPDIRECT3DDEVICE9 deviceInterface, int x, int y, int width, int height, D3DCOLOR color)
	{
		D3DRECT rect = {
			x,
			y,
			x + width,
			y + height
		};

		deviceInterface->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
	}

	void DrawText(LPD3DXFONT font, LPCSTR text, int x, int y, int width, int height, D3DCOLOR color)
	{
		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = x + width;
		rect.bottom = y + height;

		font->DrawText(NULL, text, -1, &rect, 0, color);
	}

	int GetTextWidth(LPD3DXFONT font, LPCSTR text)
	{
		RECT rect = {0, 0, 0, 0};
		
		if(font)
		{
			// calculate required rect
			font->DrawText(NULL, text, -1, &rect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
		}

		// return width
		return rect.right - rect.left;
	}
}
