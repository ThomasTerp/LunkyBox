#pragma once

#include <Windows.h>
#include "../Vector2.h"


namespace Window
{
	HWND FindMainWindow(unsigned long process_id);

	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);

	BOOL IsMainWindow(HWND handle);

	Vector2 GetRelativeCursorPosition();
}
