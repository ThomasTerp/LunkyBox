#pragma once

#include <Windows.h>
#include "Entity.h"
#include "Player.h"
#include "ENTITY_CLASS.h"
#include "../Vector2.h"
#include "../Memory/Offsets.h"


namespace Spelunky
{
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
