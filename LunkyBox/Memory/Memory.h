#pragma once

#include <Windows.h>
#include <vector>


namespace Memory
{
	//Read from a address with offsets
	template<class T>
	T Read(DWORD baseAddress, const std::vector<DWORD> offsets)
	{
		DWORD address = baseAddress;
		unsigned int offsetsSize = offsets.size();

		for(unsigned int i = 0; i < offsetsSize - 1; i++)
		{
			if(address != NULL)
			{
				address = *(DWORD*)(address + offsets[i]);
			}
		}

		return address == NULL ? NULL : *(T*)(address + offsets[offsetsSize - 1]);
	}

	//Write to a address with offsets
	template<class T>
	void Write(DWORD baseAddress, const std::vector<DWORD> offsets, T value)
	{
		DWORD address = baseAddress;
		unsigned int offsetsSize = offsets.size();

		for(unsigned int i = 0; i < offsetsSize - 1; i++)
		{
			if(address != NULL)
			{
				address = *(DWORD*)(address + offsets[i]);
			}
		}

		if(address != NULL)
		{
			*(T*)(address + offsets[offsetsSize - 1]) = value;
		}
	}
}
