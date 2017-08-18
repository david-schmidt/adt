// ADT.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ADT.h"
#include "ADTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CADTCmdLine::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	long n = atol (lpszParam);
	if (n < 10)                         /* kluge: a number is a port */
		port.Format(_T("%d"), n);       /*iff it has only one digit */
	else
		speed.Format(_T("%d"), n);
}

/////////////////////////////////////////////////////////////////////////////
// CADTApp

BEGIN_MESSAGE_MAP(CADTApp, CWinApp)
	//{{AFX_MSG_MAP(CADTApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADTApp construction

CADTApp::CADTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CADTApp object

CADTApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CADTApp initialization

BOOL CADTApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#if 1
	CADTCmdLine cmd;
	ParseCommandLine (cmd);

	CADTDlg * dlg = new CADTDlg;
	m_pMainWnd = dlg;
	dlg->Create (IDD_ADT_DIALOG, cmd);
	return TRUE;
#else
	CADTDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
#endif
}
