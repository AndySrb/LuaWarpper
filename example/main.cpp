#include <iostream>
#include <string>
#include <vector>

#ifdef __cplusplus
# include <lua5.3/lua.hpp>
#else
# include <lua5.3/lua.h>
# include <lua5.3/lualib.h>
# include <lua5.3/lauxlib.h>
#endif

#include "../luaLoader.h"

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


int luaAdd(lua_State *L, std::string func_name, int x, int y)
{
	int z;

	lua_settop(L,0);
	lua_getglobal(L, func_name.c_str());

	lua_pushnumber(L, x);   // push 1st argument 
	lua_pushnumber(L, y);   // push 2nd argument 

	if (lua_pcall(L, 2, 1, 0) != 0)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << "[ LUA_fileError ] CheckFileErrors():" << errormsg << std::endl;
		return 0;
	}

	// retrieve result
	if (!lua_isnumber(L, -1))
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << "[ LUA_fileError ] CheckFileErrors():" << errormsg << std::endl;
		return 0;
	}

	z = lua_tonumber(L, -1);
	lua_pop(L, 1); // pop returned value
	lua_settop(L,0);
	return z;
}

int main(int argc, char* argv[])
{
	MRKUSED(argc);
	MRKUSED(argv);

	std::string filename = "LuaFiles.lua";
	luaLoader luaLoad(filename);
	if (luaLoad.CheckFileErrors())
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

		std::cout << "Number of elements in Table: " << luaLoad.getArrayLen("Array") << std::endl;


		//luaLoader::getLuaState() returns insance on wich it works so you can do lowlevel calls on your own.


		//Here is example lua function we want to call via function, Check function LuaLoad in this file.
		//function add(x,y)
		//	return x + y;
		//end
		// 
		std::cout << "LuaFunction(add) : " << luaAdd(luaLoad.getLuaState(),"add",2,3) << std::endl;


		//function add(x,y)
		//	return x + y;
		//end
		//Here we use luaPushStack that in witch we insert our arguments for function for example
		std::vector<luaLoader::luaPushStack> push; // vector that holds argments in order
		luaLoader::luaPushStack arg1(2.2); //first argument x
		luaLoader::luaPushStack arg2(8.0); //seccond argument y
		push.push_back(arg1); // pushing x
		push.push_back(arg2); // pushing y
		luaLoad.execFunction("add",push); // execute function
		std::cout << "LuaClassFunction(add) : " << float(luaLoad) << std::endl; // Print out resoult
		push.clear(); //Clear vector

		// Here is another example
		//function check(x)
		//	if x == true then return "trueString" else return "falseString" end
		//end
		luaLoader::luaPushStack arg3(false);
		push.push_back(arg3);
		luaLoad.execFunction("check", push);
		std::cout << "LuaClassFunction(check) : " << std::string(luaLoad) << std::endl;
		push.clear();
		
		//Here is passing function arguments via class function.
		luaLoad.pushFunctionArguments(true); // Pass argument
		luaLoad.execFunction("check"); // Execute function
		std::cout << "LuaClassFunction(check) : " << std::string(luaLoad) << std::endl; //display resoult

		//Another example
		luaLoad.pushFunctionArguments("constChar");
		luaLoad.execFunction("string");
		std::cout << "LuaClassFunction(string) : " << std::string(luaLoad) << std::endl;

	}

	// cleanup
	//lua_close(L);
	std::cin.get(); // Use this insead std::system("")
	return 0;
}
