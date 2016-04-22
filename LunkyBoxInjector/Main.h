#pragma once

#include <Windows.h>
#include <iostream>


const char* TITLE = "LunkyBox Injector";
const char* DLL_NAME = "LunkyBox.dll";
const char* PROCESS_NAME  = "Spelunky.exe";


int main();

DWORD GetProcess(std::string processName);

bool InjectDLL(DWORD ProcessID, std::string dllName);

std::string GetModuleDirectory(HMODULE hModule);

void ConsoleTitle();
