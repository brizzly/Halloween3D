# Microsoft Developer Studio Project File - Name="projet_vc6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=projet_vc6 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "projet_vc6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "projet_vc6.mak" CFG="projet_vc6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "projet_vc6 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "projet_vc6 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "projet_vc6 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../"
# PROP Intermediate_Dir "temp_obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib openplay.lib dxguid.lib dinput.lib bass.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../../../Halloween999.exe"

!ELSEIF  "$(CFG)" == "projet_vc6 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../"
# PROP Intermediate_Dir "temp_obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"projet_vc6.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib openplay.lib dxguid.lib dinput.lib bass.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /out:"../../../Halloween999.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "projet_vc6 - Win32 Release"
# Name "projet_vc6 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\alloween.cpp
# End Source File
# Begin Source File

SOURCE=..\bspfile.cpp
# End Source File
# Begin Source File

SOURCE=..\bsptree.cpp
# End Source File
# Begin Source File

SOURCE=..\camera.cpp
# End Source File
# Begin Source File

SOURCE=..\collision.cpp
# End Source File
# Begin Source File

SOURCE=..\console.cpp
# End Source File
# Begin Source File

SOURCE=..\demo.cpp
# End Source File
# Begin Source File

SOURCE=..\display.cpp
# End Source File
# Begin Source File

SOURCE=..\dsound.cpp
# End Source File
# Begin Source File

SOURCE=..\ellipsoide.cpp
# End Source File
# Begin Source File

SOURCE=..\enveffect.cpp
# End Source File
# Begin Source File

SOURCE=..\filesys.cpp
# End Source File
# Begin Source File

SOURCE=..\frustrum.cpp
# End Source File
# Begin Source File

SOURCE=..\game.cpp
# End Source File
# Begin Source File

SOURCE=..\gameover.cpp
# End Source File
# Begin Source File

SOURCE=..\halloween.cpp
# End Source File
# Begin Source File

SOURCE=..\image.cpp
# End Source File
# Begin Source File

SOURCE=..\imageloader_win.cpp
# End Source File
# Begin Source File

SOURCE=..\input_win.cpp
# End Source File
# Begin Source File

SOURCE=..\interface.cpp
# End Source File
# Begin Source File

SOURCE=..\jpeg.cpp
# End Source File
# Begin Source File

SOURCE=..\libbass.cpp
# End Source File
# Begin Source File

SOURCE=..\liste.cpp
# End Source File
# Begin Source File

SOURCE=..\maths.cpp
# End Source File
# Begin Source File

SOURCE=..\menu.cpp
# End Source File
# Begin Source File

SOURCE=..\menu_init.cpp
# End Source File
# Begin Source File

SOURCE=..\menu_sel.cpp
# End Source File
# Begin Source File

SOURCE=..\monster.cpp
# End Source File
# Begin Source File

SOURCE=..\mskmodel.cpp
# End Source File
# Begin Source File

SOURCE=..\net_client.cpp
# End Source File
# Begin Source File

SOURCE=..\net_common.cpp
# End Source File
# Begin Source File

SOURCE=..\net_dcomp.cpp
# End Source File
# Begin Source File

SOURCE=..\net_server.cpp
# End Source File
# Begin Source File

SOURCE=..\objet.cpp
# End Source File
# Begin Source File

SOURCE=..\objet_proc.cpp
# End Source File
# Begin Source File

SOURCE=..\opengl.cpp
# End Source File
# Begin Source File

SOURCE=..\physic.cpp
# End Source File
# Begin Source File

SOURCE=..\player.cpp
# End Source File
# Begin Source File

SOURCE=..\script.cpp
# End Source File
# Begin Source File

SOURCE=..\shader.cpp
# End Source File
# Begin Source File

SOURCE=..\sorting.cpp
# End Source File
# Begin Source File

SOURCE=..\story.cpp
# End Source File
# Begin Source File

SOURCE=..\textdraw.cpp
# End Source File
# Begin Source File

SOURCE=..\tga.cpp
# End Source File
# Begin Source File

SOURCE=..\timer.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\coldet.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\alloween.h
# End Source File
# Begin Source File

SOURCE=..\bspfile.h
# End Source File
# Begin Source File

SOURCE=..\bsptree.h
# End Source File
# Begin Source File

SOURCE=..\camera.h
# End Source File
# Begin Source File

SOURCE=..\collision.h
# End Source File
# Begin Source File

SOURCE=..\console.h
# End Source File
# Begin Source File

SOURCE=..\demo.h
# End Source File
# Begin Source File

SOURCE=..\dirlib.h
# End Source File
# Begin Source File

SOURCE=..\display.h
# End Source File
# Begin Source File

SOURCE=..\dsound.h
# End Source File
# Begin Source File

SOURCE=..\ellipsoide.h
# End Source File
# Begin Source File

SOURCE=..\enveffect.h
# End Source File
# Begin Source File

SOURCE=..\filesys.h
# End Source File
# Begin Source File

SOURCE=..\frustrum.h
# End Source File
# Begin Source File

SOURCE=..\game.h
# End Source File
# Begin Source File

SOURCE=..\gameover.h
# End Source File
# Begin Source File

SOURCE=..\halloween.h
# End Source File
# Begin Source File

SOURCE=..\image.h
# End Source File
# Begin Source File

SOURCE=..\imageloader_win.h
# End Source File
# Begin Source File

SOURCE=..\input_win.h
# End Source File
# Begin Source File

SOURCE=..\interface.h
# End Source File
# Begin Source File

SOURCE=..\jpeg.h
# End Source File
# Begin Source File

SOURCE=..\libbass.h
# End Source File
# Begin Source File

SOURCE=..\liste.h
# End Source File
# Begin Source File

SOURCE=..\maths.h
# End Source File
# Begin Source File

SOURCE=..\menu.h
# End Source File
# Begin Source File

SOURCE=..\menu_init.h
# End Source File
# Begin Source File

SOURCE=..\menu_sel.h
# End Source File
# Begin Source File

SOURCE=..\monster.h
# End Source File
# Begin Source File

SOURCE=..\mskmodel.h
# End Source File
# Begin Source File

SOURCE=..\net_client.h
# End Source File
# Begin Source File

SOURCE=..\net_common.h
# End Source File
# Begin Source File

SOURCE=..\net_dcomp.h
# End Source File
# Begin Source File

SOURCE=..\net_server.h
# End Source File
# Begin Source File

SOURCE=..\objet.h
# End Source File
# Begin Source File

SOURCE=..\objet_proc.h
# End Source File
# Begin Source File

SOURCE=..\opengl.h
# End Source File
# Begin Source File

SOURCE=..\physic.h
# End Source File
# Begin Source File

SOURCE=..\player.h
# End Source File
# Begin Source File

SOURCE=..\script.h
# End Source File
# Begin Source File

SOURCE=..\shader.h
# End Source File
# Begin Source File

SOURCE=..\sorting.h
# End Source File
# Begin Source File

SOURCE=..\story.h
# End Source File
# Begin Source File

SOURCE=..\textdraw.h
# End Source File
# Begin Source File

SOURCE=..\tga.h
# End Source File
# Begin Source File

SOURCE=..\timer.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\misc\ressource.rc
# End Source File
# End Group
# End Target
# End Project
