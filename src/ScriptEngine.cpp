#include "../include/ScriptEngine.h"

#include <iostream>
#include <string>

#include "../include/LuaInterpreter.h"


ScriptEngine::ScriptEngine(LuaInterpreter* luaInterpreter)
:   _luaInterpreter(luaInterpreter),
    _waitTime(0)
{ }

int ScriptEngine::EnqueueScript(const char* file)
{
    std::ifstream script(file);
    if(!script.good())
    {
        _errorStack.push(std::string("ERROR: Invalid script - ").append(file));
        return -1;
    }

    _activeScripts.push(std::ifstream(file));
    return 0;
}

std::string ScriptEngine::GetError()
{
    if(!_errorStack.empty())
    {
        std::string error = _errorStack.top();
        _errorStack.pop();
        return error;
    }

    return "";
}
int ScriptEngine::Update(int elapsedTime)
{
    _waitTime -= elapsedTime;

    if(_waitTime <= 0)
    {
        _waitTime = 0;

        while(!_activeScripts.empty() && _waitTime <= 0)
        {
            std::string line;
            std::getline(_activeScripts.front(), line);

            if(_luaInterpreter == nullptr || !_luaInterpreter->IsValid())
            {
                _errorStack.push("ERROR: Invalid or null Lua Intepreter.");
            }
            if(_luaInterpreter->RunLine(line.c_str()) < 0)
            {
                _errorStack.push(_luaInterpreter->GetError());
                return -1;
            }

            if(_activeScripts.front().eof()) _activeScripts.pop();
        }
    }

    return 0;
}