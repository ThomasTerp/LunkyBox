#pragma once

#include <Windows.h>
#include <map>
#include "KeyState.h"


namespace Input
{
	const long long TIME_FIRST_PRESS = 500;
	const long long TIME_PRESS = 50;

	enum class INPUT_MODE
	{
		//Triggers all the time
		NORMAL,
		//Only triggers the first time.
		ONCE,
		//Only triggers the first time and on a interval after a small time. This is basicly like a textbox input.
		CONTINUOUS
	};


	extern std::map<int, KeyState*> keyStates;

	void AddKeyState(int vKey);


	//Called every tick
	void Update();

	//Is a key pressed?
	bool IsKeyPressed(int vKey, INPUT_MODE inputMode);

	//Is a key released?
	bool IsKeyReleased(int vKey, INPUT_MODE inputMode);
}
