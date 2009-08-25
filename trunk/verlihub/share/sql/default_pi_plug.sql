INSERT IGNORE INTO pi_plug (nick,autoload,path,detail) VALUES
	("isp"    ,0,"/usr/local/lib/libisp_pi.so"   ,"Advanced ISP settings"),
	("lua"    ,0,"/usr/local/lib/liblua_pi.so"   ,"Support for lua scripts"),
	("perl"   ,0,"/usr/local/lib/libperl_pi.so"  ,"Support for perl scripts"),
	("msg"    ,0,"/usr/local/lib/libmessanger_pi.so","Offline messages"),
	("flood"  ,0,"/usr/local/lib/libfloodprot_pi.so","Flood protection"),
	("log"    ,0,"/usr/local/lib/libiplog_pi.so","Log plugin"),
	("forbid" ,0,"/usr/local/lib/libforbid_pi.so","Advanced chat filter"),
	("chat"   ,0,"/usr/local/lib/libchatroom_pi.so","Multiple chatrooms"),
	("replace",0,"/usr/local/lib/libreplace_pi.so","Word replacer"),
	("stats"  ,0,"/usr/local/lib/libstats_pi.so","Statistics plugin")
