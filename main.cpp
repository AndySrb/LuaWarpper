// luaexample.cpp
#include <iostream>
#include <map>

#ifdef __cplusplus
# include <lua5.2/lua.hpp>
#else
# include <lua5.2/lua.h>
# include <lua5.2/lualib.h>
# include <lua5.2/lauxlib.h>
#endif

#include "luaLoader.h"

template <class T>
void MRKUSED(T)
{}

bool CheckLua(lua_State *L, int r)
{
	if(r != LUA_OK)
	{
	std::string errormsg = lua_tostring(L, -1);
	std::cout << errormsg << std::endl;
	return false;
	}
	return true;
}


typedef luaLoader::L_ReturnType L_ReturnType;


int main(int argc, char* argv[])
{
	MRKUSED(argc);
	MRKUSED(argv);

	std::string filename = "LuaFiles.lua";

	// initialization
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	luaLoader luaLoad(L);
	if (CheckLua(L,luaL_dofile(L,filename.c_str())))
	{
		luaLoad.readGlobalValue("intVal");
		int val = luaLoad;
		std::cout << "Value read form lua: " << val << std::endl;

		//This is intended way of getting string from lua;
		luaLoad.readFromTable("Table","Name");
		std::string str = luaLoad;
		std::cout << "Value read form lua Table: " << str << std::endl;
		
		//If you allredy declared std::string this is the way
			//std::string str = "";
			//luaLoad.readGlobalValue("Name");
			//str = std::string(luaLoad);
			//std::cout << "Value read form lua: " << str << std::endl;

		luaLoad.readFromTable("Table","Int");
		int TableVal = luaLoad;
		std::cout << "Value read from lua Table: " << TableVal << std::endl;

		//this is how to handle errors, you can see luaLoader.h to see all list of exceptions in enum L_ReturnError 
		try
		{
			luaLoad.readFromTable("Table","Desc");
			std::cout << std::string(luaLoad) << std::endl;
		}
		catch (luaLoader::L_ReturnError e)
		{
			std::cout << "Erorr caught" << std::endl;
			if (e == luaLoader::L_ReturnError::TABLE_VALUE_DOES_NOT_EXIST)
				std::cout << "caught: luaLoader::L_ReturnError::TABLE_VALUE_DOES_NOT_EXIST" << std::endl;
		}
	}
 

	// cleanup
	lua_close(L);
	std::cin.get(); // Use this insead std::system("")
	return 0;
}
