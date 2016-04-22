#pragma once

#include <string>
#include <Windows.h>


EXTERN_C IMAGE_DOS_HEADER __ImageBase;


std::string GetModuleDirectory();
