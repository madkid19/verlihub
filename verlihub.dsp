# Microsoft Developer Studio Project File - Name="verlihub" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=verlihub - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "verlihub.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "verlihub.mak" CFG="verlihub - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "verlihub - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "verlihub - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "verlihub - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /I "D:\Program Files\Libs\unixem\include" /I "d:\mysql\include" /I "D:\Program Files\Libs\pcre\dll" /I "D:\Program Files\Libs\GeoIP-1.3.1\libGeoIP" /D "WIN32" /D "_WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "WITHOUT_GEOIP" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib ws2_32.lib libmysql.lib pcre.lib unixem.lib libgeoip.a /nologo /subsystem:console /incremental:yes /machine:I386 /nodefaultlib:"libcd.lib" /libpath:"d:\mysql\lib\opt" /libpath:"D:\Program Files\Libs\pcre\dll" /libpath:"D:\Program Files\Libs\unixem\lib" /libpath:"D:\Program Files\Libs\GeoIP-1.3.1\libGeoIP"

!ELSEIF  "$(CFG)" == "verlihub - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "verlihub___Win32_Debug"
# PROP BASE Intermediate_Dir "verlihub___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "verlihub___Win32_Debug"
# PROP Intermediate_Dir "verlihub___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /vd0 /GR /GX /ZI /I "c:\mysql\include" /I "c:\Program Files\Libs\pcre\dll" /I "c:\Program Files\Libs\unixem\include" /I "c:\Program Files\Libs\GeoIP\GeoIP-1.3.1\libGeoIP" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "NOROPE" /D "_WIN32" /D "EXPLICIT_TEMPLATES" /D "WITHOUT_GEOIP" /D "WITH_UNSTABLE" /D FD_SETSIZE=10240 /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib pcre.lib libmysql.lib unixem_vc6.debug.lib /nologo /subsystem:console /pdb:none /debug /machine:I386 /libpath:"c:\mysql\lib\opt" /libpath:"c:\Program Files\Libs\GeoIP\GeoIP-1.3.1\libGeoIP" /libpath:"c:\Program Files\Libs\pcre\dll" /libpath:"c:\Program Files\Libs\unixem\lib" /libpath:"c:\winnt\system32"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "verlihub - Win32 Release"
# Name "verlihub - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\cantiflood.cpp
# End Source File
# Begin Source File

SOURCE=.\src\casyncconn.cpp
# End Source File
# Begin Source File

SOURCE=.\src\casyncsocketserver.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cban.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cbanlist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ccallbacklist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ccmdr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ccommand.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cconfigbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cconfigfile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cconfigitembase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cconfmysql.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cconnchoose.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cconndc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cconnpoll.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cconnselect.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cdbconf.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cdcconf.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cdcconsole.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cdcproto.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cdctag.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cfreqlimiter.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cinfoserver.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cinterpolexp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ckick.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ckicklist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cmeanfrequency.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cmessagedc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cmysql.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cobj.cpp
# End Source File
# Begin Source File

SOURCE=.\src\coperator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cparser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cpcre.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cpenaltylist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cpluginbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cpluginloader.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cpluginmanager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cprotocommand.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cquery.cpp
# End Source File
# Begin Source File

SOURCE=.\src\creglist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\creglistcache.cpp
# End Source File
# Begin Source File

SOURCE=.\src\creguserinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cserverdc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\csetuplist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ctempfunctionbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ctime.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ctimeout.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ctrigger.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ctriggers.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cuser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cusercollection.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cvhplugin.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cvhpluginmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tcache.cpp
# End Source File
# Begin Source File

SOURCE=.\src\thasharray.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tpluginbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\verlihub.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\cantiflood.h
# End Source File
# Begin Source File

SOURCE=.\src\casyncconn.h
# End Source File
# Begin Source File

SOURCE=.\src\casyncsocketserver.h
# End Source File
# Begin Source File

SOURCE=.\src\cban.h
# End Source File
# Begin Source File

SOURCE=.\src\cbanlist.h
# End Source File
# Begin Source File

SOURCE=.\src\cbanlist_interface.h
# End Source File
# Begin Source File

SOURCE=.\src\ccallbacklist.h
# End Source File
# Begin Source File

SOURCE=.\src\ccmdr.h
# End Source File
# Begin Source File

SOURCE=.\src\ccommand.h
# End Source File
# Begin Source File

SOURCE=.\src\cconfigbase.h
# End Source File
# Begin Source File

SOURCE=.\src\cconfigfile.h
# End Source File
# Begin Source File

SOURCE=.\src\cconfigitembase.h
# End Source File
# Begin Source File

SOURCE=.\src\cconfmysql.h
# End Source File
# Begin Source File

SOURCE=.\src\cconnbase.h
# End Source File
# Begin Source File

SOURCE=.\src\cconnchoose.h
# End Source File
# Begin Source File

SOURCE=.\src\cconndc.h
# End Source File
# Begin Source File

SOURCE=.\src\cconnpoll.h
# End Source File
# Begin Source File

SOURCE=.\src\cconnselect.h
# End Source File
# Begin Source File

SOURCE=.\src\cdbconf.h
# End Source File
# Begin Source File

SOURCE=.\src\cdcbanlist.h
# End Source File
# Begin Source File

SOURCE=.\src\cdcconf.h
# End Source File
# Begin Source File

SOURCE=.\src\cdcconsole.h
# End Source File
# Begin Source File

SOURCE=.\src\cdcproto.h
# End Source File
# Begin Source File

SOURCE=.\src\cdctag.h
# End Source File
# Begin Source File

SOURCE=.\src\cfreqlimiter.h
# End Source File
# Begin Source File

SOURCE=.\src\cgeoip.h
# End Source File
# Begin Source File

SOURCE=.\src\cinfoserver.h
# End Source File
# Begin Source File

SOURCE=.\src\cinterpolexp.h
# End Source File
# Begin Source File

SOURCE=.\src\ckick.h
# End Source File
# Begin Source File

SOURCE=.\src\ckicklist.h
# End Source File
# Begin Source File

SOURCE=.\src\cmeanfrequency.h
# End Source File
# Begin Source File

SOURCE=.\src\cmessagedc.h
# End Source File
# Begin Source File

SOURCE=.\src\cmysql.h
# End Source File
# Begin Source File

SOURCE=.\src\cmysqltable.h
# End Source File
# Begin Source File

SOURCE=.\src\cobj.h
# End Source File
# Begin Source File

SOURCE=.\src\coperator.h
# End Source File
# Begin Source File

SOURCE=.\src\cparser.h
# End Source File
# Begin Source File

SOURCE=.\src\cpcre.h
# End Source File
# Begin Source File

SOURCE=.\src\cpenaltylist.h
# End Source File
# Begin Source File

SOURCE=.\src\cpluginbase.h
# End Source File
# Begin Source File

SOURCE=.\src\cpluginloader.h
# End Source File
# Begin Source File

SOURCE=.\src\cpluginmanager.h
# End Source File
# Begin Source File

SOURCE=.\src\cprotocommand.h
# End Source File
# Begin Source File

SOURCE=.\src\cquery.h
# End Source File
# Begin Source File

SOURCE=.\src\creglist.h
# End Source File
# Begin Source File

SOURCE=.\src\creglistcache.h
# End Source File
# Begin Source File

SOURCE=.\src\creguserinfo.h
# End Source File
# Begin Source File

SOURCE=.\src\cselector.h
# End Source File
# Begin Source File

SOURCE=.\src\cserverdc.h
# End Source File
# Begin Source File

SOURCE=.\src\csetuplist.h
# End Source File
# Begin Source File

SOURCE=.\src\ctempfunctionbase.h
# End Source File
# Begin Source File

SOURCE=.\src\ctime.h
# End Source File
# Begin Source File

SOURCE=.\src\ctimeout.h
# End Source File
# Begin Source File

SOURCE=.\src\ctrigger.h
# End Source File
# Begin Source File

SOURCE=.\src\ctriggers.h
# End Source File
# Begin Source File

SOURCE=.\src\cuser.h
# End Source File
# Begin Source File

SOURCE=.\src\cusercollection.h
# End Source File
# Begin Source File

SOURCE=.\src\cvhplugin.h
# End Source File
# Begin Source File

SOURCE=.\src\cvhpluginmgr.h
# End Source File
# Begin Source File

SOURCE=.\src\dll.h
# End Source File
# Begin Source File

SOURCE=.\src\tcache.h
# End Source File
# Begin Source File

SOURCE=.\src\tchashlistmap.h
# End Source File
# Begin Source File

SOURCE=.\src\thasharray.h
# End Source File
# Begin Source File

SOURCE=.\src\tmysqlmemorylist.h
# End Source File
# Begin Source File

SOURCE=.\src\tpluginbase.h
# End Source File
# Begin Source File

SOURCE=.\src\tstringhashmap.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
