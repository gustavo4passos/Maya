#ifndef LUAINTERPRETER_H
#define LUAINTERPRETER_H

#include <stack>
#include <string>

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

class LuaInterpreter
{
public:
    LuaInterpreter();

    // Returns 0 if successfully initialized
    // Returns a negative number in case of failure
    int Init();
    int Close();

    // Returns last error string and pop it from the error stack
    std::string GetError();

    // Run lua script from file
    int RunScript(const char* file, int nReturns = 0);
    int RunLine(const char* line, int nReturns = 0);
    int RegisterFunction(lua_CFunction f, const char* name);

    int GetGlobal(const char* name, int* value);
    int GetGlobal(const char* name, std::string* value);
    int GetGlobal(const char* name, double* value);
    
    inline bool WasInitialized() const { return _initialized; }
    bool IsValid() const;
    

    int PopFromStack(int n = 1);
    int ClearStack();
    
    // Helper: prints stack content
    void StackDump();

private:
    lua_State* L;
    bool _initialized;
    std::stack<std::string> _errorStack;

    int CheckInitialization();
    void PushError(const char* erorrMessage);
    void PushErrorFromTopOfStack();

    // Pushes global onto stack and returns true, if global exists
    // Returns false and pops NIL from top of stack otherwise
    bool PushGlobalOntoStack(const char* name);
};

#endif