#include "main.h"
#include <TlHelp32.h>

using namespace std;

typedef HINSTANCE(*fpLoadLibrary)(char*);


int main()
{
	while(true)
	{
		//Ask user to restart process if its open

		if(GetProcess(PROCESS_NAME) != NULL)
		{
			DWORD processID = GetProcess(PROCESS_NAME);

			ConsoleTitle();
			cout << DLL_NAME << " has to be injected when " << PROCESS_NAME << " is opened" << endl;
			cout << "Restart " << PROCESS_NAME << " to proceed..." << endl;

			while(processID != NULL)
			{
				processID = GetProcess(PROCESS_NAME);
			}
		}


		//Wait for process to open

		ConsoleTitle();
		cout << "Waiting for " << PROCESS_NAME << " to open..." << endl;

		DWORD processID = NULL;

		while(processID == NULL)
		{
			processID = GetProcess(PROCESS_NAME);

			Sleep(10);
		}


		//Inject

		bool isMemoryWritten = InjectDLL(processID, DLL_NAME);


		//Failed

		if(!isMemoryWritten)
		{
			ConsoleTitle();
			cout << "Failed to inject " << DLL_NAME << " into " << PROCESS_NAME << endl;
			cout << "Press enter to retry..." << endl;

			cin.get();

			continue;
		}


		//Success

		ConsoleTitle();
		cout << "Successfully injected " << DLL_NAME << " into " << PROCESS_NAME << endl;
		cout << "You can close this injector" << endl;

		
		//Wait until process closes

		while(processID != NULL)
		{
			processID = GetProcess(PROCESS_NAME);
		}
	}

	return 0;
}

DWORD GetProcess(string processName)
{
	DWORD processID = NULL;
	HANDLE hProcSnap;
	PROCESSENTRY32 pe32 = {sizeof(PROCESSENTRY32)};

	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(Process32First(hProcSnap, &pe32))
	{
		do
		{
			if(!strcmp(pe32.szExeFile, processName.c_str()))
			{
				processID = pe32.th32ProcessID;
				break;
			}
		}
		while(Process32Next(hProcSnap, &pe32));
	}

	CloseHandle(hProcSnap);

	return processID;
}

bool InjectDLL(DWORD ProcessID, string dllName)
{
	HANDLE hProc;
	LPVOID paramAddr;

	HINSTANCE hDll = LoadLibrary("KERNEL32");

	fpLoadLibrary LoadLibraryAddr = (fpLoadLibrary)GetProcAddress(hDll, "LoadLibraryA");

	hProc = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessID);

	string dllPath = GetModuleDirectory(NULL);
	dllPath.append("\\");
	dllPath.append(dllName);

	paramAddr = VirtualAllocEx(hProc, 0, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
	bool isMemoryWritten = WriteProcessMemory(hProc, paramAddr, dllPath.c_str(), dllPath.size() + 1, NULL) ? true : false;

	CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAddr, paramAddr, 0, 0);

	CloseHandle(hProc);

	return isMemoryWritten;
}

string GetModuleDirectory(HMODULE hModule)
{
	char buffer[MAX_PATH];
	GetModuleFileName(hModule, buffer, MAX_PATH);

	string::size_type pos = string(buffer).find_last_of("\\/");

	return string(buffer).substr(0, pos);
}

void ConsoleTitle()
{
	system("CLS");
	cout << TITLE << endl << endl;
}
