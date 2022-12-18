#pragma once

#include <Windows.h>
#include <vector>
#include "Spelunky.h"
#include "Entity.h"
#include "ITEM_CLASS.h"
#include "../Vector2.h"


namespace Spelunky
{
	const std::vector<DWORD> PLAYER_LOCAL_OFFSETS = {0x154510, 0x30};
	const std::vector<DWORD> PLAYER_HAS_ITEM_OFFSETS = {0x280};
	const std::vector<DWORD> PLAYER_GOLD_OFFSETS = {0x280, 0x5298};
	const std::vector<DWORD> PLAYER_HEARTS_OFFSETS = {0x140};
	const std::vector<DWORD> PLAYER_BOMBS_OFFSETS = {0x280, 0x10};
	const std::vector<DWORD> PLAYER_ROPES_OFFSETS = {0x280, 0x14};
	const std::vector<DWORD> PLAYER_FAVOR_OFFSETS = {0x280, 0x529C};


	class Player : public Entity
	{
		public:
		static Player GetLocal();

		Player(DWORD address);

		//Fancy version of SetPosition
		void Teleport(Vector2 position);

		void SetHasItem(ITEM_CLASS itemClass, bool hasItem);

		bool HasItem(ITEM_CLASS itemClass);

		void SetGold(int gold);

		int GetGold();

		void SetHearts(byte hearts);

		byte GetHearts();

		void SetBombs(byte bombs);

		byte GetBombs();

		void SetRopes(byte ropes);

		byte GetRopes();

		void SetFavor(byte favor);

		byte GetFavor();
	};
}
