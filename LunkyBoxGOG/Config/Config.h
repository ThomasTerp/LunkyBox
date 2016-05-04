#pragma once

#include <string>
#include <map>


namespace Config
{
	bool Parse(std::string fileName);

	std::string GetText(std::string key);

	int GetNumber(std::string key);

	bool GetBool(std::string key);
}
