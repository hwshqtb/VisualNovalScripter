set(src_code
        Lua/lapi.c Lua/lcode.c Lua/lctype.c Lua/ldebug.c Lua/ldo.c Lua/ldump.c Lua/lfunc.c Lua/lgc.c Lua/llex.c
        Lua/lmem.c Lua/lobject.c Lua/lopcodes.c Lua/lparser.c Lua/lstate.c Lua/lstring.c Lua/ltable.c Lua/ltm.c
        Lua/lundump.c Lua/lvm.c Lua/lzio.c)
set(src_lib
        Lua/lauxlib.c Lua/lbaselib.c Lua/lcorolib.c Lua/ldblib.c Lua/liolib.c Lua/lmathlib.c Lua/loadlib.c
        Lua/loslib.c Lua/lstrlib.c Lua/ltablib.c Lua/lutf8lib.c Lua/linit.c)

set(src_exec Lua/lua.c)
set(src_comp Lua/luac.c)

add_library(liblua_static STATIC ${src_code} ${src_lib})

set_target_properties(liblua_static PROPERTIES OUTPUT_NAME lua
    CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})