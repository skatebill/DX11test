#pragma once
#include"../../lua/MyLuaManager.h"
#ifdef USE_LUA

/*
*
*	生成所需目标使用的函数，传入所需生成的type类型，返回obj id 通过id来存储所有元素，通过type码来识别
*
*/
int createObject(lua_State* L);
int setVisible(lua_State* L);
int registerEventHandler(lua_State* L);

void luaInti();




#endif