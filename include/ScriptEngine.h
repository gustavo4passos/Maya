#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <fstream>
#include <queue>
#include <stack>

class LuaInterpreter;

class ScriptEngine
{
public:
    ScriptEngine(LuaInterpreter* luaInterpreter);

    // Enqueue script for future execution
    // Returns a negative code in case of error
    // Returns 0 otherwise
    int EnqueueScript(const char* file);

    // Run active scripts
    // Returns a negative code in case of error
    // Returns 0 otherwise
    int Update(int elapsedTime);

    // Set wait time until next line of code is executed
    void Wait(unsigned duration) { _waitTime = duration; }

    inline int NumberOfActiveScripts() const { return _activeScripts.size(); }

    // Return last captured error and pop it from error stack
    std::string GetError();

private:
    LuaInterpreter* _luaInterpreter;
    std::stack<std::string> _errorStack;
    std::queue<std::ifstream> _activeScripts;
    int _waitTime;
};

#endif