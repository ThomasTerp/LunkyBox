#include "Input.h"


namespace Input
{
	std::map<int, KeyState*> keyStates;


	void Update()
	{
		for(auto const &it : keyStates)
		{
			int vKey = it.first;
			KeyState* keyState = it.second;

			keyState->oldState = keyState->state;
			keyState->state = GetAsyncKeyState(vKey) ? true : false;
		}
	}

	void AddKeyState(int vKey)
	{
		//If the key dont already exist in the map
		if(!keyStates.count(vKey))
		{
			keyStates[vKey] = new KeyState();
		}
	}

	bool IsKeyPressed(int vKey, INPUT_MODE inputMode)
	{
		AddKeyState(vKey);

		switch(inputMode)
		{
			case INPUT_MODE::NORMAL:
			{
				return keyStates[vKey]->state;
			}

			case INPUT_MODE::ONCE:
			{
				return keyStates[vKey]->state && !keyStates[vKey]->oldState;
			}

			case INPUT_MODE::CONTINUOUS:
			{
				if(keyStates[vKey]->state)
				{
					long long firstPressedduration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - keyStates[vKey]->firstPressedTime).count();

					if(!keyStates[vKey]->oldState)
					{
						keyStates[vKey]->firstPressedTime = std::chrono::high_resolution_clock::now();
						keyStates[vKey]->pressedTime = std::chrono::high_resolution_clock::now();

						return true;
					}

					if(firstPressedduration >= TIME_FIRST_PRESS)
					{
						long long pressedDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - keyStates[vKey]->pressedTime).count();

						if(pressedDuration >= TIME_PRESS)
						{
							keyStates[vKey]->pressedTime = std::chrono::high_resolution_clock::now();

							return true;
						}
					}
				}

				break;
			}
		}

		return false;
	}

	bool IsKeyReleased(int vKey, INPUT_MODE inputMode)
	{
		AddKeyState(vKey);

		switch(inputMode)
		{
			case INPUT_MODE::NORMAL:
			{
				return keyStates[vKey]->state;
			}

			case INPUT_MODE::ONCE:
			case INPUT_MODE::CONTINUOUS:
			{
				return !keyStates[vKey]->state && keyStates[vKey]->oldState;
			}
		}

		return false;
	}
}
