#include "luaLoader.h"

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

void luaLoader::readFromTable(const char *table, int value )
{
	#if DEBUG
	std::cout << "readFormTable(lua_State*, const char*) Looking For" << table << "[" << value << "]" << std::endl;
	#endif
	lua_getglobal(L,table);

	if (lua_istable(L,-1))
	{
		lua_pushinteger(L,value);
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

void luaLoader::execFunction(std::string func_name,std::vector<luaPushStack> &push )
	{

		lua_settop(L,0);
		lua_getglobal(L, func_name.c_str());
		// Push Arguments
		for (size_t i = 0; i < push.size(); i++)
		{
			switch (push[i].tag)
			{
				case L_ReturnType::BOOLEAN:
					lua_pushboolean(this->L,push[i].boolStack);
					//std::cout << "Pushed " << push[i].boolStack << std::endl;
					break;
				case L_ReturnType::NUMBER:
					lua_pushnumber(this->L,(double)push[i].doubleStack);
					//std::cout << "Pushed " << push[i].doubleStack << std::endl;
					break;
				case L_ReturnType::STRING:
					lua_pushstring(this->L,push[i].stringStack);
					break;
				case L_ReturnType::NONE:
					break;
				default:
					std::cout << "Problem" << std::endl;

			}
		}
	//Call lua file function
	if (lua_pcall(L, push.size(), 1, 0) != 0)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << "[ Lua ] FUNCTION_ERROR CheckFileErrors():" << errormsg << std::endl;
		throw FUNCTION_ERROR;
		return;
	}

	// retrieve result
	readValue();
	}

void luaLoader::execFunction(std::string func_name)
{
		lua_settop(L,0);
		lua_getglobal(L, func_name.c_str());
		// Push Arguments
		for (size_t i = 0; i < m_functionArguments.size(); i++)
		{
			switch (m_functionArguments[i].tag)
			{
				case L_ReturnType::BOOLEAN:
					lua_pushboolean(this->L,m_functionArguments[i].boolStack);
					//std::cout << "Pushed " << push[i].boolStack << std::endl;
					break;
				case L_ReturnType::NUMBER:
					lua_pushnumber(this->L,(double)m_functionArguments[i].doubleStack);
					//std::cout << "Pushed " << push[i].doubleStack << std::endl;
					break;
				case L_ReturnType::STRING:
					lua_pushstring(this->L,m_functionArguments[i].stringStack);
					break;
				case L_ReturnType::NONE:
					break;
				default:
					std::cout << "Problem" << std::endl;
			}
		}
	//Call lua file function
	if (lua_pcall(L, m_functionArguments.size(), 1, 0) != 0)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << "[ Lua ] FUNCTION_ERROR CheckFileErrors():" << errormsg << std::endl;
		throw FUNCTION_ERROR;
		return;
	}

	// retrieve result
	readValue();
	clearFunctionArguments();
}

bool luaLoader::CheckFileErrors()
{
	int r = luaL_dofile(L,m_filename.c_str());
	if(r != LUA_OK)
	{
	std::string errormsg = lua_tostring(L, -1);
	std::cout << "[ LUA_fileError ] CheckFileErrors():" << errormsg << std::endl;
	return false;
	}
	return true;
}


