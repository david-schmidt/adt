# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=ADT - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to ADT - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ADT - Win32 Release" && "$(CFG)" != "ADT - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Adt.mak" CFG="ADT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ADT - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ADT - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "ADT - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "ADT - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Adt.exe"

CLEAN : 
	-@erase ".\Release\Adt.exe"
	-@erase ".\Release\ADT.obj"
	-@erase ".\Release\Adt.pch"
	-@erase ".\Release\DiskXfer.obj"
	-@erase ".\Release\comm.obj"
	-@erase ".\Release\ADTDlg.obj"
	-@erase ".\Release\ringbuf.obj"
	-@erase ".\Release\StdAfx.obj"
	-@erase ".\Release\ADT.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Oi /Oy /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT CPP /Ox /Oa /Ow /Og /Os
CPP_PROJ=/nologo /MD /W3 /GX /Ot /Oi /Oy /Ob2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Adt.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ADT.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Adt.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Adt.pdb" /machine:I386 /out:"$(OUTDIR)/Adt.exe" 
LINK32_OBJS= \
	"$(INTDIR)/ADT.obj" \
	"$(INTDIR)/DiskXfer.obj" \
	"$(INTDIR)/comm.obj" \
	"$(INTDIR)/ADTDlg.obj" \
	"$(INTDIR)/ringbuf.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/ADT.res"

"$(OUTDIR)\Adt.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ADT - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Adt.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\Adt.pch"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\Adt.exe"
	-@erase ".\Debug\ADT.obj"
	-@erase ".\Debug\comm.obj"
	-@erase ".\Debug\DiskXfer.obj"
	-@erase ".\Debug\ADTDlg.obj"
	-@erase ".\Debug\ringbuf.obj"
	-@erase ".\Debug\StdAfx.obj"
	-@erase ".\Debug\ADT.res"
	-@erase ".\Debug\Adt.ilk"
	-@erase ".\Debug\Adt.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Adt.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ADT.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Adt.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Adt.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Adt.exe" 
LINK32_OBJS= \
	"$(INTDIR)/ADT.obj" \
	"$(INTDIR)/comm.obj" \
	"$(INTDIR)/DiskXfer.obj" \
	"$(INTDIR)/ADTDlg.obj" \
	"$(INTDIR)/ringbuf.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/ADT.res"

"$(OUTDIR)\Adt.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "ADT - Win32 Release"
# Name "ADT - Win32 Debug"

!IF  "$(CFG)" == "ADT - Win32 Release"

!ELSEIF  "$(CFG)" == "ADT - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ADTDlg.cpp
DEP_CPP_ADTDL=\
	".\StdAfx.h"\
	".\ADT.h"\
	".\ADTDlg.h"\
	".\comm.h"\
	".\DiskXfer.h"\
	".\ringbuf.h"\
	

"$(INTDIR)\ADTDlg.obj" : $(SOURCE) $(DEP_CPP_ADTDL) "$(INTDIR)"\
 "$(INTDIR)\Adt.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "ADT - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /Ot /Oi /Oy /Ob2 /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Adt.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Adt.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "ADT - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Adt.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Adt.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ADT.rc
DEP_RSC_ADT_R=\
	".\res\ADT.ico"\
	".\res\ADT.rc2"\
	

"$(INTDIR)\ADT.res" : $(SOURCE) $(DEP_RSC_ADT_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\comm.cpp
DEP_CPP_COMM_=\
	".\StdAfx.h"\
	".\comm.h"\
	".\ringbuf.h"\
	

"$(INTDIR)\comm.obj" : $(SOURCE) $(DEP_CPP_COMM_) "$(INTDIR)"\
 "$(INTDIR)\Adt.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ringbuf.cpp
DEP_CPP_RINGB=\
	".\StdAfx.h"\
	".\ringbuf.h"\
	

"$(INTDIR)\ringbuf.obj" : $(SOURCE) $(DEP_CPP_RINGB) "$(INTDIR)"\
 "$(INTDIR)\Adt.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ADT.cpp
DEP_CPP_ADT_C=\
	".\StdAfx.h"\
	".\ADT.h"\
	".\ADTDlg.h"\
	".\comm.h"\
	".\DiskXfer.h"\
	".\ringbuf.h"\
	

"$(INTDIR)\ADT.obj" : $(SOURCE) $(DEP_CPP_ADT_C) "$(INTDIR)"\
 "$(INTDIR)\Adt.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DiskXfer.cpp
DEP_CPP_DISKX=\
	".\StdAfx.h"\
	".\DiskXfer.h"\
	".\comm.h"\
	".\ringbuf.h"\
	

"$(INTDIR)\DiskXfer.obj" : $(SOURCE) $(DEP_CPP_DISKX) "$(INTDIR)"\
 "$(INTDIR)\Adt.pch"


# End Source File
# End Target
# End Project
################################################################################
################################################################################
# Section ADT : {648A5601-2C6E-101B-82B6-000000000014}
# 	2:5:Class:CCMSCommCtrl
# 	2:10:HeaderFile:cmscommctrl.h
# 	2:8:ImplFile:cmscommctrl.cpp
# End Section
################################################################################
################################################################################
# Section ADT : {648A5600-2C6E-101B-82B6-000000000014}
# 	0:15:CMSCommCtrl.cpp:C:\MSDEV\Projects\ADT\CMSCommCtrl.cpp
# 	0:13:CMSCommCtrl.h:C:\MSDEV\Projects\ADT\CMSCommCtrl.h
# 	2:21:DefaultSinkHeaderFile:cmscommctrl.h
# 	2:16:DefaultSinkClass:CCMSCommCtrl
# End Section
################################################################################
################################################################################
# Section OLE Controls
# 	{648A5600-2C6E-101B-82B6-000000000014}
# End Section
################################################################################
