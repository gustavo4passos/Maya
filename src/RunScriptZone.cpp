#include "../include/RunScriptZone.h"

#include "../include/ScriptEngine.h"

RunScriptZone::RunScriptZone(const Rect& bounds, const std::string& scriptFilename)
:   Zone(bounds), 
    _scriptFilename(scriptFilename)
{ }

