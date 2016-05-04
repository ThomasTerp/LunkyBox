#include "Main.h"


bool APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	switch(reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//Create new thread for mod

			CreateThread(NULL, 0, &Mod::Start, NULL, NULL, NULL);


			//Direct X hooking

			DisableThreadLibraryCalls(hModule);

			if(Direct3DCreate9_VMTable() != D3D_OK)
			{
				return false;
			}

			break;
		}

		case DLL_PROCESS_DETACH:
		{
			break;
		}

		case DLL_THREAD_ATTACH:
		{
			break;
		}

		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	return true;
}
