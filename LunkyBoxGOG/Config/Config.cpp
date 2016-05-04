#include "Config.h"
#include <fstream>
#include <algorithm>
#include "../Main.h"
#include "../Module.h"

using namespace std;


namespace Config
{
	map<string, string> values;


	bool Parse(string fileName)
	{
		string configPath = GetModuleDirectory();
		configPath.append("\\");
		configPath.append(fileName);

		ifstream configFile;
		configFile.open(configPath);

		if(configFile.is_open())
		{
			string line;
			while(std::getline(configFile, line))
			{
				std::istringstream line2(line);
				std::string key;

				if(std::getline(line2, key, '='))
				{
					std::string value;

					if(std::getline(line2, value))
					{
						values[key] = value;
					}
				}
			}

			return true;
		}

		return false;
	}

	string GetText(string key)
	{
		return values[key];
	}

	int GetNumber(string key)
	{
		return std::stoi(values[key], nullptr, 0);
	}

	bool GetBool(string key)
	{
		string value = values[key];

		//Convert value to lowercase
		transform(value.begin(), value.end(), value.begin(), tolower);

		if(value == "1" || value == "true" || value == "on" || value == "yes" || value == "y")
		{
			return true;
		}

		return false;
	}
}
