#include <iostream>
#include <map>

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


		operator std::string()
		{
		if (m_temp.m_type == L_ReturnType::STRING)
			{
			return m_temp.m_str;
			}
		std::cout << "[luaLoader] operator std::string(): return type is not string" << std::endl;
		std::exit(0);
		}

		operator float()
		{
		if (m_temp.m_type == L_ReturnType::NUMBER)
		{
			return m_temp.m_float;
		}
		std::cout << "[luaLoader] operator float(): return type is not float" << std::endl;
		std::exit(0);
		}

		operator int()
		{
		if (m_temp.m_type == L_ReturnType::NUMBER)
		{
			int val = (int)m_temp.m_float;
			return (int)val;
		}
		std::cout << "[luaLoader] operator float(): return type is not float" << std::endl;
		std::exit(0);
		}


		operator bool()
		{
		if (m_temp.m_type == L_ReturnType::BOOLEAN)
		{
			bool val = m_temp.m_bool;
			return val;
		}
		std::cout << "[luaLoader] operator float(): return type is not bool" << std::endl;
		std::exit(0);
		}


};
