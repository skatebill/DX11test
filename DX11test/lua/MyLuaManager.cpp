#include "MyLuaManager.h"

MyLuaManager* MyLuaManager::s_Instance=0;

void MyLuaManager::doFile(const char* file)
{
	std::string s("dofile('");
	s +=file;
	s +="')";
	doString(s.c_str());
}

void MyLuaManager::doString(const char* command){
	int error=luaL_loadbuffer(m_L,command,strlen(command),"test") || lua_pcall(m_L,0,0,0);
	if(error)
	{
		std::cout<<lua_tostring(m_L,-1)<<std::endl;
		lua_pop(m_L,1);
	}

}

void MyLuaManager::registerFun(luaL_Reg* config){
	lua_register(m_L, config->name, config->func);
}