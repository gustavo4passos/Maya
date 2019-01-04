#include "../include/LuaInterpreter.h"

LuaInterpreter::LuaInterpreter()
:   L(nullptr),
    _initialized(false)
{ }

int LuaInterpreter::Init()
{
    // If already initialized, do nothing
    if(WasInitialized()) return 0;

    // Create a new Lua thread
    L = luaL_newstate();
    // If unable to create a state (due the lack of memory), push the error message onto the stack
    if(L == nullptr)
    {
        PushError("Unable to create the Lua State.");
        _initialized = false;
        return -1;
    }

    luaL_openlibs(L);
    
    _initialized = true;
    return 0;
}

int LuaInterpreter::Close()
{
    if(CheckInitialization() < 0) return -1;

    lua_close(L);
    L = nullptr;
    _initialized = false;
    return 0;
}

std::string LuaInterpreter::GetError()
{
    if(_errorStack.empty()) return "";

    std::string errorMessage = !_errorStack.empty() ? _errorStack.top() : "";
    _errorStack.pop();

    return errorMessage;
}

int LuaInterpreter::RunScript(const char* filename, int nReturns)
{
    if(CheckInitialization() < 0) return -1;

    int status = luaL_loadfile(L, filename);

    if(status != LUA_OK)
    {
        PushErrorFromTopOfStack();
        return -1;
    }

    status = lua_pcall(L, 0, nReturns, 0);

    if(status != LUA_OK)
    {
        PushErrorFromTopOfStack();
        return -1;
    }

    return 0;
}

int LuaInterpreter::RunLine(const char* line, int nReturns)
{
    if(CheckInitialization() < 0) return -1;

    int status = luaL_loadstring(L, line);

    if(status != LUA_OK)
    {
        PushErrorFromTopOfStack();
        return -1;
    }

    status = lua_pcall(L, 0, nReturns, 0);

    if(status != LUA_OK)
    {
        PushErrorFromTopOfStack();
        return -1;
    }

    return 0;
}

int LuaInterpreter::RegisterFunction(lua_CFunction f, const char* name)
{
    if(CheckInitialization() < 0) return -1;

    lua_pushcfunction(L, f);
    lua_setglobal(L, name);   
    return 0;
}

int LuaInterpreter::GetGlobal(const char* name, int* value)
{
    if(CheckInitialization() < 0) return -1;

    if(PushGlobalOntoStack(name) == false)
    {
        PushError("Global does not exist.");
        return -1;
    }

    int isnum;
    *value = lua_tointegerx(L, -1, &isnum);
    lua_pop(L, 1);

    if(isnum == false)
    {
        PushError(std::string("Invalid type: ").append(name).append(" is a ").append(lua_typename(L, lua_type(L, -1))).c_str());
        *value = 0;
        return -1;
    }

    return 0;
}

int LuaInterpreter::GetGlobal(const char* name, std::string* value)
{
    if(CheckInitialization() < 0) return -1;

    if(PushGlobalOntoStack(name) == false)
    {
        PushError("Global does not exist.");
        return -1;
    }

    if(lua_isstring(L, -1) == false)
    {
        PushError(std::string("Invalid type: ").append(name).append(" is a ").append(lua_typename(L, lua_type(L, -1))).c_str());
        lua_pop(L, 1);
        return -1;
    }

    *value = std::string(lua_tolstring(L, -1, nullptr));
    lua_pop(L, 1);
    return 0;
}

bool LuaInterpreter::IsValid() const
{
    if(!WasInitialized()) return false;
    if(L == nullptr) return false;

    return true;
}
int LuaInterpreter::PopFromStack(int n)
{
    if(CheckInitialization() < 0) return -1;

    lua_pop(L, n);
    return 0;
}

int LuaInterpreter::ClearStack()
{
    if(CheckInitialization() < 0) return -1;

    lua_settop(L, 0);
    return 0;
}

int LuaInterpreter::CheckInitialization()
{
    if(WasInitialized() == false)
    {
        PushError("The interpreter has not been initialized.");
        return -1;
    }

    return 0;
}

void LuaInterpreter::PushError(const char* errorMessage)
{
    _errorStack.push(std::string("Error: ").append(errorMessage));
}

void LuaInterpreter::PushErrorFromTopOfStack()
{
    PushError(lua_tostring(L, -1));
    lua_pop(L, 1);
}


bool LuaInterpreter::PushGlobalOntoStack(const char* name)
{
    lua_getglobal(L, name);
    if(lua_type(L, -1) == LUA_TNIL)
    {
        lua_pop(L, 1);
        return false;
    }

    return true;
}

#include <stdio.h>
void LuaInterpreter::StackDump()
{
    int stackSize = lua_gettop(L);

    printf("[STACK DUMP]\n");
    if(stackSize == 0) printf("Stack is empty.\n");

    for(int i = 1; i <= stackSize; i++)
    {
        int type = lua_type(L, i);

        printf("%i ", i);
        switch(type)
        {
            case LUA_TNIL:
                printf("[NIL]\n");
                break;
            
            case LUA_TNUMBER:
                printf("[NUMBER] %lf\n", lua_tonumber(L, i));
                break;
            
            case LUA_TBOOLEAN:
                printf("[BOOLEAN] %s\n", lua_toboolean(L, i) == true ? "true" : "false");
                break;
            
            case LUA_TSTRING:
                printf("[STRING] %s\n", lua_tolstring(L, i, nullptr));
                break;
            
            default:
                printf("[OTHERTYPE] %s\n", lua_typename(L, type));
                break;
        }
    }
}