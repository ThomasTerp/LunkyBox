#include "Player.h"
#include "../Memory/Memory.h"


namespace Spelunky
{
	Player Player::GetLocal()
	{
		return Player(Memory::Read<DWORD>(GetBaseAddress(), PLAYER_LOCAL_OFFSETS));
	}

	Player::Player(DWORD address) : Entity(address)
	{

	}

	void Player::Teleport(Vector2 position)
	{
		Entity spelunkerTeleportEntity1 = SpawnEntity(GetPosition(), ENTITY_CLASS::SPELUNKER_TELEPORT);
		spelunkerTeleportEntity1.SetIsFlipped(IsFlipped());

		SetPosition(position);

		Entity spelunkerTeleportEntity2 = SpawnEntity(position, ENTITY_CLASS::SPELUNKER_TELEPORT);
		spelunkerTeleportEntity2.SetIsFlipped(IsFlipped());

		PlaySound("teleport.wav");
	}

	void Player::SetHasItem(ITEM_CLASS itemClass, bool hasItem)
	{
		std::vector<DWORD> offsets = {};
		offsets.insert(offsets.end(), PLAYER_HAS_ITEM_OFFSETS.begin(), PLAYER_HAS_ITEM_OFFSETS.end());
		offsets.push_back((DWORD)itemClass);

		Memory::Write<byte>(address, offsets, hasItem ? 1 : 0);
	}

	bool Player::HasItem(ITEM_CLASS itemClass)
	{
		std::vector<DWORD> offsets = {};
		offsets.insert(offsets.end(), PLAYER_HAS_ITEM_OFFSETS.begin(), PLAYER_HAS_ITEM_OFFSETS.end());
		offsets.push_back((DWORD)itemClass);

		return  Memory::Read<byte>(address, offsets) == 1 ? true : false;
	}

	void Player::SetGold(int gold)
	{
		Memory::Write<int>(address, PLAYER_GOLD_OFFSETS, gold);
	}

	int Player::GetGold()
	{
		return Memory::Read<int>(address, PLAYER_GOLD_OFFSETS);
	}

	void Player::SetHearts(byte hearts)
	{
		Memory::Write<byte>(address, PLAYER_HEARTS_OFFSETS, hearts);
	}

	byte Player::GetHearts()
	{
		return Memory::Read<byte>(address, PLAYER_HEARTS_OFFSETS);
	}

	void Player::SetBombs(byte bombs)
	{
		Memory::Write<byte>(address, PLAYER_BOMBS_OFFSETS, bombs);
	}

	byte Player::GetBombs()
	{
		return Memory::Read<byte>(address, PLAYER_BOMBS_OFFSETS);
	}

	void Player::SetRopes(byte ropes)
	{
		Memory::Write<byte>(address, PLAYER_ROPES_OFFSETS, ropes);
	}

	byte Player::GetRopes()
	{
		return Memory::Read<byte>(address, PLAYER_ROPES_OFFSETS);
	}

	void Player::SetFavor(byte favor)
	{
		Memory::Write<byte>(address, PLAYER_FAVOR_OFFSETS, favor);
	}

	byte Player::GetFavor()
	{
		return Memory::Read<byte>(address, PLAYER_FAVOR_OFFSETS);
	}
}
