#pragma once
#include"../../lua/MyLuaManager.h"
#ifdef USE_LUA

/*
*
*	��������Ŀ��ʹ�õĺ����������������ɵ�type���ͣ�����obj id ͨ��id���洢����Ԫ�أ�ͨ��type����ʶ��
*
*/
int createObject(lua_State* L);
int setVisible(lua_State* L);
int registerEventHandler(lua_State* L);

void luaInti();




#endif