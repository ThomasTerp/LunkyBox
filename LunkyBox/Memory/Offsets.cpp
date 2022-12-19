#include "Offsets.h"
namespace Offsets {

DWORD CAMERA_STATE = 0;
DWORD GLOBAL_STATE = 0;
DWORD FUNCTION_PLAYSOUND_OFFSET = 0;
DWORD FUNCTION_SPAWNENTITY_OFFSET = 0;

bool IS_STEAM = false;
bool IS_GOG = false;

std::vector<DWORD> CAMERA_POSITION_X_OFFSETS = {0};
std::vector<DWORD> CAMERA_POSITION_Y_OFFSETS = {0};
std::vector<DWORD> TIME_TOTAL_MINUTES_OFFSETS = {0};
std::vector<DWORD> TIME_TOTAL_SECONDS_OFFSETS = {0};
std::vector<DWORD> TIME_TOTAL_MILLISECONDS_OFFSETS = {0};
std::vector<DWORD> TIME_STAGE_MINUTES_OFFSETS = {0};
std::vector<DWORD> TIME_STAGE_SECONDS_OFFSETS = {0};
std::vector<DWORD> TIME_STAGE_MILLISECONDS_OFFSETS = {0};
std::vector<DWORD> GAME_IPR0D0J2V0_OFFSETS = {0};

std::vector<DWORD> PLAYER_LOCAL_OFFSETS = {0};

} // namespace Offsets

DWORD GetBaseAddress() { return (DWORD)GetModuleHandle(NULL); }

bool isSteam() {

  auto baseAddress = GetBaseAddress();
  wchar_t *kaliAcceptsPtr =
      (wchar_t *)((DWORD *)(baseAddress + Offsets::STEAM_KALI_ACCEPTS));
  std::wstring kaliAcceptsStr = std::wstring(kaliAcceptsPtr);

  if (kaliAcceptsStr.compare(Offsets::KALI_ACCEPTS) == 0) {
    Offsets::IS_STEAM = true;
    return true;
  }

  return false;
}

bool isGOG() {
  auto baseAddress = GetBaseAddress();
  wchar_t *kaliAcceptsPtr =
      (wchar_t *)((DWORD *)(baseAddress + Offsets::GOG_KALI_ACCEPTS));
  std::wstring kaliAcceptsStr = std::wstring(kaliAcceptsPtr);

  if (kaliAcceptsStr.compare(Offsets::KALI_ACCEPTS) == 0) {
    Offsets::IS_GOG = true;
    return true;
  }

  return false;
}

void Offsets::InitializeOffsets() {
  if (isSteam()) {
    Offsets::CAMERA_STATE = 0x154510;
    Offsets::GLOBAL_STATE = 0x15446C;
    Offsets::FUNCTION_PLAYSOUND_OFFSET = 0x16A95;
    Offsets::FUNCTION_SPAWNENTITY_OFFSET = 0x70AB0;
  } else if (isGOG()) {
    Offsets::CAMERA_STATE = 0x131330;
    Offsets::GLOBAL_STATE = 0x13128C;
    Offsets::FUNCTION_PLAYSOUND_OFFSET = 0x16500;
    Offsets::FUNCTION_SPAWNENTITY_OFFSET = 0x6DDD0;
  }

  if (Offsets::IS_STEAM || Offsets::IS_GOG) {
    Offsets::CAMERA_POSITION_X_OFFSETS = {Offsets::CAMERA_STATE, 0x0};
    Offsets::CAMERA_POSITION_Y_OFFSETS = {Offsets::CAMERA_STATE, 0x4};

    Offsets::TIME_TOTAL_MINUTES_OFFSETS = {Offsets::CAMERA_STATE, 0x30, 0x280,
                                           0x52AC};
    Offsets::TIME_TOTAL_SECONDS_OFFSETS = {Offsets::CAMERA_STATE, 0x30, 0x280,
                                           0x52B0};
    Offsets::TIME_TOTAL_MILLISECONDS_OFFSETS = {Offsets::CAMERA_STATE, 0x30,
                                                0x280, 0x52B7};

    Offsets::TIME_STAGE_MINUTES_OFFSETS = {Offsets::CAMERA_STATE, 0x30, 0x280,
                                           0x52BC};
    Offsets::TIME_STAGE_SECONDS_OFFSETS = {Offsets::CAMERA_STATE, 0x30, 0x280,
                                           0x52C0};
    Offsets::TIME_STAGE_MILLISECONDS_OFFSETS = {Offsets::CAMERA_STATE, 0x30,
                                                0x280, 0x52C7};
    Offsets::GAME_IPR0D0J2V0_OFFSETS = {Offsets::GLOBAL_STATE, 0x440629};

    Offsets::PLAYER_LOCAL_OFFSETS = {Offsets::CAMERA_STATE, 0x30};
  }
}
