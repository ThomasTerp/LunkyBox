#include "Window.h"


namespace Window
{
	struct HandleData
	{
		unsigned long processID;
		HWND bestHandle;
	};

	HWND FindMainWindow(DWORD processID)
	{
		HandleData data;
		data.processID = processID;
		data.bestHandle = 0;
		EnumWindows(EnumWindowsCallback, (LPARAM)&data);
		return data.bestHandle;
	}

	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM param)
	{
		HandleData& data = *(HandleData*)param;
		DWORD processID = 0;

		GetWindowThreadProcessId(handle, &processID);

		if(data.processID != processID || !IsMainWindow(handle))
		{
			return TRUE;
		}

		data.bestHandle = handle;

		return FALSE;
	}

	BOOL IsMainWindow(HWND handle)
	{
		return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
	}

	Vector2 GetRelativeCursorPosition()
	{
		POINT point;
		HWND window = FindMainWindow(GetCurrentProcessId());

		if(GetCursorPos(&point) && ScreenToClient(window, &point))
		{
			return Vector2((float)point.x, (float)point.y);
		}

		return Vector2(0, 0);
	}
}
