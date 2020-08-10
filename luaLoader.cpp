#include "luaLoader.h"
#include <lua5.2/lauxlib.h>
#include <lua5.2/lua.h>

luaLoader::luaLoader(lua_State *L)
{
	std::cout << "[luaLoader] luaLoader(lua_State *L)" << std::endl;
	this->L = L;
	
	m_temp.m_type = L_ReturnType::NONE;
}


size_t luaLoader::getArrayLen(const char *ArrayName)
{
	lua_getglobal(L,ArrayName);
	if(lua_istable(L,-1))
	{
		return lua_rawlen(L,-1);
		
	}
	else
		throw L_ReturnError::ARRAY_DOES_NOT_EXIST;
}

void luaLoader::readFromTable(const char *table, const char *value )
{
	#if DEBUG
	std::cout << "readFormTable(lua_State*, const char*) Looking For" << table << "[" << value << "]" << std::endl;
	#endif
	lua_getglobal(L,table);

	if (lua_istable(L,-1))
	{
		lua_pushstring(L,value);
		lua_gettable(L, -2);
		if (lua_isnoneornil(L,-1))
			throw TABLE_VALUE_DOES_NOT_EXIST;
		readValue();
		lua_pop(L,1);
	}
	else 
		throw TABLE_DOES_NOT_EXIST;

}

void luaLoader::readGlobalValue(const char *value)
{
	#if DEBUG
	std::cout << "readGlobalValue(lua_State*, const char*) Looking For" << value;
	#endif
	lua_getglobal(L,value);
	if (lua_isnoneornil(L,-1))
		throw GLOBAL_VALUE_DOES_NOT_EXIST;
	readValue();
}

void luaLoader::readValue()
{

	#if DEBUG
	std::cout << "luaLoader::readValue()";
	#endif

	if(lua_isnumber(L,-1))
	{
		float value = lua_tonumber(L,-1);
		m_temp.m_float = value;
		m_temp.m_type = L_ReturnType::NUMBER;
		#if DEBUG
			std::cout << "[LUA] readValue(): NUMBER " << value << std::endl;
		#endif
	}

	else if(lua_isstring(L,-1))
	{
		std::string value = lua_tostring(L,-1);
		m_temp.m_str = value;
		m_temp.m_type = L_ReturnType::STRING;
		#if DEBUG
			std::cout << "[LUA] readValue(): STRING " << value << std::endl;
		#endif
	}

	else if(lua_isboolean(L,-1))
	{
		bool value = lua_toboolean(L,-1);
		m_temp.m_bool = value;
		m_temp.m_type = L_ReturnType::BOOLEAN;
		#if DEBUG
			std::cout << "[LUA] readValue(): BOOLEAN " << value << std::endl;
		#endif
	}
}

