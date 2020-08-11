#include <iostream>
#include <map>
#include <string>

#ifdef __cplusplus
# include <lua5.2/lua.hpp>
#else
# include <lua5.2/lua.h>
# include <lua5.2/lualib.h>
# include <lua5.2/lauxlib.h>
#endif
class luaLoader
{
	public:
		enum L_ReturnType { NUMBER,BOOLEAN,STRING,NONE};
		enum L_ReturnError { WRONG_RETURN_DATA_TYPE,
			GLOBAL_VALUE_DOES_NOT_EXIST,
			TABLE_DOES_NOT_EXIST,
			TABLE_VALUE_DOES_NOT_EXIST,
			ARRAY_DOES_NOT_EXIST
		};
		
	private:
		struct luaMemoryStack
		{
			public:
				bool m_bool;
				float m_float;
				std::string m_str;
				L_ReturnType m_type;

		};
		luaMemoryStack m_temp;

		lua_State *L;
		std::string m_tableName;


		void clearTempMemory();
		void readValue();



		luaLoader(const luaLoader &obj);
		luaLoader(luaLoader &&obj);

	public:
		luaLoader(lua_State *L);
		void L_updateDataStack(std::pair<void*,L_ReturnType> obj);

		void readGlobalValue(const char *value);
		void readFromTable(const char *table, const char *value);
		void readFromTable(const char *table, int value);

		size_t getArrayLen(const char *ArrayName);


		//To get string after declarating it you have to use it like 
		//	luaLoader lua(L);
		//
		//	std::sting str = "";
		//	lua.getGlobalValue("stringExample");
		//	str = std::string(lua);
		//
		operator std::string()
		{
		if (m_temp.m_type == L_ReturnType::STRING)
			{
			return m_temp.m_str;
			}
		std::cout << "[luaLoader] operator std::string(): return type is not string" << std::endl;
		throw WRONG_RETURN_DATA_TYPE;
		//std::exit(0);
		}

		operator float()
		{
		if (m_temp.m_type == L_ReturnType::NUMBER)
		{
			return m_temp.m_float;
		}
		std::cout << "[luaLoader] operator float(): return type is not float" << std::endl;
		throw WRONG_RETURN_DATA_TYPE;
		//std::exit(0);
		}

		operator int()
		{
		if (m_temp.m_type == L_ReturnType::NUMBER)
		{
			int val = (int)m_temp.m_float;
			return (int)val;
		}
		std::cout << "[luaLoader] operator float(): return type is not float" << std::endl;
		throw WRONG_RETURN_DATA_TYPE;
		//std::exit(0);
		}


		operator bool()
		{
		if (m_temp.m_type == L_ReturnType::BOOLEAN)
		{
			bool val = m_temp.m_bool;
			return val;
		}
		std::cout << "[luaLoader] operator float(): return type is not bool" << std::endl;
		throw WRONG_RETURN_DATA_TYPE;
		//std::exit(0);
		}


};
