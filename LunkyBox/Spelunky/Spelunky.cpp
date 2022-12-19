#include "Spelunky.h"
#include "../Window/Window.h"
#include "../Memory/Memory.h"


namespace Spelunky
{
	DWORD GetBaseAddress()
	{
		return (DWORD)GetModuleHandle(NULL);
	}

	Entity SpawnEntity(Vector2 position, ENTITY_CLASS entityClass)
	{
		using SpawnEntityFastCall = DWORD(__fastcall*)(DWORD ecx, void* edx, float x, float y, int entityClassID, bool unknown);
		SpawnEntityFastCall SpawnEntity = (SpawnEntityFastCall)(GetBaseAddress() + Offsets::FUNCTION_SPAWNENTITY_OFFSET);
		DWORD ecx = *(DWORD*)(GetBaseAddress() + Offsets::GLOBAL_STATE);

		DWORD entityAddress = SpawnEntity(ecx, NULL, position.x, position.y, (DWORD)entityClass, true);

		return Entity(entityAddress);
	}

	void SetCameraPosition(Vector2 position)
	{
		DWORD baseAddress = GetBaseAddress();

		Memory::Write<float>(GetBaseAddress(), Offsets::CAMERA_POSITION_X_OFFSETS, position.x);
		Memory::Write<float>(GetBaseAddress(), Offsets::CAMERA_POSITION_Y_OFFSETS, position.y);
	}

	Vector2 GetCameraPosition()
	{
		DWORD baseAddress = GetBaseAddress();

		return Vector2(Memory::Read<float>(baseAddress, Offsets::CAMERA_POSITION_X_OFFSETS), Memory::Read<float>(baseAddress, Offsets::CAMERA_POSITION_Y_OFFSETS));
	}

	Vector2 ScreenToWorld(Vector2 screenPosition)
	{
		HWND window = Window::FindMainWindow(GetCurrentProcessId());
		RECT windowRect;
		
		if(GetWindowRect(window, &windowRect))
		{
			long windowWidth = windowRect.right - windowRect.left;
			long windowHeight = windowRect.bottom - windowRect.top;
			
			float worldMultiplier;

			//I know this is not the best way to do it, but its the only way I can get working
			if(windowWidth >= 1920)
			{
				worldMultiplier = WORLD_MULTIPLIER_1920;
			}
			else if(windowWidth >= 1776)
			{
				worldMultiplier = WORLD_MULTIPLIER_1776;
			}
			else if(windowWidth >= 1680)
			{
				worldMultiplier = WORLD_MULTIPLIER_1680;
			}
			else if(windowWidth >= 1600)
			{
				worldMultiplier = WORLD_MULTIPLIER_1600;
			}
			else if(windowWidth >= 1440)
			{
				worldMultiplier = WORLD_MULTIPLIER_1440;
			}
			else if(windowWidth >= 1280)
			{
				worldMultiplier = WORLD_MULTIPLIER_1280;
			}
			else if(windowWidth >= 1152)
			{
				worldMultiplier = WORLD_MULTIPLIER_1152;
			}
			else if(windowWidth >= 1024)
			{
				worldMultiplier = WORLD_MULTIPLIER_1024;
			}
			else if(windowWidth >= 800)
			{
				worldMultiplier = WORLD_MULTIPLIER_800;
			}
			else if(windowWidth >= 720)
			{
				worldMultiplier = WORLD_MULTIPLIER_720;
			}
			else
			{
				worldMultiplier = WORLD_MULTIPLIER_640;
			}
			
			Vector2 position = GetCameraPosition();
			position.x += (screenPosition.x - (windowWidth * 0.5f)) * worldMultiplier;
			position.y -= (screenPosition.y - (windowHeight * 0.5f)) * worldMultiplier;

			return position;
		}

		return Vector2(0, 0);
	}

	void SetTotalMinutes(byte minutes)
	{
		Memory::Write<byte>(GetBaseAddress(), Offsets::TIME_TOTAL_MINUTES_OFFSETS, minutes);
	}

	byte GetTotalMinutes()
	{
		return Memory::Read<byte>(GetBaseAddress(), Offsets::TIME_TOTAL_MINUTES_OFFSETS);
	}

	void SetTotalSeconds(byte seconds)
	{
		Memory::Write<byte>(GetBaseAddress(), Offsets::TIME_TOTAL_SECONDS_OFFSETS, seconds);
	}

	byte GetTotalSeconds()
	{
		return Memory::Read<byte>(GetBaseAddress(), Offsets::TIME_TOTAL_SECONDS_OFFSETS);
	}

	void SetTotalMilliseconds(int millisecond)
	{
		Memory::Write<int>(GetBaseAddress(), Offsets::TIME_TOTAL_MILLISECONDS_OFFSETS, millisecond);
	}

	int GetTotalMilliseconds()
	{
		return Memory::Read<int>(GetBaseAddress(), Offsets::TIME_TOTAL_MILLISECONDS_OFFSETS);
	}

	void SetStageMinutes(byte minutes)
	{
		Memory::Write<byte>(GetBaseAddress(), Offsets::TIME_STAGE_MINUTES_OFFSETS, minutes);
	}

	byte GetStageMinutes()
	{
		return Memory::Read<byte>(GetBaseAddress(), Offsets::TIME_STAGE_MINUTES_OFFSETS);
	}

	void SetStageSeconds(byte seconds)
	{
		Memory::Write<byte>(GetBaseAddress(), Offsets::TIME_STAGE_SECONDS_OFFSETS, seconds);
	}

	byte GetStageSeconds()
	{
		return Memory::Read<byte>(GetBaseAddress(), Offsets::TIME_STAGE_SECONDS_OFFSETS);
	}

	void SetStageMilliseconds(int millisecond)
	{
		Memory::Write<int>(GetBaseAddress(), Offsets::TIME_STAGE_MILLISECONDS_OFFSETS, millisecond);
	}

	int GetStageMilliseconds()
	{
		return Memory::Read<int>(GetBaseAddress(), Offsets::TIME_STAGE_MILLISECONDS_OFFSETS);
	}

	void SetIPR0D0J2V0()
	{
		if (Offsets::IS_STEAM) {
			Memory::Write<byte>(GetBaseAddress(), Offsets::GAME_IPR0D0J2V0_OFFSETS, 0);
		}
	}
}
