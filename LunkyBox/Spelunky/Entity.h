#pragma once

#include <Windows.h>
#include <vector>
#include "../Vector2.h"


namespace Spelunky
{
	const std::vector<DWORD> ENTITY_POSITION_X_OFFSETS = {0x30};
	const std::vector<DWORD> ENTITY_POSITION_Y_OFFSETS = {0x34};
	const std::vector<DWORD> ENTITY_VELOCITY_X_OFFSETS = {0x244};
	const std::vector<DWORD> ENTITY_VELOCITY_Y_OFFSETS = {0x248};
	const std::vector<DWORD> ENTITY_IS_FLIPPED_OFFSETS = {0x9D};

	class Entity
	{
		public:
		DWORD address;


		Entity(DWORD address);

		void SetPosition(Vector2 position);

		Vector2 GetPosition();

		void SetVelocity(Vector2 velocity);

		Vector2 GetVelocity();

		void SetIsFlipped(bool isFlipped);

		bool IsFlipped();

		void PlaySound(const char* sound);
	};
}
