#pragma once
#define USE_LUA_NO
#ifdef USE_LUA
#include<lua.hpp>
#include<iostream>
#include<string>
#pragma comment(lib,"lua5.1.lib")
class MyLuaManager
{
private:
	static MyLuaManager* s_Instance;
	lua_State *m_L;
public:
	MyLuaManager(void):m_L(0){m_L=luaL_newstate();luaL_openlibs(m_L);}
	~MyLuaManager(void){lua_close(m_L);};
	static MyLuaManager* getInstance(){if(!s_Instance)s_Instance=new MyLuaManager();return s_Instance;}

	void doString(const char*);
	void doFile(const char*);
	void registerFun(luaL_Reg*);
	void registerFun(char* funname,lua_CFunction fun);
	void registerLib(char* libName,luaL_Reg*);
};
#endif
