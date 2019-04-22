#include "../include/LuaScript.h"

#include <iostream>

LuaScript::LuaScript(const std::string& filename) {
  L = luaL_newstate();
  luaL_openlibs(L);
  if(luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
    std::cout << "Error: unable to load[" << filename << "]" << std::endl;
    L = nullptr;
  }
}

LuaScript::~LuaScript() {
  if(L) lua_close(L);
}

void LuaScript::PrintError(const std::string& variableName, const std::string& reason) {
  std::cout << "Unable to get [" << variableName << "]. " << reason << '\n';
}

bool LuaScript::LuaGetToStack(const std::string& variableName) {
  this->_level = 0;
  std::string var = "";
  for(unsigned i = 0; i < variableName.size(); i++) {
    if(variableName.at(i) == '.'){
      if(this->_level == 0) {
        lua_getglobal(L, var.c_str());
      }
      else {
        lua_getfield(L, -1, var.c_str());
      }

      if(lua_isnil(L, -1)){
        PrintError(variableName, var + " is not defined");
        return false;
      }
      else {
        var = "";
        this->_level++;
      }
    }
    else {
      var += variableName.at(i);
    }
  }
  if(this->_level == 0) lua_getglobal(L, var.c_str());
  else lua_getfield(L, -1, var.c_str());

  if(lua_isnil(L, -1)) {
    PrintError(variableName, var + " is not defined");
    return false;
  }

  return true;
}

void LuaScript::CleanStack() {
  if(L) lua_settop(L, 0);
}

void LuaScript::StackDump() {
  if(L) {
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {
      int t = lua_type(L, i);
      switch(t){
        case LUA_TSTRING:
          std::cout << lua_tostring(L, i) << std::endl;
          break;
        case LUA_TBOOLEAN:
          std::cout << (((bool)(lua_toboolean(L, i)) == true) ? "true" : "false") << std::endl;
          break;
        case LUA_TNUMBER:
          std::cout << lua_tonumber(L, i) << std::endl;
          break;
        default:
          std::cout << lua_typename(L, i) << std::endl;
          break;
      }
    }
  }
}
