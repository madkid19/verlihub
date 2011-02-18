SET(PLUGMAN_FOUND TRUE)
ADD_PLUGIN(PLUGMAN_FOUND plugman)
MACRO_LOG_FEATURE(PLUGMAN_FOUND "plugman" "Pluging Manager support")

# Find dependences
Find_Package(Lua51)
#Find_Package(LuaSocket)
ADD_PLUGIN(LUA51_FOUND lua)

MACRO_LOG_FEATURE(LUA51_FOUND "lua" "Lua Plugin for lua scripting" "http://www.lua.org" "5.1" "Lua libray NOT found")