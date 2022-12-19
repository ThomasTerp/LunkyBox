#include "Entity.h"
#include "Spelunky.h"
#include "../Memory/Memory.h"


namespace Spelunky
{
	Entity::Entity(DWORD address)
	{
		this->address = address;
	}

	void Entity::SetPosition(Vector2 position)
	{
		Memory::Write<float>(address, ENTITY_POSITION_X_OFFSETS, position.x);
		Memory::Write<float>(address, ENTITY_POSITION_Y_OFFSETS, position.y);
	}

	Vector2 Entity::GetPosition()
	{
		return Vector2(Memory::Read<float>(address, ENTITY_POSITION_X_OFFSETS), Memory::Read<float>(address, ENTITY_POSITION_Y_OFFSETS));
	}

	void Entity::SetVelocity(Vector2 velocity)
	{
		Memory::Write<float>(address, ENTITY_VELOCITY_X_OFFSETS, velocity.x);
		Memory::Write<float>(address, ENTITY_VELOCITY_Y_OFFSETS, velocity.y);
	}

	Vector2 Entity::GetVelocity()
	{
		return Vector2(Memory::Read<float>(address, ENTITY_VELOCITY_X_OFFSETS) , Memory::Read<float>(address, ENTITY_VELOCITY_Y_OFFSETS));
	}

	void Entity::SetIsFlipped(bool isFlipped)
	{
		Memory::Write<byte>(address, ENTITY_IS_FLIPPED_OFFSETS, isFlipped ? 1 : 0);
	}

	bool Entity::IsFlipped()
	{
		return Memory::Read<byte>(address, ENTITY_IS_FLIPPED_OFFSETS) == 1 ? true : false;
	}

	void Entity::PlaySound(const char* sound)
	{
		using PlaySoundFastCall = VOID(__fastcall*)(DWORD entityAddress, void* edx, const char* sound);
		PlaySoundFastCall PlaySound = (PlaySoundFastCall)(GetBaseAddress() + Offsets::FUNCTION_PLAYSOUND_OFFSET);

		PlaySound(address, NULL, sound);
	}
}
