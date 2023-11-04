#include <iostream>
#include <map>
#include <string>
#include <vector>

#ifdef __cplusplus
# include <lua5.3/lua.hpp>
#else
# include <lua5.3/lua.h>
# include <lua5.3/lualib.h>
# include <lua5.3/lauxlib.h>
#endif

class luaLoader
{
	public:
		enum L_ReturnType { NUMBER,BOOLEAN,STRING,NONE};
		enum L_ReturnError { 
			WRONG_RETURN_DATA_TYPE,
			GLOBAL_VALUE_DOES_NOT_EXIST,
			TABLE_DOES_NOT_EXIST,
			TABLE_VALUE_DOES_NOT_EXIST,
			ARRAY_DOES_NOT_EXIST,
			NO_RETURN_DATA_TYPE,
			FUNCTION_ERROR
		};

		struct luaPushStack
		{
			L_ReturnType tag;
			union 
			{
				bool boolStack;
				int intStack;
				//float floatStack;
				double doubleStack;
				const char *stringStack;
			};

			luaPushStack(bool b)
			{
				tag=L_ReturnType::BOOLEAN;
				boolStack=b;

			};

			luaPushStack(int i)
			{
				tag=L_ReturnType::NUMBER;
				doubleStack=i;

			};

			luaPushStack(float f)
			{
				tag=L_ReturnType::NUMBER;
				doubleStack=f;

			};
			luaPushStack(double d)
			{
				tag=L_ReturnType::NUMBER;
				doubleStack=d;

			};
			luaPushStack(std::string str) : stringStack{str.c_str()}
			{
				tag=L_ReturnType::STRING;
				//u.stringStack=str.c_str();
			}
			luaPushStack(const char str[]) : stringStack{str}
			{
				tag=L_ReturnType::STRING;
				//u.stringStack=str.c_str();
			}
			~luaPushStack()
			{
			}
		};
		
	private:
		bool m_classCreated = 0; // Is lua instance crated by class. This has use for deconstructor to know that should it close 
								 // its instance.
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
		std::string m_filename;

		std::vector<luaPushStack> m_functionArguments;


		void clearTempMemory();
		void readValue();


		luaLoader(const luaLoader &obj);
		luaLoader(luaLoader &&obj);
		luaLoader();


	public:
		~luaLoader()
		{
			if (m_classCreated)
			{
			lua_close(L);
			}
		}
		luaLoader(lua_State *L);

		luaLoader(std::string filename) : m_classCreated{true}
		{
			m_filename = filename;
			L = luaL_newstate(); //init new luastate
			luaL_openlibs(L);
		}
		void L_updateDataStack(std::pair<void*,L_ReturnType> obj);

		void readGlobalValue(const char *value);
		void readFromTable(const char *table, const char *value);
		void readFromTable(const char *table, int value);
		size_t getArrayLen(const char *ArrayName);
		void execFunction(std::string func_name,std::vector<luaPushStack> &push);
		void execFunction(std::string func_name);

		bool CheckFileErrors();
		lua_State* getLuaState()
		{
			return L;
		}

		
		void clearFunctionArguments()
		{
			m_functionArguments.clear();
		}

		void pushFunctionArguments(bool b)
		{
			m_functionArguments.push_back(luaPushStack(b));
		}
		void pushFunctionArguments(int i)
		{
			m_functionArguments.push_back(luaPushStack(i));
		}
		void pushFunctionArguments(double d)
		{
			m_functionArguments.push_back(luaPushStack(d));
		}
		void pushFunctionArguments(std::string str)
		{
			m_functionArguments.push_back(luaPushStack(str));
			//<++>
		}
		void pushFunctionArguments(const char str[])
		{
			m_functionArguments.push_back(luaPushStack(str));
		}


		

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
				m_temp.m_type = L_ReturnType::NONE;
				return m_temp.m_str;
			}
		else if (m_temp.m_type == L_ReturnType::NONE)
			{
				std::cout << "[ LuaLoader] operator string(): NO_RETURN_DATA_TYPE" << std::endl;
				throw NO_RETURN_DATA_TYPE;
			}
		std::cout << "[luaLoader] operator std::string(): return type is not string" << std::endl;
		throw WRONG_RETURN_DATA_TYPE;
		//std::exit(0);
		}

		operator float()
		{
		if (m_temp.m_type == L_ReturnType::NUMBER)
		{
			m_temp.m_type = L_ReturnType::NONE;
			return m_temp.m_float;
		}
		else if (m_temp.m_type == L_ReturnType::NONE)
		{
			std::cout << "[ LuaLoader] operator float(): NO_RETURN_DATA_TYPE" << std::endl;
			throw NO_RETURN_DATA_TYPE;
		}
		std::cout << "[luaLoader] operator float(): return type is not float" << std::endl;
		throw WRONG_RETURN_DATA_TYPE;
		//std::exit(0);
		}

		operator int()
		{
		if (m_temp.m_type == L_ReturnType::NUMBER)
		{

			m_temp.m_type = L_ReturnType::NONE;
			int val = (int)m_temp.m_float;
			return (int)val;
		}
		else if (m_temp.m_type == L_ReturnType::NONE)
			{
				std::cout << "[ LuaLoader] operator int(): NO_RETURN_DATA_TYPE" << std::endl;
				throw NO_RETURN_DATA_TYPE;
			}
		std::cout << "[luaLoader] operator int(): return type is not int" << std::endl;
		throw WRONG_RETURN_DATA_TYPE;
		//std::exit(0);
		}

		operator bool()
		{
		if (m_temp.m_type == L_ReturnType::BOOLEAN)
		{
			m_temp.m_type = L_ReturnType::NONE;
			bool val = m_temp.m_bool;
			return val;
		}
		else if (m_temp.m_type == L_ReturnType::NONE)
			{
				std::cout << "[ LuaLoader] operator bool(): NO_RETURN_DATA_TYPE" << std::endl;
				throw NO_RETURN_DATA_TYPE;
			}
		std::cout << "[luaLoader] operator bool(): return type is not bool" << std::endl;
		throw WRONG_RETURN_DATA_TYPE;
		//std::exit(0);
		}


};
