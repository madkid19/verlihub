SET(ENABLE_PLUGIN TRUE)
ADD_PLUGIN(ENABLE_PLUGIN plugman)
MACRO_LOG_FEATURE(ENABLE_PLUGIN "plugman" "Pluging Manager support")

# Lua plugin
Find_Package(Lua51)
#Find_Package(LuaSocket)
ADD_PLUGIN(LUA51_FOUND lua)
MACRO_LOG_FEATURE(LUA51_FOUND "lua" "Lua Plugin for lua scripting" "http://www.lua.org" "5.1" "Lua libray NOT found")

# Python plugin
Find_Package(PythonLibs)
ADD_PLUGIN(PYTHONLIBS_FOUND python)
MACRO_LOG_FEATURE(PYTHONLIBS_FOUND "python" "Python Plugin for python scripting" "http://www.python.org" "2.4" "Python libray NOT found")

ADD_PLUGIN(ENABLE_PLUGIN forbid)
MACRO_LOG_FEATURE(ENABLE_PLUGIN "forbid" "Filter main chat and private messages for forbidden words")

ADD_PLUGIN(ENABLE_PLUGIN chatroom)
MACRO_LOG_FEATURE(ENABLE_PLUGIN "chatroom" "Create chatrooms for users")

ADD_PLUGIN(ENABLE_PLUGIN iplog)
MACRO_LOG_FEATURE(ENABLE_PLUGIN "iplog" "Log IP addresses of users who join and leave the hub")

ADD_PLUGIN(ENABLE_PLUGIN isp)
MACRO_LOG_FEATURE(ENABLE_PLUGIN "isp" "Check connection types, nickname prefixes, shares, etc. on ISP")

ADD_PLUGIN(ENABLE_PLUGIN messenger)
MACRO_LOG_FEATURE(ENABLE_PLUGIN "messenger" "Send messages to offline users")

ADD_PLUGIN(ENABLE_PLUGIN stats)
MACRO_LOG_FEATURE(ENABLE_PLUGIN "stats" "Save statistics about users, share, upload, searching, etc.")

ADD_PLUGIN(ENABLE_PLUGIN replacer)
MACRO_LOG_FEATURE(ENABLE_PLUGIN "replacer" "Replace given patterns in text messages")
