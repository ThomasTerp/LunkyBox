#pragma once

#include <Windows.h>
#include "Entity.h"
#include "Player.h"
#include "ENTITY_CLASS.h"
#include "../Vector2.h"


namespace Spelunky
{
	const DWORD FUNCTION_PLAYSOUND_OFFSET = 0x16A95;
	const DWORD FUNCTION_SPAWNENTITY_OFFSET = 0x70AB0;

	const std::vector<DWORD> CAMERA_POSITION_X_OFFSETS = {0x154510, 0x0};
	const std::vector<DWORD> CAMERA_POSITION_Y_OFFSETS = {0x154510, 0x4};

	const std::vector<DWORD> TIME_TOTAL_MINUTES_OFFSETS = {0x154510, 0x30, 0x280, 0x52AC};
	const std::vector<DWORD> TIME_TOTAL_SECONDS_OFFSETS = {0x154510, 0x30, 0x280, 0x52B0};
	const std::vector<DWORD> TIME_TOTAL_MILLISECONDS_OFFSETS = {0x154510, 0x30, 0x280, 0x52B7};

	const std::vector<DWORD> TIME_STAGE_MINUTES_OFFSETS = {0x154510, 0x30, 0x280, 0x52BC};
	const std::vector<DWORD> TIME_STAGE_SECONDS_OFFSETS = {0x154510, 0x30, 0x280, 0x52C0};
	const std::vector<DWORD> TIME_STAGE_MILLISECONDS_OFFSETS = {0x154510, 0x30, 0x280, 0x52C7};

	const std::vector<DWORD> GAME_IPR0D0J2V0_OFFSETS = {0x15446C, 0x440629};

	const float WORLD_MULTIPLIER_1920 = 0.0104f;
	const float WORLD_MULTIPLIER_1776 = 0.0112f;
	const float WORLD_MULTIPLIER_1680 = 0.0119f;
	const float WORLD_MULTIPLIER_1600 = 0.0125f;
	const float WORLD_MULTIPLIER_1440 = 0.0138f;
	const float WORLD_MULTIPLIER_1280 = 0.0157f;
	const float WORLD_MULTIPLIER_1152 = 0.0173f;
	const float WORLD_MULTIPLIER_1024 = 0.0196f;
	const float WORLD_MULTIPLIER_800 = 0.025f;
	const float WORLD_MULTIPLIER_720 = 0.0282f;
	const float WORLD_MULTIPLIER_640 = 0.0318f;

	
	DWORD GetBaseAddress();

	Entity SpawnEntity(Vector2 position, ENTITY_CLASS entityClass);

	void SetCameraPosition(Vector2 position);

	Vector2 GetCameraPosition();

	Vector2 ScreenToWorld(Vector2 screenPosition);

	void SetTotalMinutes(byte minutes);

	byte GetTotalMinutes();

	void SetTotalSeconds(byte seconds);

	byte GetTotalSeconds();

	void SetTotalMilliseconds(int millisecond);

	int GetTotalMilliseconds();

	void SetStageMinutes(byte minutes);

	byte GetStageMinutes();

	void SetStageSeconds(byte seconds);

	byte GetStageSeconds();

	void SetStageMilliseconds(int millisecond);

	int GetStageMilliseconds();

	void SetIPR0D0J2V0();
}
