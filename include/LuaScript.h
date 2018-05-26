#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <string>
#include <vector>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class LuaScript {
public:
  LuaScript(const std::string& filename);
  ~LuaScript();

  void PrintError(const std::string& variableName, const std::string& reason);

  // Returns the value of a key
  template<typename T>
  T Get(const std::string& variableName) {
    if(!L) {
      PrintError(variableName, "Script is not loaded");
      return LuaGetDefault<T>();
    }

    T result;
    if(LuaGetToStack(variableName)){
      result = LuaGet<T>(variableName);
    }
    else {
      result = LuaGetDefault<T>();
    }

    CleanStack();
    _level = 0;
    return result;
  }

  // Returns all fields of an array
  template<typename T>
  std::vector<T> GetArray(const std::string& variableName) {
    if(!L) {
      PrintError(variableName, "Script is not loaded");
      return std::vector<T>();
    }

    lua_getglobal(L, variableName.c_str());

    if(lua_isnil(L, -1)) {
      PrintError(variableName, "Variable is not defined.");
      return std::vector<T>();
    }

    std::vector<T> data;
    lua_pushnil(L);
    while(lua_next(L, -2)) {
      data.push_back(GetTopOfStack<T>());
      lua_pop(L, 1);
    }

    CleanStack();

    return data;
  }

private:
  bool LuaGetToStack(const std::string& variableName);

  void CleanStack();
  void StackDump();

  template<typename T>
  T LuaGet(const std::string& variableName){
    return 0;
  }

  template<typename T>
  T LuaGetDefault() {
    return 0;
  }

  template<typename T>
  std::vector<T> LuaGetArray(const std::string& variableName) {
    std::vector<T> empty;
    return empty;
  }

  template<typename T>
  T GetTopOfStack() {
    return 0;
  }

  lua_State* L;
  int _level;
};

// Template specifications for LuaGet

template<>
inline std::string LuaScript::LuaGetDefault() {
  return "null";
}

template<>
inline bool LuaScript::LuaGet(const std::string& variableName) {
  return (bool)lua_toboolean(L, -1);
}

template<>
inline float LuaScript::LuaGet(const std::string& variableName) {
  if(!lua_isnumber(L, -1)){
    PrintError(variableName, "Not a number");
  }
  return (float)lua_tonumber(L, -1);
}

template<>
inline int LuaScript::LuaGet(const std::string& variableName) {
  if(!lua_isnumber(L, -1)) {
    PrintError(variableName, "Not a number");
  }
  return (int)lua_tonumber(L, -1);
}

template<>
inline std::string LuaScript::LuaGet(const std::string& variableName) {
  std::string s = "null";
  if(lua_isstring(L, -1)) {
    s = std::string(lua_tostring(L, -1));
  }
  else {
    PrintError(variableName, "Not a string");
  }

  return s;
}

template<>
inline int LuaScript::GetTopOfStack() {
  return (int)lua_tonumber(L, -1);
}

template<>
inline float LuaScript::GetTopOfStack() {
  return (float)lua_tonumber(L, -1);
}

template<>
inline std::string LuaScript::GetTopOfStack() {
  return std::string(lua_tostring(L, -1));
}

#endif
