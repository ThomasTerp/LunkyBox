#include "Module.h"

using namespace std;


string GetModuleDirectory()
{
	char path[MAX_PATH] = {0};
	GetModuleFileName((HINSTANCE)&__ImageBase, path, _countof(path));

	string::size_type pos = string(path).find_last_of("\\/");

	return string(path).substr(0, pos);
}
