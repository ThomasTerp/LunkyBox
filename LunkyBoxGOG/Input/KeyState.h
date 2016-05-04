#pragma once

#include <chrono>


namespace Input
{
	class KeyState
	{
		public:
		bool state;
		bool oldState;
		std::chrono::time_point<std::chrono::steady_clock> firstPressedTime;
		std::chrono::time_point<std::chrono::steady_clock> pressedTime;

		KeyState();
	};
}
