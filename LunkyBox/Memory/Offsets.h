#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace Offsets {
// Offsets used to detect whether we're loading into Steam or GOG
const DWORD STEAM_KALI_ACCEPTS = 0x112D20;
// TODO: This offset isn't correct for the current GOG offsets.
const DWORD GOG_KALI_ACCEPTS = 0x0fda0c;

// UTF-16 string we're looking for at the offset to verify version
const std::wstring KALI_ACCEPTS = L"KALI_ACCEPTS";

extern DWORD CAMERA_STATE;
extern DWORD GLOBAL_STATE;
extern DWORD FUNCTION_PLAYSOUND_OFFSET;
extern DWORD FUNCTION_SPAWNENTITY_OFFSET;

extern bool IS_STEAM;
extern bool IS_GOG;

extern std::vector<DWORD> CAMERA_POSITION_X_OFFSETS;
extern std::vector<DWORD> CAMERA_POSITION_Y_OFFSETS;
extern std::vector<DWORD> TIME_TOTAL_MINUTES_OFFSETS;
extern std::vector<DWORD> TIME_TOTAL_SECONDS_OFFSETS;
extern std::vector<DWORD> TIME_TOTAL_MILLISECONDS_OFFSETS;
extern std::vector<DWORD> TIME_STAGE_MINUTES_OFFSETS;
extern std::vector<DWORD> TIME_STAGE_SECONDS_OFFSETS;
extern std::vector<DWORD> TIME_STAGE_MILLISECONDS_OFFSETS;
extern std::vector<DWORD> GAME_IPR0D0J2V0_OFFSETS;

extern std::vector<DWORD> PLAYER_LOCAL_OFFSETS;

void InitializeOffsets();
} // namespace Offsets
