#pragma once
#include <string>
#include <windows.h>
#include <Shlwapi.h>
#include "d3dx9/d3dx9.h"
#include "czConfig.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#pragma comment(lib, "Libs\\czMain.lib")
#   define ex_czApi   extern "C" __declspec(dllexport)
#   define in_czApi   extern __declspec(dllimport)

struct pluginInfo
{
	std::string Description;
	std::string Version;
	std::string Author;
	uint64_t UIBool;
};


ex_czApi void shutDown();
ex_czApi void draw(LPDIRECT3DDEVICE9, DWORD*);
ex_czApi bool keyboardEvent(WPARAM Keycode, UINT WindowsMessage);
ex_czApi void loadFonts();
ex_czApi void init(HMODULE mainMod);
ex_czApi bool canDraw();
ex_czApi void getInfo(pluginInfo* pl_info);
static HMODULE thisMod;
namespace czApi
{
	inline HMODULE mainMod = nullptr;
	in_czApi void writeConsole(std::string msg);
	in_czApi std::string getGUID();
	in_czApi DWORD* addHook(char* hk_address, char* re_fn, char* tramp);
	in_czApi void removeHook(DWORD*);
	in_czApi void addHudWindows(int num_windows);
	in_czApi void removeHudWindows(int num_windows);
	in_czApi bool Require(std::string module);
	in_czApi DWORD cod4x;
};

