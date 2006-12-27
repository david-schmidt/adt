// ADT.h : main header file for the ADT application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CADTCmdLine : public CCommandLineInfo
{
public:
	CString port;
	CString speed;

	CADTCmdLine () : port (_T("1")), speed (_T("19200")) {};
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );
};

/////////////////////////////////////////////////////////////////////////////
// CADTApp:
// See ADT.cpp for the implementation of this class
//

class CADTApp : public CWinApp
{
public:
	CADTApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADTApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CADTApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
