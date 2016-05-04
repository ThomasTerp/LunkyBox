//Thanks to Roverturbo | www.unknowncheats.me for this d3d9 code


#include "D3d9.h"


typedef HRESULT(WINAPI* CreateDevice_Prototype)        (LPDIRECT3D9, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, LPDIRECT3DDEVICE9*);
typedef HRESULT(WINAPI* Reset_Prototype)               (LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef HRESULT(WINAPI* EndScene_Prototype)            (LPDIRECT3DDEVICE9);
typedef HRESULT(WINAPI* DrawIndexedPrimitive_Prototype)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);

CreateDevice_Prototype         CreateDevice_Pointer = NULL;
Reset_Prototype                Reset_Pointer = NULL;
EndScene_Prototype             EndScene_Pointer = NULL;
DrawIndexedPrimitive_Prototype DrawIndexedPrimitive_Pointer = NULL;

DWORD WINAPI VirtualMethodTableRepatchingLoopToCounterExtensionRepatching(LPVOID);
PDWORD direct3D_VMTable = NULL;


HRESULT WINAPI Direct3DCreate9_VMTable()
{
	LPDIRECT3D9 Direct3D_Object = Direct3DCreate9(D3D_SDK_VERSION);

	if(Direct3D_Object == NULL)
		return D3DERR_INVALIDCALL;

	direct3D_VMTable = (PDWORD)*(PDWORD)Direct3D_Object;
	Direct3D_Object->Release();

	DWORD dwProtect;

	if(VirtualProtect(&direct3D_VMTable[16], sizeof(DWORD), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PDWORD)&CreateDevice_Pointer = direct3D_VMTable[16];
		*(PDWORD)&direct3D_VMTable[16] = (DWORD)CreateDevice_Detour;

		if(VirtualProtect(&direct3D_VMTable[16], sizeof(DWORD), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	return D3D_OK;
}

HRESULT WINAPI CreateDevice_Detour(LPDIRECT3D9 direct3DObject, UINT adapter, D3DDEVTYPE deviceType, HWND focusWindow, DWORD behaviorFlags, D3DPRESENT_PARAMETERS* presentationParameters, LPDIRECT3DDEVICE9* returnedDeviceInterface)
{
	HRESULT returnedResult = CreateDevice_Pointer(direct3DObject, adapter, deviceType, focusWindow, behaviorFlags, presentationParameters, returnedDeviceInterface);

	DWORD dwProtect;

	if(VirtualProtect(&direct3D_VMTable[16], sizeof(DWORD), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PDWORD)&direct3D_VMTable[16] = *(PDWORD)&CreateDevice_Pointer;
		CreateDevice_Pointer = NULL;

		if(VirtualProtect(&direct3D_VMTable[16], sizeof(DWORD), dwProtect, &dwProtect) == 0)
		{
			return D3DERR_INVALIDCALL;
		}
	}
	else
		return D3DERR_INVALIDCALL;

	if(returnedResult == D3D_OK)
	{
		direct3D_VMTable = (PDWORD)*(PDWORD)*returnedDeviceInterface;

		*(PDWORD)&Reset_Pointer = (DWORD)direct3D_VMTable[16];
		*(PDWORD)&EndScene_Pointer = (DWORD)direct3D_VMTable[42];
		*(PDWORD)&DrawIndexedPrimitive_Pointer = (DWORD)direct3D_VMTable[82];

		if(CreateThread(NULL, 0, VirtualMethodTableRepatchingLoopToCounterExtensionRepatching, NULL, 0, NULL) == NULL)
		{
			return D3DERR_INVALIDCALL;
		}
	}

	return returnedResult;
}

HRESULT WINAPI Reset_Detour(LPDIRECT3DDEVICE9 deviceInterface, D3DPRESENT_PARAMETERS* presentationParameters)
{
	return Reset_Pointer(deviceInterface, presentationParameters);
}

HRESULT WINAPI EndScene_Detour(LPDIRECT3DDEVICE9 deviceInterface)
{
	Mod::D3D9EndScene(deviceInterface);

	return EndScene_Pointer(deviceInterface);
}

HRESULT WINAPI DrawIndexedPrimitive_Detour(LPDIRECT3DDEVICE9 deviceInterface, D3DPRIMITIVETYPE type, INT baseIndex, UINT minIndex, UINT numVertices, UINT startIndex, UINT primitiveCount)
{
	LPDIRECT3DVERTEXBUFFER9 Stream_Data;
	UINT Offset = 0;
	UINT Stride = 0;

	if(deviceInterface->GetStreamSource(0, &Stream_Data, &Offset, &Stride) == D3D_OK)
	{
		Stream_Data->Release();
	}

	if(Stride == 0)
	{

	}

	return DrawIndexedPrimitive_Pointer(deviceInterface, type, baseIndex, minIndex, numVertices, startIndex, primitiveCount);
}

DWORD WINAPI VirtualMethodTableRepatchingLoopToCounterExtensionRepatching(LPVOID param)
{
	UNREFERENCED_PARAMETER(param);

	while(1)
	{
		Sleep(100);

		*(PDWORD)&direct3D_VMTable[16] = (DWORD)Reset_Detour;
		*(PDWORD)&direct3D_VMTable[42] = (DWORD)EndScene_Detour;
		*(PDWORD)&direct3D_VMTable[82] = (DWORD)DrawIndexedPrimitive_Detour;
	}

	return 1;
}
