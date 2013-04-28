#pragma once
#include "MyLuaManager.h"
#ifdef USE_LUA
#include "..\winbase\dx11\DX11WBASE.h"


extern "C" int _Version(lua_State* L) {
	puts("this is Versdion 1.0 of console Program");
	puts(LUA_VERSION);
	puts(LUA_COPYRIGHT);
	puts(LUA_AUTHORS);
	return 0;
}
luaL_Reg ConsleGlue[] = { { "Version", _Version }, { 0, 0 } };
#endif