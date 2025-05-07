#include "luahelper.h"
#include <QtWidgets/QMainWindow>

class LuaMainWindow: public QMainWindow {
public:
    LuaMainWindow():
        L(luaL_newstate()) {
        
    }

private:
    lua_State* L;

};